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

#include "Hect/Core/Export.h"
#include "Hect/Network/IPAddress.h"

// Forward declarations for ENet types
typedef struct _ENetPeer ENetPeer;

namespace hect
{

///
/// A state that a peer is in.
enum PeerState
{
    ///
    /// Not connected.
    PeerState_Disconnected,

    ///
    /// Connection in progress.
    PeerState_Connecting,

    ///
    /// Acknowledgement of connection received.
    PeerState_AcknowledgingConnect,

    ///
    /// Connection pending.
    PeerState_ConnectionPending,

    ///
    /// Connection succeeded.
    PeerState_ConnectionSucceeded,

    ///
    /// Connection fully established.
    PeerState_Connected,

    ///
    /// Disconnection will triggered.
    PeerState_DisconnectLater,

    ///
    /// Disconnection in progress.
    PeerState_Disconnecting,

    ///
    /// Acknowledgement of disconnection received.
    PeerState_AcknowledgeDisconnect,

    ///
    /// Unknown state.
    PeerState_Unknown
};

///
/// A locally unique id for a peer.
typedef uint16_t PeerId;

///
/// A handle to a remote host.
///
/// \note A peer is a lightweight handle without any associated resources.
/// A copied peer will refer to the same peer as it was copied from.
class HECT_EXPORT Peer
{
    friend class Host;
public:
    Peer();

    ///
    /// Returns the locally unique id of the peer.
    PeerId id() const;

    ///
    /// Returns the remote address of the peer.
    IPAddress address() const;

    ///
    /// Returns the current state of the peer.
    PeerState state() const;

    ///
    /// Returns whether the peer is the same as another.
    ///
    /// \param peer The other peer.
    bool operator==(const Peer& peer) const;

    ///
    /// Returns whether the peer is different from another.
    ///
    /// \param peer The other peer.
    bool operator!=(const Peer& peer) const;

private:
    Peer(ENetPeer* enetPeer);

    ENetPeer* _enetPeer { nullptr };
};

}