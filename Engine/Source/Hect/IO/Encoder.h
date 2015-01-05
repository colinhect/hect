///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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
#include "Hect/IO/EncodeError.h"
#include "Hect/IO/EncodeOperations.h"
#include "Hect/IO/WriteStream.h"
#include "Hect/Reflection/Enum.h"

namespace hect
{

///
/// Provides access for encoding structured data to a persistent format.
class HECT_EXPORT Encoder
{
public:

    ///
    /// Returns whether the encoder is writing to a binary stream.
    virtual bool isBinaryStream() const = 0;

    ///
    /// Returns the raw binary stream.
    ///
    /// \throws InvalidOperation If the encoder is not writing to a binary
    /// stream.
    virtual WriteStream& binaryStream() = 0;

    ///
    /// Begins an array.
    ///
    /// \note Values encoded following this call will be encoded as elements
    /// in the array.
    virtual void beginArray() = 0;

    ///
    /// Ends an array.
    ///
    /// \note No more elements can be encoded to be the array after it is
    /// ended.
    ///
    /// \throws InvalidOperation If an array was not started using
    /// beginArray().
    virtual void endArray() = 0;

    ///
    /// Begins an object.
    ///
    /// \note The specific members of an object can be targeted using
    /// selectMember().
    virtual void beginObject() = 0;

    ///
    /// Ends an object.
    ///
    /// \throws InvalidOperation If an object was not started using
    /// beginObject().
    virtual void endObject() = 0;

    ///
    /// Selects a specific member of the object.
    ///
    /// \note The following call to encode a value (included beginning an
    /// array or object) will encode to the selected member of the object.
    ///
    /// \param name The name of the member to select.
    ///
    /// \throws InvalidOperation If an object was not started using
    /// beginObject().
    virtual void selectMember(const char* name) = 0;

    ///
    /// Encodes a string.
    ///
    /// \param value The value to encode.
    virtual void encodeString(const std::string& value) = 0;

    ///
    /// Encodes an 8-bit signed integer.
    ///
    /// \param value The value to encode.
    virtual void encodeInt8(int8_t value) = 0;

    ///
    /// Encodes an 8-bit unsigned integer.
    ///
    /// \param value The value to encode.
    virtual void encodeUInt8(uint8_t value) = 0;

    ///
    /// Encodes a 16-bit signed integer.
    ///
    /// \param value The value to encode.
    virtual void encodeInt16(int16_t value) = 0;

    ///
    /// Encodes a 16-bit unsigned integer.
    ///
    /// \param value The value to encode.
    virtual void encodeUInt16(uint16_t value) = 0;

    ///
    /// Encodes a 32-bit signed integer.
    ///
    /// \param value The value to encode.
    virtual void encodeInt32(int32_t value) = 0;

    ///
    /// Encodes a 32-bit unsigned integer.
    ///
    /// \param value The value to encode.
    virtual void encodeUInt32(uint32_t value) = 0;

    ///
    /// Encodes a 64-bit signed integer.
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

HECT_EXPORT Encoder& operator<<(Encoder& encoder, const BeginArray& beginArray);
HECT_EXPORT Encoder& operator<<(Encoder& encoder, const EndArray&);

HECT_EXPORT Encoder& operator<<(Encoder& encoder, const BeginObject& beginObject);
HECT_EXPORT Encoder& operator<<(Encoder& encoder, const EndObject&);

template <typename T>
Encoder& operator<<(Encoder& encoder, const EncodeValue<T>& encodeValue);

template <typename T>
Encoder& operator<<(Encoder& encoder, const EncodeVector<T>& encodeVector);

template <typename T>
Encoder& operator<<(Encoder& encoder, const EncodeEnum<T>& encodeEnum);

HECT_EXPORT Encoder& operator<<(Encoder& encoder, const char* value);
HECT_EXPORT Encoder& operator<<(Encoder& encoder, const std::string& value);
HECT_EXPORT Encoder& operator<<(Encoder& encoder, int8_t value);
HECT_EXPORT Encoder& operator<<(Encoder& encoder, uint8_t value);
HECT_EXPORT Encoder& operator<<(Encoder& encoder, int16_t value);
HECT_EXPORT Encoder& operator<<(Encoder& encoder, uint16_t value);
HECT_EXPORT Encoder& operator<<(Encoder& encoder, int32_t value);
HECT_EXPORT Encoder& operator<<(Encoder& encoder, uint32_t value);
HECT_EXPORT Encoder& operator<<(Encoder& encoder, int64_t value);
HECT_EXPORT Encoder& operator<<(Encoder& encoder, uint64_t value);
HECT_EXPORT Encoder& operator<<(Encoder& encoder, float value);
HECT_EXPORT Encoder& operator<<(Encoder& encoder, double value);
HECT_EXPORT Encoder& operator<<(Encoder& encoder, bool value);

}

#include "Encoder.inl"
