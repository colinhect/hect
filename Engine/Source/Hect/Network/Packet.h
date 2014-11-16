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
#include <vector>

#include "Hect/IO/ByteVector.h"

namespace hect
{

///
/// A flag describing how a packet is transported.
enum PacketFlag
{
    ///
    /// Indicates no packet flags.
    PacketFlag_None = 0,

    ///
    /// Packet must be received by the target peer and resend attempts
    /// should be made until the packet is delivered.
    PacketFlag_Reliable = 1,

    ///
    /// Packet will not be sequenced with other packets.
    ///
    /// \warning Not supported for reliable packets.
    PacketFlag_Unsequenced = 2
};

///
/// A bit vector containing any number of packet flags.
typedef uint8_t PacketFlags;

///
/// A packet of data to be transported across a network connection.
class Packet
{
public:

    ///
    /// Constructs a packet with default flags and no data.
    Packet();

    ///
    /// Constructs a packet given its flags.
    ///
    /// \param flags The flags describing how the packet is transported.
    Packet(PacketFlags flags);

    ///
    /// Constructs a packet given its data and flags.
    ///
    /// \param data The data to transport.
    /// \param flags The flags describing how the packet is transported.
    Packet(const ByteVector& data, PacketFlags flags);

    ///
    /// Returns the packet data.
    ByteVector& data();

    ///
    /// Returns the packet data.
    const ByteVector& data() const;

    ///
    /// Returns the packet flags.
    PacketFlags flags() const;

    ///
    /// Returns whether the given packet flag is enabled.
    ///
    /// \param flag The flag.
    bool hasFlag(PacketFlag flag) const;

private:
    ByteVector _data;
    PacketFlags _flags { 0 };
};

}