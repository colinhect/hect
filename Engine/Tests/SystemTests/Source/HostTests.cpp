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
#include <Hect/Core/Exception.h>
#include <Hect/IO/BinaryDecoder.h>
#include <Hect/IO/BinaryEncoder.h>
#include <Hect/Network/Host.h>

using namespace hect;

#include <atomic>
#include <catch.hpp>
#include <mutex>
#include <thread>

static const char* _host = "localhost";
static const size_t _maxPeerCount = 1;
static const size_t _channelCount = 1;
static const Port _port = 1234;
static const TimeSpan _timeOut = TimeSpan::fromMilliseconds(100);

// Catch macros are not thread-safe; this macro provides a simple fix for that
static std::mutex _catchMutex;
#define THREADSAFE_REQUIRE(expr)\
    {\
        std::lock_guard<std::mutex> lock(_catchMutex);\
        REQUIRE(expr);\
    }

TEST_CASE("Connect and disconnect from local host", "[Host]")
{
    std::atomic<bool> listening;
    listening.store(false);

    std::thread serverThread([&]
    {
        bool connectEvent = false;
        bool disconnectEvent = false;

        Host host(_maxPeerCount, _channelCount, _port);

        listening.store(true);

        while (true)
        {
            PeerEvent event;
            if (host.pollEvent(event))
            {
                if (event.type == PeerEventType_Connect)
                {
                    connectEvent = true;
                }
                else if (event.type == PeerEventType_Disconnect)
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

        Host host(_maxPeerCount, _channelCount);

        Peer peer = host.requestConnectTo(_host, _port);
        THREADSAFE_REQUIRE(peer.state() == PeerState_Connecting);

        PeerEvent event;
        while (host.pollEvent(event, _timeOut))
        {
            if (event.type == PeerEventType_Connect)
            {
                THREADSAFE_REQUIRE(event.peer == peer);
                THREADSAFE_REQUIRE(peer.state() == PeerState_Connected);
                connectEvent = true;
            }
        }
        THREADSAFE_REQUIRE(connectEvent == true);

        host.requestDisconnectFrom(peer);
        THREADSAFE_REQUIRE(peer.state() == PeerState_Disconnecting);

        while (host.pollEvent(event, _timeOut))
        {
            if (event.type == PeerEventType_Disconnect)
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

TEST_CASE("Send a packet from the server host", "[Host]")
{
    std::atomic<bool> listening;
    listening.store(false);

    std::thread serverThread([&]
    {
        Host host(_maxPeerCount, _channelCount, _port);

        listening.store(true);

        while (true)
        {
            PeerEvent event;
            if (host.pollEvent(event))
            {
                if (event.type == PeerEventType_Connect)
                {
                    ByteVector packetData;

                    BinaryEncoder encoder(packetData);
                    encoder << encodeValue("Testing...");

                    host.sendPacket(event.peer, 0, packetData);
                    host.flush();
                }
                else if (event.type == PeerEventType_Disconnect)
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

        Host host(_maxPeerCount, _channelCount);

        Peer peer = host.requestConnectTo(_host, _port);

        PeerEvent event;
        while (host.pollEvent(event, _timeOut))
        {
            if (event.type == PeerEventType_ReceivePacket)
            {
                packetReceived = true;

                std::string string;

                BinaryDecoder decoder(event.packetData);
                decoder >> decodeValue(string);

                THREADSAFE_REQUIRE(string == "Testing...");
            }
        }
        THREADSAFE_REQUIRE(packetReceived);

        host.requestDisconnectFrom(peer);

        while (host.pollEvent(event, _timeOut))
        {
            if (event.type == PeerEventType_Disconnect)
            {
                break;
            }
        }
    });

    clientThread.join();
    serverThread.join();
}

TEST_CASE("Send a packet from the client host", "[Host]")
{
    std::atomic<bool> listening;
    listening.store(false);

    std::thread serverThread([&]
    {
        Host host(_maxPeerCount, _channelCount, _port);

        listening.store(true);

        bool packetReceived = false;
        while (true)
        {
            PeerEvent event;
            if (host.pollEvent(event))
            {
                if (event.type == PeerEventType_ReceivePacket)
                {
                    packetReceived = true;

                    std::string string;

                    BinaryDecoder decoder(event.packetData);
                    decoder >> decodeValue(string);

                    THREADSAFE_REQUIRE(string == "Testing...");
                }
                else if (event.type == PeerEventType_Disconnect)
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

        Host host(_maxPeerCount, _channelCount);

        Peer peer = host.requestConnectTo(_host, _port);

        PeerEvent event;
        while (host.pollEvent(event, _timeOut))
        {
            if (event.type == PeerEventType_Connect)
            {
                ByteVector packetData;

                BinaryEncoder encoder(packetData);
                encoder << encodeValue("Testing...");

                host.sendPacket(event.peer, 0, packetData);
                host.flush();
            }
        }

        host.requestDisconnectFrom(peer);

        while (host.pollEvent(event, _timeOut))
        {
            if (event.type == PeerEventType_Disconnect)
            {
                break;
            }
        }
    });

    clientThread.join();
    serverThread.join();
}