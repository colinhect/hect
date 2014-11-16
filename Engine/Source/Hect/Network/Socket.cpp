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

#include <cstring>
#include <enet/enet.h>

#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"
#include "Hect/Core/Logging.h"

using namespace hect;

Socket::Socket()
{
    initializeENet();
}

Socket::Socket(size_t peerCount, size_t channelCount) :
    _peerCount(peerCount),
    _channelCount(channelCount)
{
    initializeENet();
}

Socket::~Socket()
{
    // Destroy the associated host if there is one
    if (_enetHost)
    {
        enet_host_destroy(_enetHost);
        _enetHost = nullptr;
    }

    deinitializeENet();
}

void Socket::listenOnPort(Port port)
{
    // Ensure that the socket is not already listening
    if (_listening)
    {
        throw Error("Socket is already listening on a port");
    }
    _listening = true;

    // Ensure that the socket has not attempted to connect to a remote socket
    if (_enetHost)
    {
        throw Error("Socket has active connections and cannot listen on a port");
    }

    // Create the ENet address to listen from
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = port;

    // Create the ENet host
    _enetHost = enet_host_create(&address, _peerCount, _channelCount, 0, 0);
    if (!_enetHost)
    {
        throw Error("Failed to create ENet host");
    }

    HECT_INFO(format("Listening for connections on port %i", port));
}

Peer Socket::requestConnectTo(IPAddress address, Port port)
{
    // Create the ENet host (non-listening) if a host was not already created
    if (!_enetHost)
    {
        _enetHost = enet_host_create(nullptr, _peerCount, _channelCount, 0, 0);
        if (!_enetHost)
        {
            throw Error("Failed to create ENet host");
        }
    }

    // Create the ENet address to connect to
    ENetAddress enetAddress;
    enetAddress.host = static_cast<uint32_t>(address);
    enetAddress.port = port;

    // Perform the connection
    ENetPeer* enetPeer = enet_host_connect(_enetHost, &enetAddress, _enetHost->channelLimit, 0);
    if (!enetPeer)
    {
        throw Error("Failed to create ENet peer");
    }

    HECT_INFO(format("Requested connection to peer at address %s on port %d", address.asString().c_str(), port));

    // Return the associated peer
    return Peer(enetPeer);
}

void Socket::requestDisconnectFrom(Peer peer)
{
    // Trigger the disconnect
    if (peer.state() == PeerState_Connected)
    {
        enet_peer_disconnect(peer._enetPeer, 0);
        HECT_INFO(format("Requested disconnection from peer at address %s", peer.address().asString().c_str()));
    }
    else
    {
        enet_peer_reset(peer._enetPeer);
    }
}

bool Socket::pollEvent(SocketEvent& event, TimeSpan timeOut)
{
    // Poll for the ENet event
    ENetEvent enetEvent;
    if (enet_host_service(_enetHost, &enetEvent, static_cast<uint32_t>(timeOut.milliseconds())) > 0)
    {
        Peer peer(enetEvent.peer);

        // Create the event
        event = SocketEvent();
        event.type = static_cast<SocketEventType>(enetEvent.type);
        event.peer = peer;

        // If it is a packet receive event then copy the packet data to the
        // packet of the event
        if (enetEvent.type == ENET_EVENT_TYPE_RECEIVE)
        {
            ByteVector data(enetEvent.packet->dataLength, 0);
            std::memcpy(&data[0], enetEvent.packet->data, data.size());
            event.packet = Packet(data, PacketFlag_None);
            enet_packet_destroy(enetEvent.packet);
        }

        // Log connection/disconnection events
        switch (event.type)
        {
        case SocketEventType_Connect:
            HECT_INFO(format("Connected to remote socket at address %s (peer id = %d)", peer.address().asString().c_str(), peer.id()));
            break;
        case SocketEventType_Disconnect:
            HECT_INFO(format("Disconnected from remote socket at address %s (peer id = %d)", peer.address().asString().c_str(), peer.id()));
            break;
        default:
            break;
        }

        // An event was received
        return true;
    }

    // An event was not received
    return false;
}

void Socket::sendPacket(Peer peer, Channel channel, const Packet& packet)
{
    const ByteVector& data = packet.data();
    ENetPacket* enetPacket = enet_packet_create(&data[0], data.size(), packet.flags());
    enet_peer_send(peer._enetPeer, channel, enetPacket);
}

void Socket::broadcastPacket(Channel channel, const Packet& packet)
{
    const ByteVector& data = packet.data();
    ENetPacket* enetPacket = enet_packet_create(&data[0], data.size(), packet.flags());
    enet_host_broadcast(_enetHost, channel, enetPacket);
}

void Socket::flush()
{
    enet_host_flush(_enetHost);
}

void Socket::initializeENet()
{
    std::lock_guard<std::mutex> lock(enetInitializationMutex);

    // If this is the first call to initialize ENet
    if (enetInitializationCounter == 0)
    {
        ++enetInitializationCounter;

        // Intialize ENet
        if (enet_initialize() != 0)
        {
            throw Error("Failed to initialized ENet");
        }

        // Log the ENet version
        ENetVersion version = ENET_VERSION;
        HECT_INFO(format("ENet version %d.%d.%d", ENET_VERSION_GET_MAJOR(version), ENET_VERSION_GET_MINOR(version), ENET_VERSION_GET_PATCH(version)));
    }
    else
    {
        // Already initalized
        ++enetInitializationCounter;
    }
}

void Socket::deinitializeENet()
{
    std::lock_guard<std::mutex> lock(enetInitializationMutex);

    if (enetInitializationCounter == 1)
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