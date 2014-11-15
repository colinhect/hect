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

TEST_CASE("Socket_ClientSocketConnect", "[Fails]")
{
    std::atomic_bool serverListening;
    serverListening.store(false);

    // Spin up a server thread
    std::thread serverThread(
        [&]()
        {
            Socket socket(16, 4);
            socket.listenOnPort(_port);

            serverListening.store(true);

            SocketEvent event;
            while (!socket.pollEvent(event))
            {
                socket.flush();
            }
            std::cout << Enum::toString(event.type) << std::endl;
            while (!socket.pollEvent(event))
            {
            }
            std::cout << Enum::toString(event.type) << std::endl;
        }
    );

    while (!serverListening)
    {
        std::this_thread::yield();
    }

    Socket socket(1, 4);

    Peer peer = socket.requestConnectTo("localhost", _port);
    REQUIRE(peer.state() == PeerState_Connecting);

    SocketEvent event;
    if (socket.pollEvent(event, TimeSpan::fromSeconds(5)))
    {
        REQUIRE(event.type == SocketEventType_Connect);
        REQUIRE(event.peer == peer);
    }
    else
    {
        FAIL("Did not receive connect event on client");
    }
    REQUIRE(peer.state() == PeerState_Connected);

    socket.requestDisconnectFrom(peer);
    REQUIRE(peer.state() == PeerState_Disconnecting);

    if (socket.pollEvent(event, TimeSpan::fromSeconds(5)))
    {
        REQUIRE(event.type == SocketEventType_Disconnect);
        REQUIRE(event.peer == peer);
    }
    else
    {
        FAIL("Did not receive disconnect event on client");
    }
    REQUIRE(peer.state() == PeerState_Disconnected);

    serverThread.join();
}