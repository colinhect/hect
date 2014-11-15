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
        default:
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