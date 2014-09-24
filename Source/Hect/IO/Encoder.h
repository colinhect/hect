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

#include "Hect/IO/EncodeOperations.h"
#include "Hect/IO/WriteStream.h"

namespace hect
{

class Encoder
{
public:

    ///
    /// Returns whether the encoder is writing to a binary stream.
    virtual bool isBinaryStream() const = 0;

    ///
    /// Returns the raw binary stream.
    ///
    /// \throws Error If the encoder is not writing to a binary stream.
    virtual WriteStream& binaryStream() = 0;

    virtual Encoder& beginArray() = 0;
    virtual Encoder& endArray() = 0;

    virtual Encoder& beginObject() = 0;
    virtual Encoder& endObject() = 0;

    virtual Encoder& selectMember(const char* name) = 0;

    virtual Encoder& encodeString(const std::string& value) = 0;
    virtual Encoder& encodeInt8(int8_t value) = 0;
    virtual Encoder& encodeUInt8(uint8_t value) = 0;
    virtual Encoder& encodeInt16(int16_t value) = 0;
    virtual Encoder& encodeUInt16(uint16_t value) = 0;
    virtual Encoder& encodeInt32(int32_t value) = 0;
    virtual Encoder& encodeUInt32(uint32_t value) = 0;
    virtual Encoder& encodeInt64(int64_t value) = 0;
    virtual Encoder& encodeUInt64(uint64_t value) = 0;
    virtual Encoder& encodeFloat32(float value) = 0;
    virtual Encoder& encodeFloat64(double value) = 0;
    virtual Encoder& encodeBool(bool value) = 0;

    template <typename T>
    Encoder& encodeEnum(T value);
};

Encoder& operator<<(Encoder& encoder, const BeginArray& beginArray);
Encoder& operator<<(Encoder& encoder, const EndArray&);

Encoder& operator<<(Encoder& encoder, const BeginObject& beginObject);
Encoder& operator<<(Encoder& encoder, const EndObject&);

Encoder& operator<<(Encoder& encoder, const char* value);
Encoder& operator<<(Encoder& encoder, const std::string& value);
Encoder& operator<<(Encoder& encoder, int8_t value);
Encoder& operator<<(Encoder& encoder, uint8_t value);
Encoder& operator<<(Encoder& encoder, int16_t value);
Encoder& operator<<(Encoder& encoder, uint16_t value);
Encoder& operator<<(Encoder& encoder, int32_t value);
Encoder& operator<<(Encoder& encoder, uint32_t value);
Encoder& operator<<(Encoder& encoder, int64_t value);
Encoder& operator<<(Encoder& encoder, uint64_t value);
Encoder& operator<<(Encoder& encoder, float value);
Encoder& operator<<(Encoder& encoder, double value);
Encoder& operator<<(Encoder& encoder, bool value);

template <typename T>
Encoder& operator<<(Encoder& encoder, const EncodeMember<T>& encodeMember);

}

#include "Encoder.inl"