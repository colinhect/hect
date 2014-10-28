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

#include <vector>

#include "Hect/Core/Uncopyable.h"

namespace hect
{

///
/// An encode operation for beginning an array.
struct BeginArray
{
    BeginArray();
    BeginArray(const char* name);

    const char* name;
};

///
/// An encode operation for ending an array.
struct EndArray
{
};

///
/// An encode operation for beginning an object.
struct BeginObject
{
    BeginObject();
    BeginObject(const char* name);

    const char* name;
};

///
/// An encode operation for ending an object.
struct EndObject
{
};

///
/// An encode operation for encoding a value.
template <typename T>
struct EncodeValue :
        public Uncopyable
{
    EncodeValue(const T& value);
    EncodeValue(const char* name, const T& value);

    const char* name;
    const T& value;
};

template <typename T>
struct EncodeVector :
        public Uncopyable
{
    EncodeVector(const std::vector<T>& values);
    EncodeVector(const char* name, const std::vector<T>& values);

    const char* name;
    const std::vector<T>& values;
};

///
/// An decode operation for encoding a value.
template <typename T>
struct DecodeValue :
        public Uncopyable
{
    DecodeValue(T& value);
    DecodeValue(const char* name, T& value, bool required);

    const char* name;
    T& value;
    const bool required;
};

template <typename T>
struct DecodeVector :
        public Uncopyable
{
    DecodeVector(std::vector<T>& values);
    DecodeVector(const char* name, std::vector<T>& values);

    const char* name;
    std::vector<T>& values;
};

///
/// An encode operation for encoding an enum.
template <typename T>
struct EncodeEnum :
        public Uncopyable
{
    EncodeEnum(T value);
    EncodeEnum(const char* name, T value);

    const char* name;
    T value;
};

///
/// An encode operation for encoding an enum.
template <typename T>
struct DecodeEnum :
        public Uncopyable
{
    DecodeEnum(T& value);
    DecodeEnum(const char* name, T& value);

    const char* name;
    T& value;
};

///
/// Returns an encode operation for beginning an array.
BeginArray beginArray();

///
/// Returns an encode operation for beginning an array as a member of the
/// current object.
///
/// \param name The name of the member to begin the array for.
BeginArray beginArray(const char* name);

///
/// Returns an encode operation for ending the current array.
EndArray endArray();

///
/// Returns an encode operation for beginning an object.
BeginObject beginObject();

///
/// Returns an encode operation for beginning an object as a member of the
/// current object.
///
/// \param name The name of the member to begin the object for.
BeginObject beginObject(const char* name);

///
/// Returns an encode operation for ending the current object.
EndObject endObject();

///
/// Returns an encode operation for encoding an arbitrary value.
///
/// \param value The value to encode.
template <typename T>
EncodeValue<T> encodeValue(const T& value);

///
/// Returns an encode operation for encoding an arbitrary value as a member of
/// the current object.
///
/// \param name The name of the member to encode the value for.
/// \param value The value to encode.
template <typename T>
EncodeValue<T> encodeValue(const char* name, const T& value);

template <typename T>
EncodeVector<T> encodeVector(const std::vector<T>& values);

template <typename T>
EncodeVector<T> encodeVector(const char* name, const std::vector<T>& values);

template <typename T>
DecodeValue<T> decodeValue(T& value);

template <typename T>
DecodeValue<T> decodeValue(const char* name, T& value, bool required = false);

template <typename T>
DecodeVector<T> decodeVector(std::vector<T>& values);

template <typename T>
DecodeVector<T> decodeVector(const char* name, std::vector<T>& values);

///
/// Returns an encode operation for encoding an enum value.
///
/// \param value The value to encode.
template <typename T>
EncodeEnum<T> encodeEnum(T value);

///
/// Returns an encode operation for encoding an enum value as a member of
/// the current object.
///
/// \param name The name of the member to encode the value for.
/// \param value The value to encode.
template <typename T>
EncodeEnum<T> encodeEnum(const char* name, T value);

template <typename T>
DecodeEnum<T> decodeEnum(T& value);

template <typename T>
DecodeEnum<T> decodeEnum(const char* name, T& value);

}

#include "EncodeOperations.inl"