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
#include "Hect/Reflection/Enum.h"

namespace hect
{

///
/// Provides access for encoding structured data to a persistent format.
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

    ///
    /// Begins an array.
    virtual void beginArray() = 0;

    ///
    /// Ends the current array.
    virtual void endArray() = 0;

    ///
    /// Begins an object.
    virtual void beginObject() = 0;

    ///
    /// Ends the current object.
    virtual void endObject() = 0;

    ///
    /// Selects a member of the current object.
    ///
    /// \param name The name of the member to select.
    virtual void selectMember(const char* name) = 0;

    ///
    /// Encodes a string.
    ///
    /// \param value The value to encode.
    virtual void encodeString(const std::string& value) = 0;

    ///
    /// Encodes an 8-bit integer.
    ///
    /// \param value The value to encode.
    virtual void encodeInt8(int8_t value) = 0;

    ///
    /// Encodes an 8-bit unsigned integer.
    ///
    /// \param value The value to encode.
    virtual void encodeUInt8(uint8_t value) = 0;

    ///
    /// Encodes a 16-bit integer.
    ///
    /// \param value The value to encode.
    virtual void encodeInt16(int16_t value) = 0;

    ///
    /// Encodes a 16-bit unsigned integer.
    ///
    /// \param value The value to encode.
    virtual void encodeUInt16(uint16_t value) = 0;

    ///
    /// Encodes a 32-bit integer.
    ///
    /// \param value The value to encode.
    virtual void encodeInt32(int32_t value) = 0;

    ///
    /// Encodes a 32-bit unsigned integer.
    ///
    /// \param value The value to encode.
    virtual void encodeUInt32(uint32_t value) = 0;

    ///
    /// Encodes a 64-bit integer.
    ///
    /// \param value The value to encode.
    virtual void encodeInt64(int64_t value) = 0;

    ///
    /// Encodes a 64-bit unsigned integer.
    ///
    /// \param value The value to encode.
    virtual void encodeUInt64(uint64_t value) = 0;

    ///
    /// Encodes a 32-bit float.
    ///
    /// \param value The value to encode.
    virtual void encodeFloat32(float value) = 0;

    ///
    /// Encodes a 64-bit float.
    ///
    /// \param value The value to encode.
    virtual void encodeFloat64(double value) = 0;

    ///
    /// Encodes a boolean.
    ///
    /// \param value The value to encode.
    virtual void encodeBool(bool value) = 0;
};

Encoder& operator<<(Encoder& encoder, const BeginArray& beginArray);
Encoder& operator<<(Encoder& encoder, const EndArray&);

Encoder& operator<<(Encoder& encoder, const BeginObject& beginObject);
Encoder& operator<<(Encoder& encoder, const EndObject&);

template <typename T>
Encoder& operator<<(Encoder& encoder, const EncodeValue<T>& encodeValue);

template <typename T>
Encoder& operator<<(Encoder& encoder, const EncodeVector<T>& encodeVector);

template <typename T>
Encoder& operator<<(Encoder& encoder, const EncodeEnum<T>& encodeEnum);

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

}

#include "Encoder.inl"
