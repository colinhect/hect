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

namespace hect
{

///
/// Provides functionality to read from a packet.
typedef MemoryReadStream PacketReadStream;

///
/// Provides functionality to write to a packet.
typedef MemoryWriteStream PacketWriteStream;

///
/// A flag describing how a packet is transported.
enum PacketFlag
{
    ///
    /// Packet must be received by the target peer and resend attempts
    /// should be made until the packet is delivered.
    Reliable = 1,

    ///
    /// Packet will not be sequenced with other packets.
    ///
    /// \warning Not supported for reliable packets.
    Unsequenced = 2
};

///
/// A packet of data to be transported across a network connection.
class Packet
{
    friend class Socket;
public:

    ///
    /// Constructs a packet given its flags.
    ///
    /// \param flags The flags describing how the packet is transported.
    Packet(uint8_t flags = 0);

    ///
    /// Returns a read stream for the packet data.
    PacketReadStream readStream() const;

    ///
    /// Returns a write stream for the packet data.
    PacketWriteStream writeStream();

private:
    Packet(const std::vector<uint8_t>& data);

    uint8_t _flags;
    std::vector<uint8_t> _data;
};

}