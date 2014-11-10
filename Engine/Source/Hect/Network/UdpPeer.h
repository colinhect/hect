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

#include <memory>

#include "Hect/Core/Uncopyable.h"
#include "Hect/Network/IPAddress.h"

// Forward declarations for ENet types
typedef struct _ENetPeer ENetPeer;

namespace hect
{

///
/// A state that a peer is in.
enum UdpPeerState
{
    ///
    /// Not connected.
    UdpPeerState_Disconnected,

    ///
    /// Connection in progress.
    UdpPeerState_Connecting,

    ///
    /// Acknowledgement of connection received.
    UdpPeerState_AcknowledgingConnect,

    ///
    /// Connection pending.
    UdpPeerState_ConnectionPending,

    ///
    /// Connection succeeded.
    UdpPeerState_ConnectionSucceeded,

    ///
    /// Connection fully established.
    UdpPeerState_Connected,

    ///
    /// Disconnection will triggered.
    UdpPeerState_DisconnectLater,

    ///
    /// Disconnection in progress.
    UdpPeerState_Disconnecting,

    ///
    /// Acknowledgement of disconnection received.
    UdpPeerState_AcknowledgeDisconnect,

    ///
    /// Unknown state.
    UdpPeerState_Unknown
};


///
/// A locally unique id for a peer.
typedef uint16_t UdpPeerId;

///
/// A remote point of contact for remote communication over UDP.
class UdpPeer :
    public Uncopyable
{
    friend class UdpSocket;
public:

    ///
    /// A handle for a peer;
    class Handle
    {
        friend class UdpSocket;
    public:

        ///
        /// Creates an invalid peer handle.
        Handle();

        ///
        /// Dereferences the handle to a reference to the associated peer.
        ///
        /// \returns A reference to the associated peer.
        ///
        /// \throws Error If the handle is invalid.
        UdpPeer& operator*() const;

        ///
        /// Dereferences the handle to a pointer to the associated peer.
        ///
        /// \returns A pointer to the associated peer.
        ///
        /// \throws Error If the handle is invalid.
        UdpPeer* operator->() const;

        ///
        /// Returns whether the handle is valid.
        operator bool() const;

    private:
        Handle(const std::weak_ptr<UdpPeer>& peer);

        std::weak_ptr<UdpPeer> _peer;
    };

    ///
    /// Returns the locally unique id of the peer.
    UdpPeerId id() const;

    ///
    /// Returns the remote address of the peer.
    IPAddress address() const;

    ///
    /// Returns the current state of the peer.
    UdpPeerState state() const;

    ///
    /// Returns whether the peer the is same as another.
    ///
    /// \param peer The other peer.
    bool operator==(const UdpPeer& peer) const;

private:
    UdpPeer(ENetPeer* enetPeer);

    ENetPeer* _enetPeer{ nullptr };
};

}