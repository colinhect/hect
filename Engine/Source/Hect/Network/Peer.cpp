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
#include "Peer.h"

#include <enet/enet.h>

#include "Hect/Core/Exception.h"

using namespace hect;

Peer::Peer()
{
}

PeerId Peer::id() const
{
    PeerId id = PeerId(-1);

    if (_enetPeer)
    {
        id = _enetPeer->incomingPeerID;
    }

    return id;
}

IPAddress Peer::address() const
{
    IPAddress address;

    if (_enetPeer)
    {
        address = IPAddress(_enetPeer->address.host);
    }

    return address;
}

PeerState Peer::state() const
{
    PeerState state = PeerState::Disconnected;

    if (_enetPeer)
    {
        state = static_cast<PeerState>(_enetPeer->state);
    }

    return state;
}

bool Peer::operator==(const Peer& peer) const
{
    return _enetPeer == peer._enetPeer;
}

bool Peer::operator!=(const Peer& peer) const
{
    return _enetPeer != peer._enetPeer;
}

Peer::Peer(ENetPeer* enetPeer) :
    _enetPeer(enetPeer)
{
}