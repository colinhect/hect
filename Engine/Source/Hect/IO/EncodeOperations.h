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

#include <vector>

#include "Hect/Core/Export.h"

namespace hect
{

///
/// An operation for beginning an array.
struct HECT_EXPORT BeginArray
{
    ///
    /// Begins an unnamed array.
    BeginArray();

    ///
    /// Begins a named array.
    ///
    /// \param name The name of the array.
    BeginArray(const char* name);

    const char* name
    {
        nullptr
    };
};

///
/// An operation for ending an array.
struct HECT_EXPORT EndArray
{
};

///
/// An operation for beginning an object.
struct HECT_EXPORT BeginObject
{

    ///
    /// Begins an unnamed object.
    BeginObject();

    ///
    /// Begins a named object.
    ///
    /// \param name The name of the object.
    BeginObject(const char* name);

    const char* name
    {
        nullptr
    };
};

///
/// An operation for ending an object.
struct HECT_EXPORT EndObject
{
};

///
/// An operation for encoding a value.
template <typename T>
struct EncodeValue
{
    ///
    /// Encodes an unnamed value.
    ///
    /// \param value The value to encode.
    EncodeValue(const T& value);

    ///
    /// Encodes a named value.
    ///
    /// \param name The name of the value.
    /// \param value The value to encode.
    EncodeValue(const char* name, const T& value);

    const char* name
    {
        nullptr
    };
    const T& value;
};

///
/// An operation for encoding a vector as an array.
template <typename T>
struct EncodeVector
{
    ///
    /// Encodes an unnamed array from a vector.
    ///
    /// \param values The values to encode.
    EncodeVector(const std::vector<T>& values);

    ///
    /// Encodes a named array from a vector.
    ///
    /// \param name The name of the array.
    /// \param values The values to encode.
    EncodeVector(const char* name, const std::vector<T>& values);

    const char* name
    {
        nullptr
    };
    const std::vector<T>& values;
};

///
/// An operation for encoding an enum.
template <typename T>
struct EncodeEnum
{
    ///
    /// Encodes an unnamed value from an enum.
    ///
    /// \param value The value to encode.
    EncodeEnum(T value);

    ///
    /// Encodes a named value from an enum.
    ///
    /// \param name The name of the value.
    /// \param value The value to encode.
    EncodeEnum(const char* name, T value);

    const char* name
    {
        nullptr
    };
    T value;
};

///
/// An operation for decoding a value.
template <typename T>
struct DecodeValue
{
    ///
    /// Decodes an unnamed value.
    ///
    /// \param value The value to decode.
    DecodeValue(T& value);

    ///
    /// Decodes a named value.
    ///
    /// \param name The name of the value.
    /// \param value The value to decode.
    /// \param required Whether the decode should fail if a value of the
    /// specified name does not exist.
    DecodeValue(const char* name, T& value, bool required);

    const char* name
    {
        nullptr
    };
    T& value;
    const bool required
    {
        false
    };
};

///
/// An operation for decoding an array as a vector.
template <typename T>
struct DecodeVector
{
    ///
    /// Decodes an unnamed array to a vector.
    ///
    /// \param values The values to decode.
    DecodeVector(std::vector<T>& values);

    ///
    /// Decodes a named array to a vector.
    ///
    /// \param name The name of the array.
    /// \param values The values to decode.
    /// \param required Whether the decode should fail if a value of the
    /// specified name does not exist.
    DecodeVector(const char* name, std::vector<T>& values, bool required);

    const char* name
    {
        nullptr
    };
    std::vector<T>& values;
    const bool required
    {
        false
    };
};

///
/// An operation for decoding an enum.
template <typename T>
struct DecodeEnum
{
    ///
    /// Decodes an unnamed value to an enum.
    ///
    /// \param value The value to decode.
    DecodeEnum(T& value);

    ///
    /// Decodes a named value to an enum.
    ///
    /// \param name The name of the value.
    /// \param value The value to decode.
    /// \param required Whether the decode should fail if a value of the
    /// specified name does not exist.
    DecodeEnum(const char* name, T& value, bool required);

    const char* name
    {
        nullptr
    };
    T& value;
    const bool required
    {
        false
    };
};

///
/// Creates an operation for beginning an array.
HECT_EXPORT BeginArray beginArray();

///
/// Creates an operation for beginning an array as a member of the current
/// object.
///
/// \param name The name of the member to begin the array for.
HECT_EXPORT BeginArray beginArray(const char* name);

///
/// Creates an operation for ending the current array.
HECT_EXPORT EndArray endArray();

///
/// Creates an operation for beginning an object.
HECT_EXPORT BeginObject beginObject();

///
/// Creates an operation for beginning an object as a member of the current
/// object.
///
/// \param name The name of the member to begin the object for.
HECT_EXPORT BeginObject beginObject(const char* name);

///
/// Creates an operation for ending the current object.
HECT_EXPORT EndObject endObject();

///
/// Creates an operation for encoding an arbitrary value.
///
/// \param value The value to encode.
template <typename T>
EncodeValue<T> encodeValue(const T& value);

///
/// Creates an operation for encoding an arbitrary value as a member of the
/// current object.
///
/// \param name The name of the member to encode the value for.
/// \param value The value to encode.
template <typename T>
EncodeValue<T> encodeValue(const char* name, const T& value);

///
/// Creates an operation for encoding a vector of values.
///
/// \param values The values to encode.
template <typename T>
EncodeVector<T> encodeVector(const std::vector<T>& values);

///
/// Creates an operation for encoding a vector of values as a member of the
/// current object.
///
/// \param name The name of the member to encode the vector for.
/// \param values The values to encode.
template <typename T>
EncodeVector<T> encodeVector(const char* name, const std::vector<T>& values);

///
/// Creates an operation for encoding an enum value.
///
/// \param value The value to encode.
template <typename T>
EncodeEnum<T> encodeEnum(T value);

///
/// Creates an operation for encoding an enum value as a member of the current
/// object.
///
/// \param name The name of the member to encode the value for.
/// \param value The value to encode.
template <typename T>
EncodeEnum<T> encodeEnum(const char* name, T value);

///
/// Creates an operation for decoding an arbitrary value.
///
/// \param value The value to decode.
template <typename T>
DecodeValue<T> decodeValue(T& value);

///
/// Creates an operation for decoding an arbitrary value as a member of the
/// current object.
///
/// \param name The name of the member to decode the value for.
/// \param value The value to decode.
/// \param required Whether the decode should fail if a value of the specified
/// name does not exist.
template <typename T>
DecodeValue<T> decodeValue(const char* name, T& value, bool required = false);

///
/// Creates an operation for decoding a vector of values.
///
/// \param values The values to decode.
template <typename T>
DecodeVector<T> decodeVector(std::vector<T>& values);

///
/// Creates an operation for decoding a vector of values as a member of the
/// current object.
///
/// \param name The name of the member to decode the vector for.
/// \param values The values to decode.
/// \param required Whether the decode should fail if a value of the specified
/// name does not exist.
template <typename T>
DecodeVector<T> decodeVector(const char* name, std::vector<T>& values, bool required = false);

///
/// Creates an operation for decoding an enum value.
///
/// \param value The value to decode.
template <typename T>
DecodeEnum<T> decodeEnum(T& value);

///
/// Creates an operation for decoding an enum value as a member of the current
/// object.
///
/// \param name The name of the member to decode the value for.
/// \param value The value to decode.
/// \param required Whether the decode should fail if a value of the specified
/// name does not exist.
template <typename T>
DecodeEnum<T> decodeEnum(const char* name, T& value, bool required = false);

}

#include "EncodeOperations.inl"