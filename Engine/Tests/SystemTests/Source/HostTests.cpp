///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2016 Colin Hill
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
#include <Hect.h>
using namespace hect;

#include <atomic>
#include <catch.hpp>
#include <mutex>
#include <thread>

static const char* _host = "localhost";
static const size_t _maxPeerCount = 1;
static const size_t _channelCount = 1;
static const Port _port = 1234;
static const Milliseconds _timeOut(100);

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

    std::thread server_thread([&]
    {
        bool connect_event = false;
        bool disconnect_event = false;

        Host host(_maxPeerCount, _channelCount, _port);

        listening.store(true);

        while (true)
        {
            PeerEvent event;
            if (host.poll_event(event))
            {
                if (event.type == PeerEventType::Connect)
                {
                    connect_event = true;
                }
                else if (event.type == PeerEventType::Disconnect)
                {
                    disconnect_event = true;
                    break;
                }
            }
        }

        THREADSAFE_REQUIRE(connect_event == true);
        THREADSAFE_REQUIRE(disconnect_event == true);
    });

    std::thread client_thread([&]
    {
        bool connect_event = false;
        bool disconnect_event = false;

        while (!listening)
        {
            std::this_thread::yield();
        }

        Host host(_maxPeerCount, _channelCount);

        Peer peer = host.request_connect_to(_host, _port);
        THREADSAFE_REQUIRE(peer.state() == PeerState::Connecting);

        PeerEvent event;
        while (host.poll_event(event, _timeOut))
        {
            if (event.type == PeerEventType::Connect)
            {
                THREADSAFE_REQUIRE(event.peer == peer);
                THREADSAFE_REQUIRE(peer.state() == PeerState::Connected);
                connect_event = true;
            }
        }
        THREADSAFE_REQUIRE(connect_event == true);

        host.request_disconnect_from(peer);
        THREADSAFE_REQUIRE(peer.state() == PeerState::Disconnecting);

        while (host.poll_event(event, _timeOut))
        {
            if (event.type == PeerEventType::Disconnect)
            {
                THREADSAFE_REQUIRE(event.peer == peer);
                THREADSAFE_REQUIRE(peer.state() == PeerState::Disconnected);
                disconnect_event = true;
            }
        }
        THREADSAFE_REQUIRE(disconnect_event == true);
    });

    client_thread.join();
    server_thread.join();
}

TEST_CASE("Send a packet from the server host", "[Host]")
{
    std::atomic<bool> listening;
    listening.store(false);

    std::thread server_thread([&]
    {
        Host host(_maxPeerCount, _channelCount, _port);

        listening.store(true);

        while (true)
        {
            PeerEvent event;
            if (host.poll_event(event))
            {
                if (event.type == PeerEventType::Connect)
                {
                    ByteVector packet_data;

                    BinaryEncoder encoder(packet_data);
                    encoder << encode_value("Testing...");

                    host.send_packet(event.peer, 0, packet_data);
                    host.flush();
                }
                else if (event.type == PeerEventType::Disconnect)
                {
                    break;
                }
            }
        }
    });

    std::thread client_thread([&]
    {
        bool packet_received = false;

        while (!listening)
        {
            std::this_thread::yield();
        }

        Host host(_maxPeerCount, _channelCount);

        Peer peer = host.request_connect_to(_host, _port);

        PeerEvent event;
        while (host.poll_event(event, _timeOut))
        {
            if (event.type == PeerEventType::ReceivePacket)
            {
                packet_received = true;

                std::string string;

                BinaryDecoder decoder(event.packet_data);
                decoder >> decode_value(string);

                THREADSAFE_REQUIRE(string == "Testing...");
            }
        }
        THREADSAFE_REQUIRE(packet_received);

        host.request_disconnect_from(peer);

        while (host.poll_event(event, _timeOut))
        {
            if (event.type == PeerEventType::Disconnect)
            {
                break;
            }
        }
    });

    client_thread.join();
    server_thread.join();
}

TEST_CASE("Send a packet from the client host", "[Host]")
{
    std::atomic<bool> listening;
    listening.store(false);

    std::thread server_thread([&]
    {
        Host host(_maxPeerCount, _channelCount, _port);

        listening.store(true);

        bool packet_received = false;
        while (true)
        {
            PeerEvent event;
            if (host.poll_event(event))
            {
                if (event.type == PeerEventType::ReceivePacket)
                {
                    packet_received = true;

                    std::string string;

                    BinaryDecoder decoder(event.packet_data);
                    decoder >> decode_value(string);

                    THREADSAFE_REQUIRE(string == "Testing...");
                }
                else if (event.type == PeerEventType::Disconnect)
                {
                    break;
                }
            }
        }

        THREADSAFE_REQUIRE(packet_received == true);
    });

    std::thread client_thread([&]
    {
        while (!listening)
        {
            std::this_thread::yield();
        }

        Host host(_maxPeerCount, _channelCount);

        Peer peer = host.request_connect_to(_host, _port);

        PeerEvent event;
        while (host.poll_event(event, _timeOut))
        {
            if (event.type == PeerEventType::Connect)
            {
                ByteVector packet_data;

                BinaryEncoder encoder(packet_data);
                encoder << encode_value("Testing...");

                host.send_packet(event.peer, 0, packet_data);
                host.flush();
            }
        }

        host.request_disconnect_from(peer);

        while (host.poll_event(event, _timeOut))
        {
            if (event.type == PeerEventType::Disconnect)
            {
                break;
            }
        }
    });

    client_thread.join();
    server_thread.join();
}
