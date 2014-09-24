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

#include <stack>

#include "Hect/IO/Encoder.h"

namespace hect
{

///
/// Provides access for encoding structured data to a binary stream.
class BinaryEncoder :
    public Encoder
{
public:

    ///
    /// Constructs an encoder given the stream to encode to.
    ///
    /// \param stream The stream to encode to.
    BinaryEncoder(WriteStream& stream);

    bool isBinaryStream() const;

    WriteStream& binaryStream();

    Encoder& beginArray();
    Encoder& endArray();

    Encoder& beginObject();
    Encoder& endObject();

    Encoder& selectMember(const char* name);

    Encoder& encodeString(const std::string& value);
    Encoder& encodeInt8(int8_t value);
    Encoder& encodeUInt8(uint8_t value);
    Encoder& encodeInt16(int16_t value);
    Encoder& encodeUInt16(uint16_t value);
    Encoder& encodeInt32(int32_t value);
    Encoder& encodeUInt32(uint32_t value);
    Encoder& encodeInt64(int64_t value);
    Encoder& encodeUInt64(uint64_t value);
    Encoder& encodeFloat32(float value);
    Encoder& encodeFloat64(double value);
    Encoder& encodeBool(bool value);

private:
    std::stack<size_t> _countPositionStack;
    std::stack<unsigned> _countStack;
    std::stack<bool> _isObjectStack;

    WriteStream* _stream;
};

}