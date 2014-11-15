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
#include <Hect/Core/Logging.h>
#include <Hect/Network/Socket.h>
#include <Hect/Reflection/Enum.h>
#include <Hect/Timing/Timer.h>
using namespace hect;

#include <atomic>
#include <catch.hpp>
#include <thread>

namespace
{

const unsigned _port = 1234;

}

TEST_CASE("Socket_ClientSocketConnect")
{
    // Start a server thread
    std::thread serverThread(
        [&]()
        {
            Socket socket;
            socket.listenOnPort(_port);

            // Ininitely poll events until a client disconnects
            while (true)
            {
                SocketEvent event;
                if (socket.pollEvent(event))
                {
                    if (event.type == SocketEventType_Disconnect)
                    {
                        break;
                    }
                }
            }
        }
    );

    Socket socket;
    SocketEvent event;

    // Request connection to local host
    Peer peer = socket.requestConnectTo("localhost", _port);
    REQUIRE(peer.state() == PeerState_Connecting);

    // Wait for connection event
    bool connectEvent = false;
    while (socket.pollEvent(event, TimeSpan::fromSeconds(1)))
    {
        if (event.type == SocketEventType_Connect)
        {
            connectEvent = true;
            REQUIRE(event.peer == peer);
        }
    }
    REQUIRE(connectEvent);

    REQUIRE(peer.state() == PeerState_Connected);

    // Request disconnection from local host
    socket.requestDisconnectFrom(peer);
    REQUIRE(peer.state() == PeerState_Disconnecting);

    // Wait for disconnection event
    bool disconnectEvent = false;
    while (socket.pollEvent(event, TimeSpan::fromSeconds(1)))
    {
        if (event.type == SocketEventType_Disconnect)
        {
            disconnectEvent = true;
            REQUIRE(event.peer == peer);
        }
    }
    REQUIRE(disconnectEvent);

    REQUIRE(peer.state() == PeerState_Disconnected);

    serverThread.join();
}