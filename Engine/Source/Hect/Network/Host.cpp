///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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
#include "Host.h"

#include <cstring>
#include <enet/enet.h>
#include <iostream>

#include "Hect/Core/Exception.h"
#include "Hect/Core/Format.h"
#include "Hect/Core/Logging.h"

using namespace hect;

Host::Host(size_t maxPeerCount, size_t channelCount, Port port)
{
    int* x = new int(123);
    std::cout << *x << std::endl;
    
    initializeENet();

    // If a port was specified
    if (port)
    {
        // Create the address to listen from
        ENetAddress address;
        address.host = ENET_HOST_ANY;
        address.port = port;

        // Create a listening ENet host
        _enetHost = enet_host_create(&address, maxPeerCount, channelCount, 0, 0);

        HECT_INFO(format("Created local host listening on port %d", port));
    }
    else
    {
        _enetHost = enet_host_create(nullptr, maxPeerCount, channelCount, 0, 0);

        HECT_INFO(format("Created local host"));
    }

    // Verify that the host was successfully created
    if (!_enetHost)
    {
        throw FatalError("Failed to create host");
    }
}

Host::~Host()
{
    // Destroy the associated host if there is one
    if (_enetHost)
    {
        if (_enetHost->address.port)
        {
            HECT_INFO(format("Destroyed local host listening on port %d", _enetHost->address.port));
        }
        else
        {
            HECT_INFO("Destroyed local host");
        }

        enet_host_destroy(_enetHost);
    }
}

Peer Host::requestConnectTo(IPAddress address, Port port)
{
    // Create the ENet address to connect to
    ENetAddress enetAddress;
    enetAddress.host = static_cast<uint32_t>(address);
    enetAddress.port = port;

    // Trigger the connection
    ENetPeer* enetPeer = enet_host_connect(_enetHost, &enetAddress, _enetHost->channelLimit, 0);
    if (!enetPeer)
    {
        throw FatalError("Failed to create peer");
    }

    HECT_INFO(format("Requested connection to remote host at address %s on port %d", address.asString().c_str(), port));

    // Return the associated peer
    return Peer(enetPeer);
}

void Host::requestDisconnectFrom(Peer peer)
{
    // Trigger the disconnect
    if (peer.state() == PeerState::Connected)
    {
        enet_peer_disconnect(peer._enetPeer, 0);
        HECT_INFO(format("Requested disconnection from remote host at address %s", peer.address().asString().c_str()));
    }
    else
    {
        enet_peer_reset(peer._enetPeer);
    }
}

bool Host::pollEvent(PeerEvent& event, TimeSpan timeOut)
{
    // Poll for the ENet event
    ENetEvent enetEvent;
    if (enet_host_service(_enetHost, &enetEvent, static_cast<uint32_t>(timeOut.milliseconds())) > 0)
    {
        Peer peer(enetEvent.peer);

        // Create the event
        event = PeerEvent();
        event.type = static_cast<PeerEventType>(enetEvent.type);
        event.peer = peer;

        // If it is a packet receive event then copy the packet data to the
        // packet of the event
        if (enetEvent.type == ENET_EVENT_TYPE_RECEIVE)
        {
            ByteVector packetData(enetEvent.packet->dataLength, 0);
            std::memcpy(&packetData[0], enetEvent.packet->data, packetData.size());
            event.packetData = std::move(packetData);
            enet_packet_destroy(enetEvent.packet);
        }

        // Log connection/disconnection events
        switch (event.type)
        {
        case PeerEventType::Connect:
            HECT_INFO(format("Connected to remote host at address %s (peer id = %d)", peer.address().asString().c_str(), peer.id()));
            break;
        case PeerEventType::Disconnect:
            HECT_INFO(format("Disconnected from remote host at address %s (peer id = %d)", peer.address().asString().c_str(), peer.id()));
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

void Host::sendPacket(Peer peer, Channel channel, const ByteVector& packetData, PacketFlags packetFlags)
{
    ENetPacket* enetPacket = enet_packet_create(&packetData[0], packetData.size(), packetFlags);
    enet_peer_send(peer._enetPeer, channel, enetPacket);
}

void Host::broadcastPacket(Channel channel, const ByteVector& packetData, PacketFlags packetFlags)
{
    ENetPacket* enetPacket = enet_packet_create(&packetData[0], packetData.size(), packetFlags);
    enet_host_broadcast(_enetHost, channel, enetPacket);
}

void Host::flush()
{
    enet_host_flush(_enetHost);
}

void Host::initializeENet()
{
    static bool initialized = false;

    if (!initialized)
    {
        initialized = true;

        // Intialize ENet
        if (enet_initialize() != 0)
        {
            throw FatalError("Failed to initialized ENet");
        }

        // Log the ENet version
        ENetVersion version = ENET_VERSION;
        HECT_INFO(format("ENet version %d.%d.%d", ENET_VERSION_GET_MAJOR(version), ENET_VERSION_GET_MINOR(version), ENET_VERSION_GET_PATCH(version)));

        // Deininitialize ENet at exit
        atexit(enet_deinitialize);
    }
}
