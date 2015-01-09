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
#pragma once

#include "Hect/Core/Export.h"
#include "Hect/Core/Uncopyable.h"
#include "Hect/IO/ByteVector.h"
#include "Hect/Network/Peer.h"
#include "Hect/Timing/TimeSpan.h"

// Forward declarations for ENet types
typedef struct _ENetHost ENetHost;

namespace hect
{

///
/// An integer representing a channel that packets can be sent over to or from
/// a Peer.
typedef uint8_t Channel;

///
/// A flag describing how a packet is transported to a Peer.
enum PacketFlag
{
    ///
    /// Indicates no packet flags.
    PacketFlag_None = 0,

    ///
    /// Packet must be received by the target peer and resend attempts should
    /// be made until the packet is delivered.
    PacketFlag_Reliable = 1,

    ///
    /// Packet will not be sequenced with other packets.
    ///
    /// \warning Not supported for reliable packets.
    PacketFlag_Unsequenced = 2
};

///
/// A bit vector containing any number of PacketFlag%s.
typedef uint8_t PacketFlags;

///
/// A PeerEvent type.
enum PeerEventType
{
    ///
    /// No event occurred.
    PeerEventType_None = 0,

    ///
    /// A connection to a peer has been established.
    PeerEventType_Connect,

    ///
    /// A connection to a peer has been lost.
    PeerEventType_Disconnect,

    ///
    /// A packet has been received from a peer.
    PeerEventType_ReceivePacket
};

///
/// An event triggered from a Peer.
class HECT_EXPORT PeerEvent
{
public:

    ///
    /// The type of event.
    PeerEventType type { PeerEventType_None };

    ///
    /// The remote host triggering the event.
    Peer peer;

    ///
    /// The packet data received.
    ///
    /// \note Only valid for events of type PeerEventType_ReceivePacket.
    ByteVector packetData;
};

///
/// A local host for communicating to Peer%s.
class HECT_EXPORT Host :
    public Uncopyable
{
public:

    ///
    /// Constructs a host.
    ///
    /// \param maxPeerCount The maximum number of simultaneous peer
    /// connections.
    /// \param channelCount The number of channels to use; if 0 then the
    /// maximum number of channels are used.
    /// \param port The port to listen for incoming connections on; if 0 then
    /// incoming connections will be ignored.
    Host(size_t maxPeerCount, size_t channelCount = 0, Port port = 0);

    ~Host();

    ///
    /// Triggers a connection handshake attempt with a remote host.
    ///
    /// \note Although a peer is always returned successfully, the
    /// connection to that peer may or may not successfully establish.  If the
    /// connection succeeds then an event will be received and the peer's state
    /// will change to PeerState_Connected.
    ///
    /// \param address The address of the peer.
    /// \param port The port the remote host is listening on.
    ///
    /// \returns A peer representing the remote host the connection request
    /// was sent to.
    Peer requestConnectTo(IPAddress address, Port port);

    ///
    /// Triggers a disconnection handshake attempt with a remote host.
    ///
    /// \note If the disconnection succeeds then an event will be received and
    /// the peer's state will change to to PeerState_Disconnected.
    ///
    /// \param peer The peer representing the remove host to disconnect from.
    ///
    /// \throws InvalidOperation If the specified peer does not belong to this
    /// host.
    void requestDisconnectFrom(Peer peer);

    ///
    /// Polls the next event triggered from a Peer.
    ///
    /// \param event The event.
    /// \param timeOut The time span to wait for an event to occur.
    ///
    /// \returns True if an event was received; false otherwise.
    bool pollEvent(PeerEvent& event, TimeSpan timeOut = TimeSpan::fromMilliseconds(0));

    ///
    /// Sends a packet to a Peer.
    ///
    /// \note The packet is not sent instantaneously.  It will eventually
    /// occur during a call to pollEvent() or can be forced by calling flush().
    ///
    /// \param peer The peer to send the packet to.
    /// \param channel The channel to send the packet on.
    /// \param packetData The packet data to send.
    /// \param packetFlags The flags describing how the packet is transported.
    void sendPacket(Peer peer, Channel channel, const ByteVector& packetData, PacketFlags packetFlags = PacketFlag_Reliable);

    ///
    /// Broadcasts a packet to all connected Peer%s.
    ///
    /// \note The packet is not sent instantaneously.  It will eventually
    /// occur during a call to pollEvent() or can be forced by calling flush().
    ///
    /// \param channel The channel to send the packet on.
    /// \param packetData The packet data to send.
    /// \param packetFlags The flags describing how the packet is transported.
    void broadcastPacket(Channel channel, const ByteVector& packetData, PacketFlags packetFlags = PacketFlag_Reliable);

    ///
    /// Force any enqueued packet transmissions to occur.
    void flush();

private:
    static void initializeENet();

    ENetHost* _enetHost { nullptr };
};

}