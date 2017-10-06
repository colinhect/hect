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
    virtual ~Encoder() = default;

    ///
    /// Returns whether the encoder is writing to a binary stream.
    virtual bool is_binary_stream() const = 0;

    ///
    /// Returns the raw binary stream.
    ///
    /// \throws InvalidOperation If the encoder is not writing to a binary
    /// stream.
    virtual WriteStream& binary_stream() = 0;

    ///
    /// Begins an array.
    ///
    /// \note Values encoded following this call will be encoded as elements
    /// in the array.
    virtual void begin_array() = 0;

    ///
    /// Ends an array.
    ///
    /// \note No more elements can be encoded to be the array after it is
    /// ended.
    ///
    /// \throws InvalidOperation If an array was not started using
    /// begin_array().
    virtual void end_array() = 0;

    ///
    /// Begins an object.
    ///
    /// \note The specific members of an object can be targeted using
    /// select_member().
    virtual void begin_object() = 0;

    ///
    /// Ends an object.
    ///
    /// \throws InvalidOperation If an object was not started using
    /// begin_object().
    virtual void end_object() = 0;

    ///
    /// Selects a specific member of the object.
    ///
    /// \note The following call to encode a value (included beginning an
    /// array or object) will encode to the selected member of the object.
    ///
    /// \param name The name of the member to select.
    ///
    /// \throws InvalidOperation If an object was not started using
    /// begin_object().
    virtual void select_member(const char* name) = 0;

    ///
    /// Encodes a string.
    ///
    /// \param value The value to encode.
    virtual void encode_string(const std::string& value) = 0;

    ///
    /// Encodes an 8-bit signed integer.
    ///
    /// \param value The value to encode.
    virtual void encode_int8(int8_t value) = 0;

    ///
    /// Encodes an 8-bit unsigned integer.
    ///
    /// \param value The value to encode.
    virtual void encode_uint8(uint8_t value) = 0;

    ///
    /// Encodes a 16-bit signed integer.
    ///
    /// \param value The value to encode.
    virtual void encode_int16(int16_t value) = 0;

    ///
    /// Encodes a 16-bit unsigned integer.
    ///
    /// \param value The value to encode.
    virtual void encode_uint16(uint16_t value) = 0;

    ///
    /// Encodes a 32-bit signed integer.
    ///
    /// \param value The value to encode.
    virtual void encode_int32(int32_t value) = 0;

    ///
    /// Encodes a 32-bit unsigned integer.
    ///
    /// \param value The value to encode.
    virtual void encode_uint32(uint32_t value) = 0;

    ///
    /// Encodes a 64-bit signed integer.
    ///
    /// \param value The value to encode.
    virtual void encode_int64(int64_t value) = 0;

    ///
    /// Encodes a 64-bit unsigned integer.
    ///
    /// \param value The value to encode.
    virtual void encode_uint64(uint64_t value) = 0;

    ///
    /// Encodes a 32-bit float.
    ///
    /// \param value The value to encode.
    virtual void encode_float32(float value) = 0;

    ///
    /// Encodes a 64-bit float.
    ///
    /// \param value The value to encode.
    virtual void encode_float64(double value) = 0;

    ///
    /// Encodes a boolean.
    ///
    /// \param value The value to encode.
    virtual void encode_bool(bool value) = 0;
};

HECT_EXPORT Encoder& operator<<(Encoder& encoder, const BeginArray& begin_array);
HECT_EXPORT Encoder& operator<<(Encoder& encoder, const EndArray&);

HECT_EXPORT Encoder& operator<<(Encoder& encoder, const BeginObject& begin_object);
HECT_EXPORT Encoder& operator<<(Encoder& encoder, const EndObject&);

template <typename Type>
Encoder& operator<<(Encoder& encoder, const EncodeValue<Type>& encode_value);

template <typename Type>
Encoder& operator<<(Encoder& encoder, const EncodeVector<Type>& encode_vector);

template <typename Type>
Encoder& operator<<(Encoder& encoder, const EncodeEnum<Type>& encode_enum);

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
