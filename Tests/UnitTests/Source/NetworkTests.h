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
void runTestServer(unsigned maxConnectionCount, uint8_t channelCount, std::function<void(Socket&, SocketEvent&)> eventHandler)
{
    Socket socket(6006, maxConnectionCount, channelCount);

    bool connection = false;
    bool disconnection = false;

    SocketEvent event;
    while (socket.pollEvent(event, TimeSpan::fromMilliseconds(5000)))
    {
        eventHandler(socket, event);

        if (event.type == SocketEventType::Connect)
        {
            connection = true;
        }
        else if (event.type == SocketEventType::Disconnect)
        {
            disconnection = true;
            break;
        }
    }

    CHECK(connection);
    CHECK(disconnection);
}

void runTestClient(unsigned maxConnectionCount, uint8_t channelCount, std::function<void(Socket&, SocketEvent&)> eventHandler)
{
    Socket socket(maxConnectionCount, channelCount);
    Peer server = socket.connectToPeer(IpAddress::localAddress(), 6006);

    bool connection = false;
    bool disconnection = false;

    SocketEvent event;
    while (socket.pollEvent(event, TimeSpan::fromMilliseconds(500)))
    {
        eventHandler(socket, event);

        if (event.type == SocketEventType::Connect)
        {
            connection = true;
        }
    }

    CHECK(PeerState::Connected == server.state());

    if (connection)
    {
        socket.disconnectFromPeer(server);

        while (socket.pollEvent(event, TimeSpan::fromMilliseconds(500)))
        {
            if (event.type == SocketEventType::Disconnect)
            {
                disconnection = true;
            }
        }
    }

    CHECK(connection);
    CHECK(disconnection);
}


SUITE(Network)
{
    TEST(ConnectionAndDisconnection)
    {
        TaskPool taskPool(4);

        Task serverTask = taskPool.enqueue([]
        {
            runTestServer(1, 1, [] (Socket&, SocketEvent&) { });
        });

        Task clientTask = taskPool.enqueue([]
        {
            runTestClient(1, 1, [] (Socket&, SocketEvent&) { });
        });

        clientTask.wait();
        serverTask.wait();
    }

    TEST(SendAndReceivePacket)
    {
        TaskPool taskPool(4);

        Task serverTask = taskPool.enqueue([]
        {
            runTestServer(1, 1, [] (Socket& socket, SocketEvent& event)
            {
                if (event.type == SocketEventType::Connect)
                {
                    Packet packet(PacketFlag::Reliable);

                    PacketWriteStream stream = packet.writeStream();
                    stream.writeString("Hello");

                    socket.sendPacket(event.peer, 0, packet);
                    socket.flush();
                }
            });
        });

        Task clientTask = taskPool.enqueue([]
        {
            runTestClient(1, 1, [] (Socket& socket, SocketEvent& event)
            {
                socket;

                if (event.type == SocketEventType::Receive)
                {
                    PacketReadStream stream = event.packet.readStream();
                    std::string message = stream.readString();
                    CHECK_EQUAL("Hello", message);
                }
            });
        });

        clientTask.wait();
        serverTask.wait();
    }
}