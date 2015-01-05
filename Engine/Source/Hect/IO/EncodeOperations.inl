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
namespace hect
{

template <typename T>
EncodeValue<T>::EncodeValue(const T& value) :
    value(value)
{
}

template <typename T>
EncodeValue<T>::EncodeValue(const char* name, const T& value) :
    name(name),
    value(value)
{
}

template <typename T>
EncodeVector<T>::EncodeVector(const std::vector<T>& values) :
    values(values)
{
}

template <typename T>
EncodeVector<T>::EncodeVector(const char* name, const std::vector<T>& values) :
    name(name),
    values(values)
{
}

template <typename T>
EncodeEnum<T>::EncodeEnum(T value) :
    value(value)
{
}

template <typename T>
EncodeEnum<T>::EncodeEnum(const char* name, T value) :
    name(name),
    value(value)
{
}

template <typename T>
DecodeValue<T>::DecodeValue(T& value) :
    value(value)
{
}

template <typename T>
DecodeValue<T>::DecodeValue(const char* name, T& value, bool required) :
    name(name),
    value(value),
    required(required)
{
}

template <typename T>
DecodeVector<T>::DecodeVector(std::vector<T>& values) :
    values(values)
{
}

template <typename T>
DecodeVector<T>::DecodeVector(const char* name, std::vector<T>& values, bool required) :
    name(name),
    values(values),
    required(required)
{
}

template <typename T>
DecodeEnum<T>::DecodeEnum(T& value) :
    value(value)
{
}

template <typename T>
DecodeEnum<T>::DecodeEnum(const char* name, T& value, bool required) :
    name(name),
    value(value),
    required(required)
{
}

template <typename T>
EncodeValue<T> encodeValue(const T& value)
{
    return EncodeValue<T>(value);
}

template <typename T>
EncodeValue<T> encodeValue(const char* name, const T& value)
{
    return EncodeValue<T>(name, value);
}

template <typename T>
EncodeVector<T> encodeVector(const std::vector<T>& values)
{
    return EncodeVector<T>(values);
}

template <typename T>
EncodeVector<T> encodeVector(const char* name, const std::vector<T>& values)
{
    return EncodeVector<T>(name, values);
}

template <typename T>
EncodeEnum<T> encodeEnum(T value)
{
    return EncodeEnum<T>(value);
}

template <typename T>
EncodeEnum<T> encodeEnum(const char* name, T value)
{
    return EncodeEnum<T>(name, value);
}

template <typename T>
DecodeValue<T> decodeValue(T& value)
{
    return DecodeValue<T>(value);
}

template <typename T>
DecodeValue<T> decodeValue(const char* name, T& value, bool required)
{
    return DecodeValue<T>(name, value, required);
}

template <typename T>
DecodeVector<T> decodeVector(std::vector<T>& value)
{
    return DecodeVector<T>(value);
}

template <typename T>
DecodeVector<T> decodeVector(const char* name, std::vector<T>& value, bool required)
{
    return DecodeVector<T>(name, value, required);
}

template <typename T>
DecodeEnum<T> decodeEnum(T& value)
{
    return DecodeEnum<T>(value);
}

template <typename T>
DecodeEnum<T> decodeEnum(const char* name, T& value, bool required)
{
    return DecodeEnum<T>(name, value, required);
}

}