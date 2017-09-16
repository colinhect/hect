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
#include "Host.h"

#include <cstring>
#include <enet/enet.h>

#include "Hect/Core/Exception.h"
#include "Hect/Core/Format.h"
#include "Hect/Core/Logging.h"

using namespace hect;

Host::Host(size_t max_peer_count, size_t channel_count, Port port)
{
    initialize_e_net();

    // If a port was specified
    if (port)
    {
        // Create the address to listen from
        ENetAddress address;
        address.host = ENET_HOST_ANY;
        address.port = port;

        // Create a listening ENet host
        _enet_host = enet_host_create(&address, max_peer_count, channel_count, 0, 0);

        HECT_INFO(format("Created local host listening on port %d", port));
    }
    else
    {
        _enet_host = enet_host_create(nullptr, max_peer_count, channel_count, 0, 0);

        HECT_INFO("Created local host");
    }

    // Verify that the host was successfully created
    if (!_enet_host)
    {
        throw FatalError("Failed to create host");
    }
}

Host::~Host()
{
    // Destroy the associated host if there is one
    if (_enet_host)
    {
        if (_enet_host->address.port)
        {
            HECT_INFO(format("Destroyed local host listening on port %d", _enet_host->address.port));
        }
        else
        {
            HECT_INFO("Destroyed local host");
        }

        enet_host_destroy(_enet_host);
    }
}

Peer Host::request_connect_to(IPAddress address, Port port)
{
    // Create the ENet address to connect to
    ENetAddress enet_address;
    enet_address.host = static_cast<uint32_t>(address);
    enet_address.port = port;

    // Trigger the connection
    ENetPeer* enet_peer = enet_host_connect(_enet_host, &enet_address, _enet_host->channelLimit, 0);
    if (!enet_peer)
    {
        throw FatalError("Failed to create peer");
    }

    HECT_INFO(format("Requested connection to remote host at address %s on port %d", address.as_string().data(), port));

    // Return the associated peer
    return Peer(enet_peer);
}

void Host::request_disconnect_from(Peer peer)
{
    // Trigger the disconnect
    if (peer.state() == PeerState::Connected)
    {
        enet_peer_disconnect(peer._enet_peer, 0);
        HECT_INFO(format("Requested disconnection from remote host at address %s", peer.address().as_string().data()));
    }
    else
    {
        enet_peer_reset(peer._enet_peer);
    }
}

bool Host::poll_event(PeerEvent& event, Milliseconds time_out)
{
    // Poll for the ENet event
    ENetEvent enet_event;
    if (enet_host_service(_enet_host, &enet_event, static_cast<uint32_t>(time_out.value)) > 0)
    {
        Peer peer(enet_event.peer);

        // Create the event
        event = PeerEvent();
        event.type = static_cast<PeerEventType>(enet_event.type);
        event.peer = peer;

        // If it is a packet receive event then copy the packet data to the
        // packet of the event
        if (enet_event.type == ENET_EVENT_TYPE_RECEIVE)
        {
            ByteVector packet_data(enet_event.packet->dataLength, 0);
            std::memcpy(&packet_data[0], enet_event.packet->data, packet_data.size());
            event.packet_data = std::move(packet_data);
            enet_packet_destroy(enet_event.packet);
        }

        // Log connection/disconnection events
        switch (event.type)
        {
        case PeerEventType::Connect:
            HECT_INFO(format("Connected to remote host at address %s (peer id = %d)", peer.address().as_string().data(), peer.id()));
            break;
        case PeerEventType::Disconnect:
            HECT_INFO(format("Disconnected from remote host at address %s (peer id = %d)", peer.address().as_string().data(), peer.id()));
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

void Host::send_packet(Peer peer, Channel channel, const ByteVector& packet_data, PacketFlags packet_flags)
{
    ENetPacket* enet_packet = enet_packet_create(&packet_data[0], packet_data.size(), packet_flags);
    enet_peer_send(peer._enet_peer, channel, enet_packet);
}

void Host::broadcast_packet(Channel channel, const ByteVector& packet_data, PacketFlags packet_flags)
{
    ENetPacket* enet_packet = enet_packet_create(&packet_data[0], packet_data.size(), packet_flags);
    enet_host_broadcast(_enet_host, channel, enet_packet);
}

void Host::flush()
{
    enet_host_flush(_enet_host);
}

void Host::initialize_e_net()
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
