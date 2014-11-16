///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2014 Colin Hill
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////
#include <Hect/Core/Error.h>
#include <Hect/IO/BinaryDecoder.h>
#include <Hect/IO/BinaryEncoder.h>
#include <Hect/Network/Socket.h>

using namespace hect;

#include <atomic>
#include <catch.hpp>
#include <thread>

static const char* _host = "localhost";
static const Port _port = 1234;
static const TimeSpan _timeOut = TimeSpan::fromMilliseconds(100);

// Catch macros are not thread-safe; this macro provides a simple fix for that
static std::mutex _catchMutex;
#define THREADSAFE_REQUIRE(expr)\
    {\
        std::lock_guard<std::mutex> lock(_catchMutex);\
        REQUIRE(expr);\
    }

TEST_CASE("Socket_ListenAfterConnectRequest")
{
    Socket socket;
    socket.requestConnectTo(_host, _port);
    REQUIRE_THROWS_AS(socket.listenOnPort(_port), Error);
}

TEST_CASE("Socket_ListenAgain")
{
    Socket socket;
    socket.listenOnPort(_port);
    REQUIRE_THROWS_AS(socket.listenOnPort(_port), Error);
}

TEST_CASE("Socket_ConnectDisconnect")
{
    std::atomic<bool> listening;
    listening.store(false);

    std::thread serverThread([&]
    {
        bool connectEvent = false;
        bool disconnectEvent = false;

        Socket socket;
        socket.listenOnPort(_port);

        listening.store(true);

        while (true)
        {
            SocketEvent event;
            if (socket.pollEvent(event))
            {
                if (event.type == SocketEventType_Connect)
                {
                    connectEvent = true;
                }
                else if (event.type == SocketEventType_Disconnect)
                {
                    disconnectEvent = true;
                    break;
                }
            }
        }

        THREADSAFE_REQUIRE(connectEvent == true);
        THREADSAFE_REQUIRE(disconnectEvent == true);
    });

    std::thread clientThread([&]
    {
        bool connectEvent = false;
        bool disconnectEvent = false;

        while (!listening)
        {
            std::this_thread::yield();
        }

        Socket socket;

        Peer peer = socket.requestConnectTo(_host, _port);
        THREADSAFE_REQUIRE(peer.state() == PeerState_Connecting);

        SocketEvent event;
        while (socket.pollEvent(event, _timeOut))
        {
            if (event.type == SocketEventType_Connect)
            {
                THREADSAFE_REQUIRE(event.peer == peer);
                THREADSAFE_REQUIRE(peer.state() == PeerState_Connected);
                connectEvent = true;
            }
        }
        THREADSAFE_REQUIRE(connectEvent == true);

        socket.requestDisconnectFrom(peer);
        THREADSAFE_REQUIRE(peer.state() == PeerState_Disconnecting);

        while (socket.pollEvent(event, _timeOut))
        {
            if (event.type == SocketEventType_Disconnect)
            {
                THREADSAFE_REQUIRE(event.peer == peer);
                THREADSAFE_REQUIRE(peer.state() == PeerState_Disconnected);
                disconnectEvent = true;
            }
        }
        THREADSAFE_REQUIRE(disconnectEvent == true);
    });

    clientThread.join();
    serverThread.join();
}

TEST_CASE("Socket_ServerSendPacket")
{
    std::atomic<bool> listening;
    listening.store(false);

    std::thread serverThread([&]
    {
        Socket socket;
        socket.listenOnPort(_port);

        listening.store(true);

        while (true)
        {
            SocketEvent event;
            if (socket.pollEvent(event))
            {
                if (event.type == SocketEventType_Connect)
                {
                    ByteVector data;

                    BinaryEncoder encoder(data);
                    encoder << encodeValue("Testing...");

                    Packet packet(data, PacketFlag_Reliable);
                    socket.sendPacket(event.peer, 0, packet);
                    socket.flush();
                }
                else if (event.type == SocketEventType_Disconnect)
                {
                    break;
                }
            }
        }
    });

    std::thread clientThread([&]
    {
        bool packetReceived = false;

        while (!listening)
        {
            std::this_thread::yield();
        }

        Socket socket;

        Peer peer = socket.requestConnectTo(_host, _port);

        SocketEvent event;
        while (socket.pollEvent(event, _timeOut))
        {
            if (event.type == SocketEventType_Receive)
            {
                packetReceived = true;

                std::string string;

                BinaryDecoder decoder(event.packet.data());
                decoder >> decodeValue(string);

                THREADSAFE_REQUIRE(string == "Testing...");
            }
        }
        THREADSAFE_REQUIRE(packetReceived);

        socket.requestDisconnectFrom(peer);

        while (socket.pollEvent(event, _timeOut))
        {
            if (event.type == SocketEventType_Disconnect)
            {
                break;
            }
        }
    });

    clientThread.join();
    serverThread.join();
}

TEST_CASE("Socket_ClientSendPacket")
{
    std::atomic<bool> listening;
    listening.store(false);

    std::thread serverThread([&]
    {
        Socket socket;
        socket.listenOnPort(_port);

        listening.store(true);

        bool packetReceived = false;
        while (true)
        {
            SocketEvent event;
            if (socket.pollEvent(event))
            {
                if (event.type == SocketEventType_Receive)
                {
                    packetReceived = true;

                    std::string string;

                    BinaryDecoder decoder(event.packet.data());
                    decoder >> decodeValue(string);

                    THREADSAFE_REQUIRE(string == "Testing...");
                }
                else if (event.type == SocketEventType_Disconnect)
                {
                    break;
                }
            }
        }

        THREADSAFE_REQUIRE(packetReceived == true);
    });

    std::thread clientThread([&]
    {
        while (!listening)
        {
            std::this_thread::yield();
        }

        Socket socket;

        Peer peer = socket.requestConnectTo(_host, _port);

        SocketEvent event;
        while (socket.pollEvent(event, _timeOut))
        {
            if (event.type == SocketEventType_Connect)
            {
                ByteVector data;

                BinaryEncoder encoder(data);
                encoder << encodeValue("Testing...");

                Packet packet(data, PacketFlag_Reliable);
                socket.sendPacket(event.peer, 0, packet);
                socket.flush();
            }
        }

        socket.requestDisconnectFrom(peer);

        while (socket.pollEvent(event, _timeOut))
        {
            if (event.type == SocketEventType_Disconnect)
            {
                break;
            }
        }
    });

    clientThread.join();
    serverThread.join();
}