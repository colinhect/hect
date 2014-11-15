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

#include <mutex>

#include "Hect/Core/Uncopyable.h"
#include "Hect/Timing/TimeSpan.h"
#include "Hect/Network/Packet.h"
#include "Hect/Network/Peer.h"

// Forward declarations for ENet types
typedef struct _ENetHost ENetHost;

namespace hect
{

///
/// A channel of a UDP socket.
typedef uint8_t Channel;

///
/// A socket event type.
enum SocketEventType
{
    ///
    /// No event occurred.
    SocketEventType_None = 0,

    ///
    /// A connection to a peer has been established.
    SocketEventType_Connect,

    ///
    /// A connection to a peer has been lost.
    SocketEventType_Disconnect,

    ///
    /// A packet has been received from a peer.
    SocketEventType_Receive
};

///
/// An event triggered from a remote socket.
class SocketEvent
{
    friend class Socket;
public:
    ///
    /// The event type.
    SocketEventType type { SocketEventType_None };

    ///
    /// The packet received (only for events with type
    /// Socket::Event::Receive).
    Packet packet;

    ///
    /// The remote socket triggering the event.
    Peer peer;
};

///
/// A local point of contact for remote communication over UDP.
///
/// \note A socket can either listen for incoming connections or attempt
/// to connect to a remote socket which is listening.
class Socket :
    public Uncopyable
{
public:

    ///
    /// Constructs a socket.
    Socket();

    ///
    /// Constructs a socket.
    ///
    /// \param peerCount The maximum number of simultaneous connections the
    /// socket can have.
    /// \param channelCount The number of channels to use.
    Socket(unsigned peerCount, uint8_t channelCount);

    ///
    /// Destroys the socket.
    ~Socket();

    ///
    /// Begins listening for incoming connections on a given port.
    ///
    /// \param port The port to listen on.
    ///
    /// \throws Error If the socket is already listening for connections or has
    /// any active connections to a remote socket.
    void listenOnPort(Port port);

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
    Peer requestConnectTo(IPAddress address, Port port);

    ///
    /// Triggers a disconnection handshake attempt with a remote socket.
    ///
    /// \note If a connection to the peer is not established then nothing
    /// happens.
    ///
    /// \param peer The peer.
    ///
    /// \throws Error If the specified peer does not belong to this socket.
    void requestDisconnectFrom(Peer peer);

    ///
    /// Polls the next event triggered from a remote socket.
    ///
    /// \param event The event.
    /// \param timeOut The time span to wait for an event to occur.
    ///
    /// \returns True if an event was received; false otherwise.
    bool pollEvent(SocketEvent& event, TimeSpan timeOut = TimeSpan::fromMilliseconds(0));

    ///
    /// Sends a packet to a remote socket.
    ///
    /// \note The packet is not sent instantaneously.  It will eventually
    /// occur during a call to pollEvent() or can be forced by calling flush().
    ///
    /// \param peer The peer to receive the packet.
    /// \param channel The channel to send the packet on.
    /// \param packet The packet to send.
    void sendPacket(Peer peer, Channel channel, const Packet& packet);

    ///
    /// Broadcasts a packet to all connected remote sockets.
    ///
    /// \note The packet is not sent instantaneously.  It will eventually
    /// occur during a call to pollEvent() or can be forced by calling flush().
    ///
    /// \param channel The channel to send the packet on.
    /// \param packet The packet to send.
    void broadcastPacket(Channel channel, const Packet& packet);

    ///
    /// Force any enqueued packet transmissions to occur.
    void flush();

private:
    static void initializeENet();
    static void deinitializeENet();

    static int enetInitializationCounter;
    static std::mutex enetInitializationMutex;

    unsigned _peerCount { 32 };
    uint8_t _channelCount { 1 };
    bool _listening { false };
    ENetHost* _enetHost { nullptr };
};

}