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
#include "Socket.h"

#include <algorithm>
#include <cstring>
#include <enet/enet.h>

#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"
#include "Hect/Core/Logging.h"

using namespace hect;


void Socket::serverTest()
{
    if (enet_initialize() != 0)
    {
        fprintf(stderr, "An error occurred while initializing ENet.\n");
        return;
    }
    ENetAddress address;
    ENetHost * server;
    /* Bind the server to the default localhost.     */
    /* A specific host address can be specified by   */
    /* enet_address_set_host (& address, "x.x.x.x"); */
    address.host = ENET_HOST_ANY;
    /* Bind the server to port 1234. */
    address.port = 1234;
    server = enet_host_create(&address /* the address to bind the server host to */,
                              32      /* allow up to 32 clients and/or outgoing connections */,
                              2      /* allow up to 2 channels to be used, 0 and 1 */,
                              0      /* assume any amount of incoming bandwidth */,
                              0      /* assume any amount of outgoing bandwidth */);
    if (server == NULL)
    {
        fprintf(stderr,
                "An error occurred while trying to create an ENet server host.\n");
        return;
    }

    ENetEvent event;
    /* Wait up to 1000 milliseconds for an event. */
    while (enet_host_service(server, &event, 5000) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            printf("A new client connected from %x:%u.\n",
                   event.peer->address.host,
                   event.peer->address.port);
            /* Store any relevant client information here. */
            event.peer->data = "Client information";
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            printf("A packet of length %u containing %s was received from %s on channel %u.\n",
                   event.packet->dataLength,
                   event.packet->data,
                   event.peer->data,
                   event.channelID);
            /* Clean up the packet now that we're done using it. */
            enet_packet_destroy(event.packet);

            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            printf("%s disconnected.\n", event.peer->data);
            /* Reset the peer's client information. */
            event.peer->data = NULL;
        }
    }

    enet_host_destroy(server);
}

void Socket::clientTest()
{
    if (enet_initialize() != 0)
    {
        fprintf(stderr, "An error occurred while initializing ENet.\n");
        return;
    }
    ENetHost * client;
    client = enet_host_create(NULL /* create a client host */,
                              1 /* only allow 1 outgoing connection */,
                              2 /* allow up 2 channels to be used, 0 and 1 */,
                              57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
                              14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);
    if (client == NULL)
    {
        fprintf(stderr,
                "An error occurred while trying to create an ENet client host.\n");
        return;
    }

    ENetAddress address;
    ENetEvent event;
    ENetPeer *peer;
    /* Connect to some.server.net:1234. */
    enet_address_set_host(&address, "localhost");
    address.port = 1234;
    /* Initiate the connection, allocating the two channels 0 and 1. */
    peer = enet_host_connect(client, &address, 2, 0);
    if (peer == NULL)
    {
        fprintf(stderr,
                "No available peers for initiating an ENet connection.\n");
        exit(EXIT_FAILURE);
    }
    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if (enet_host_service(client, &event, 5000) > 0 &&
            event.type == ENET_EVENT_TYPE_CONNECT)
    {
        puts("Connection to localhost:1234 succeeded.");
    }
    else
    {
        /* Either the 5 seconds are up or a disconnect event was */
        /* received. Reset the peer in the event the 5 seconds   */
        /* had run out without any significant event.            */
        enet_peer_reset(peer);
        puts("Connection to some.server.net:1234 failed.");
    }

    enet_peer_disconnect(peer, 0);
    /* Allow up to 3 seconds for the disconnect to succeed
    * and drop any packets received packets.
    */
    while (enet_host_service(client, &event, 3000) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_RECEIVE:
            enet_packet_destroy(event.packet);
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
            puts("Disconnection succeeded.");
            return;
        }
    }
    /* We've arrived here, so the disconnect attempt didn't */
    /* succeed yet.  Force the connection down.             */
    enet_peer_reset(peer);

    enet_host_destroy(client);
}

Socket::Socket(unsigned peerCount, uint8_t channelCount)
{
    initializeENet();

    _enetHost = enet_host_create(nullptr, peerCount, channelCount, 0, 0);
    if (!_enetHost)
    {
        throw Error("Failed to create ENet host");
    }
}

Socket::~Socket()
{
    if (_enetHost)
    {
        enet_host_destroy(_enetHost);
        _enetHost = nullptr;
    }

    deinitializeENet();
}

void Socket::listenOnPort(Port port)
{
    size_t peerCount = _enetHost->peerCount;
    size_t channelCount = _enetHost->channelLimit;

    enet_host_destroy(_enetHost);
    _enetHost = nullptr;

    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = port;
    _enetHost = enet_host_create(&address, peerCount, channelCount, 0, 0);
    if (!_enetHost)
    {
        throw Error("Failed to create ENet host");
    }

    HECT_INFO(format("Listening for UDP connections on port %i", port));
}

Peer Socket::requestConnectTo(IPAddress address, Port port)
{
    ENetAddress enetAddress;
    enetAddress.host = static_cast<uint32_t>(address);
    enetAddress.port = port;

    ENetPeer* enetPeer = enet_host_connect(_enetHost, &enetAddress, _enetHost->channelLimit, 0);
    if (!enetPeer)
    {
        throw Error("Failed to create ENet peer");
    }

    HECT_INFO(format("Requested connection to UDP peer at address %s on port %d", address.toString().c_str(), port));

    return Peer(enetPeer);
}

void Socket::requestDisconnectFrom(Peer peer)
{
    // Trigger the disconnect
    if (peer.state() == PeerState_Connected)
    {
        enet_peer_disconnect(peer._enetPeer, 0);
        HECT_INFO(format("Requested disconnection from UDP peer at address %s", peer.address().toString().c_str()));
    }
    else
    {
        enet_peer_reset(peer._enetPeer);
    }
}

bool Socket::pollEvent(SocketEvent& event, TimeSpan timeOut)
{
    ENetEvent enetEvent;
    if (enet_host_service(_enetHost, &enetEvent, static_cast<uint32_t>(timeOut.milliseconds())) > 0)
    {
        event = SocketEvent();

        event.type = static_cast<SocketEventType>(enetEvent.type);
        event.peer = Peer(enetEvent.peer);

        if (enetEvent.type == ENET_EVENT_TYPE_RECEIVE)
        {
            std::vector<uint8_t> data(enetEvent.packet->dataLength, 0);
            std::memcpy(&data[0], enetEvent.packet->data, data.size());
            event.packet = Packet(data);
            enet_packet_destroy(enetEvent.packet);
        }

        switch (event.type)
        {
        case SocketEventType_Connect:
            HECT_INFO(format("Connected to remote peer at address %s (peer ID = %d)", event.peer.address().toString().c_str(), event.peer.id()));
            break;
        case SocketEventType_Disconnect:
            HECT_INFO(format("Disconnected from remote peer at address %s (peer ID = %d)", event.peer.address().toString().c_str(), event.peer.id()));
            break;
        }

        return true;
    }

    return false;
}

void Socket::sendPacket(Peer peer, Channel channel, const Packet& packet)
{
    const std::vector<uint8_t>& data = packet._data;
    ENetPacket* enetPacket = enet_packet_create(&data[0], data.size(), packet._flags);
    enet_peer_send(peer._enetPeer, channel, enetPacket);
}

void Socket::broadcastPacket(Channel channel, const Packet& packet)
{
    const std::vector<uint8_t>& data = packet._data;
    ENetPacket* enetPacket = enet_packet_create(&data[0], data.size(), packet._flags);
    enet_host_broadcast(_enetHost, channel, enetPacket);
}

void Socket::flush()
{
    enet_host_flush(_enetHost);
}

void Socket::initializeENet()
{
    std::lock_guard<std::mutex> lock(enetInitializationMutex);

    if (enetInitializationCounter == 0)
    {
        ++enetInitializationCounter;
        if (enet_initialize() != 0)
        {
            throw Error("Failed to initialized ENet");
        }
    }
    else
    {
        ++enetInitializationCounter;
    }
}

void Socket::deinitializeENet()
{
    std::lock_guard<std::mutex> lock(enetInitializationMutex);

    if (enetInitializationCounter != 0)
    {
        --enetInitializationCounter;
        enet_deinitialize();
    }
    else
    {
        --enetInitializationCounter;
    }
}

int Socket::enetInitializationCounter = 0;
std::mutex Socket::enetInitializationMutex;