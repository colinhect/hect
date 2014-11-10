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
#include "UdpPeer.h"

#include <enet/enet.h>

#include "Hect/Core/Error.h"

using namespace hect;

UdpPeer::Handle::Handle()
{
}

UdpPeer& UdpPeer::Handle::operator*() const
{
    auto peer = _peer.lock();

    if (!peer)
    {
        throw Error("Invalid peer handle");
    }

    return *peer;
}

UdpPeer* UdpPeer::Handle::operator->() const
{
    auto peer = _peer.lock();

    if (!peer)
    {
        throw Error("Invalid peer handle");
    }

    return peer.get();
}

UdpPeer::Handle::operator bool() const
{
    auto peer = _peer.lock();
    return peer.get() != nullptr;
}

UdpPeer::Handle::Handle(const std::weak_ptr<UdpPeer>& peer) :
    _peer(peer)
{
}

UdpPeerId UdpPeer::id() const
{
    if (!_enetPeer)
    {
        return 0;
    }

    return _enetPeer->incomingPeerID;
}

IPAddress UdpPeer::address() const
{
    if (!_enetPeer)
    {
        return IPAddress(0);
    }

    return IPAddress(_enetPeer->address.host);
}

UdpPeerState UdpPeer::state() const
{
    if (!_enetPeer)
    {
        return UdpPeerState_Disconnected;
    }

    return static_cast<UdpPeerState>(_enetPeer->state);
}

bool UdpPeer::operator==(const UdpPeer& peer) const
{
    return _enetPeer == peer._enetPeer;
}

UdpPeer::UdpPeer(ENetPeer* enetPeer) :
    _enetPeer(enetPeer)
{
}