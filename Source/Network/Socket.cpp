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
#include "Hect.h"

using namespace hect;

#include <enet/enet.h>

int _enetInitializationCounter = 0;

SocketEvent::SocketEvent() :
    type(SocketEventType::None)
{
}

Socket::Socket(unsigned maxConnectionCount, uint8_t channelCount) :
    _enetHost(nullptr)
{
    if (_enetInitializationCounter == 0)
    {
        ++_enetInitializationCounter;
        if (enet_initialize() != 0)
        {
            throw Error("Failed to initialized ENet");
        }
    }

    _enetHost = enet_host_create(nullptr, maxConnectionCount, channelCount, 0, 0);
    if (!_enetHost)
    {
        throw Error("Failed to create socket");
    }
}

Socket::Socket(Port port, unsigned maxConnectionCount, uint8_t channelCount) :
    _enetHost(nullptr)
{
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = port;
    _enetHost = enet_host_create(&address, maxConnectionCount, channelCount, 0, 0);
    if (!_enetHost)
    {
        throw Error("Failed to create socket");
    }
}

Socket::~Socket()
{
    if (_enetHost)
    {
        enet_host_destroy((ENetHost*)_enetHost);
        _enetHost = nullptr;
    }

    if (_enetInitializationCounter != 0)
    {
        --_enetInitializationCounter;
        enet_deinitialize();
    }
}

Peer Socket::connectToPeer(IpAddress address, Port port)
{
    ENetAddress enetAddress;
    enetAddress.host = (uint32_t)address;
    enetAddress.port = port;

    ENetPeer* enetPeer = enet_host_connect((ENetHost*)_enetHost, &enetAddress, ((ENetHost*)_enetHost)->channelLimit, 0);
    if (!enetPeer)
    {
        throw Error("Failed to create peer");
    }

    Peer peer;
    peer._enetPeer = enetPeer;

    _peers.push_back(peer);

    return peer;
}

void Socket::disconnectFromPeer(Peer peer)
{
    // Ensure the peer belongs to this socket
    if (std::find(_peers.begin(), _peers.end(), peer) == _peers.end())
    {
        return;
    }

    // Remove the peer
    _peers.erase(std::remove(_peers.begin(), _peers.end(), peer), _peers.end());

    // Trigger the disconnect
    ENetPeer* enetPeer = (ENetPeer*)peer._enetPeer;
    if (peer.state() == PeerState::Connected)
    {
        enet_peer_disconnect(enetPeer, 0);
    }
    else
    {
        enet_peer_reset(enetPeer);
    }
}

bool Socket::pollEvent(SocketEvent& event, TimeSpan timeOut)
{
    ENetEvent enetEvent;
    if (enet_host_service((ENetHost*)_enetHost, &enetEvent, (uint32_t)timeOut.milliseconds()) > 0)
    {
        event.type = (SocketEventType)enetEvent.type;
        event.peer._enetPeer = enetEvent.peer;

        if (enetEvent.type == ENET_EVENT_TYPE_RECEIVE)
        {
            std::vector<uint8_t> data(enetEvent.packet->dataLength, 0);
            std::memcpy(&data[0], enetEvent.packet->data, data.size());
            event.packet = Packet(data);
            enet_packet_destroy(enetEvent.packet);
        }

        return true;
    }

    return false;
}

void Socket::sendPacket(Peer peer, uint8_t channel, const Packet& packet)
{
    const std::vector<uint8_t>& data = packet._data;
    ENetPacket* enetPacket = enet_packet_create(&data[0], data.size(), packet._flags);
    enet_peer_send((ENetPeer*)peer._enetPeer, channel, enetPacket);
}

void Socket::broadcastPacket(uint8_t channel, const Packet& packet)
{
    const std::vector<uint8_t>& data = packet._data;
    ENetPacket* enetPacket = enet_packet_create(&data[0], data.size(), packet._flags);
    enet_host_broadcast((ENetHost*)_enetHost, channel, enetPacket);
}

void Socket::flush()
{
    enet_host_flush((ENetHost*)_enetHost);
}