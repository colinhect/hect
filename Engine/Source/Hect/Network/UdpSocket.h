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
#pragma once

#include "Hect/Core/Uncopyable.h"
#include "Hect/Timing/TimeSpan.h"
#include "Hect/Network/UdpPacket.h"
#include "Hect/Network/UdpPeer.h"

// Forward declarations for ENet types
typedef struct _ENetHost ENetHost;

namespace hect
{

///
/// An Internet Protocol port.
typedef uint16_t Port;

///
/// A channel of a UDP socket.
typedef uint8_t Channel;

///
/// An event type.
enum UdpSocketEventType
{
    ///
    /// No event occurred.
    UdpSocketEventType_None = 0,

    ///
    /// A connection to a peer has been established.
    UdpSocketEventType_Connect,

    ///
    /// A connection to a peer has been lost.
    UdpSocketEventType_Disconnect,

    ///
    /// A packet has been received from a peer.
    UdpSocketEventType_Receive
};

///
/// An event triggered from a remote socket.
class UdpSocketEvent
{
public:

    ///
    /// The event type.
    UdpSocketEventType type { UdpSocketEventType_None };

    ///
    /// The remote socket triggering the event.
    UdpPeer::Handle peer;

    ///
    /// The packet received (only for events with type
    /// UdpSocket::Event::Receive).
    UdpPacket packet;
};

///
/// A local point of contact for remote communication over UDP.
///
/// \note A socket can either listen for incoming connections or attempt
/// to connect to a remote socket which is listening.
class UdpSocket :
    public Uncopyable
{
public:

    ///
    /// Constructs a socket which does not listen for incoming connections.
    ///
    /// \param maxConnectionCount The maximum number of simultaneous outgoing
    /// connections the socket can create.
    /// \param channelCount The number of channels to use.
    UdpSocket(unsigned maxConnectionCount, uint8_t channelCount);

    ///
    /// Constructs a socket which listens for incoming connections on a given
    /// port.
    ///
    /// \param port The port to listen on.
    /// \param maxConnectionCount The maximum number of simultaneous incoming
    /// connections the socket can accept.
    /// \param channelCount The number of channels to use.
    UdpSocket(Port port, unsigned maxConnectionCount, uint8_t channelCount);

    ///
    /// Destroys the socket.
    ~UdpSocket();

    ///
    /// Triggers a connection handshake attempt with a remote socket.
    ///
    /// \note Although a peer is always returned successfully, the
    /// connection to that peer may or may not successfully establish.  If the
    /// connection succeeds then an event will be received and the peer's state
    /// will change to PeerState_Connected.
    ///
    /// \param address The address of the remote socket.
    /// \param port The port the remote socket is listening on.
    UdpPeer::Handle connectToPeer(IPAddress address, Port port);

    ///
    /// Triggers a disconnection handshake attempt with a remote socket.
    ///
    /// \note If a connection to the peer is not established then nothing
    /// happens.
    ///
    /// \param peer The peer.
    ///
    /// \throws Error If the specified peer does not belong to this socket.
    void disconnectFromPeer(UdpPeer& peer);

    ///
    /// Polls the next event triggered from a remote socket.
    ///
    /// \param event The event.
    /// \param timeOut The time span to wait for an event to occur.
    ///
    /// \returns True if an event was received; false otherwise.
    bool pollEvent(UdpSocketEvent& event, TimeSpan timeOut = TimeSpan::fromMilliseconds(0));

    ///
    /// Sends a packet to a remote socket.
    ///
    /// \note The packet is not sent instantaneously.  It will eventually
    /// occur during a call to pollEvent() or can be forced by calling flush().
    ///
    /// \param peer The peer to receive the packet.
    /// \param channel The channel to send the packet on.
    /// \param packet The packet to send.
    void sendPacket(UdpPeer& peer, Channel channel, const UdpPacket& packet);

    ///
    /// Broadcasts a packet to all connected remote sockets.
    ///
    /// \note The packet is not sent instantaneously.  It will eventually
    /// occur during a call to pollEvent() or can be forced by calling flush().
    ///
    /// \param channel The channel to send the packet on.
    /// \param packet The packet to send.
    void broadcastPacket(Channel channel, const UdpPacket& packet);

    ///
    /// Force any enqueued packet transmissions to occur.
    void flush();

private:
    ENetHost* _enetHost { nullptr };
    std::vector<std::shared_ptr<UdpPeer>> _peers;
};

}