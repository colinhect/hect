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
namespace hect
{

template <typename Type>
EncodeValue<Type>::EncodeValue(const Type& value) :
    value(value)
{
}

template <typename Type>
EncodeValue<Type>::EncodeValue(const char* name, const Type& value) :
    name(name),
    value(value)
{
}

template <typename Type>
EncodeVector<Type>::EncodeVector(const std::vector<Type>& values) :
    values(values)
{
}

template <typename Type>
EncodeVector<Type>::EncodeVector(const char* name, const std::vector<Type>& values) :
    name(name),
    values(values)
{
}

template <typename Type>
EncodeEnum<Type>::EncodeEnum(Type value) :
    value(value)
{
}

template <typename Type>
EncodeEnum<Type>::EncodeEnum(const char* name, Type value) :
    name(name),
    value(value)
{
}

template <typename Type>
DecodeValue<Type>::DecodeValue(Type& value) :
    value(value)
{
}

template <typename Type>
DecodeValue<Type>::DecodeValue(const char* name, Type& value, bool required) :
    name(name),
    value(value),
    required(required)
{
}

template <typename Type>
DecodeVector<Type>::DecodeVector(std::vector<Type>& values) :
    values(values)
{
}

template <typename Type>
DecodeVector<Type>::DecodeVector(const char* name, std::vector<Type>& values, bool required) :
    name(name),
    values(values),
    required(required)
{
}

template <typename Type>
DecodeEnum<Type>::DecodeEnum(Type& value) :
    value(value)
{
}

template <typename Type>
DecodeEnum<Type>::DecodeEnum(const char* name, Type& value, bool required) :
    name(name),
    value(value),
    required(required)
{
}

template <typename Type>
EncodeValue<Type> encode_value(const Type& value)
{
    return EncodeValue<Type>(value);
}

template <typename Type>
EncodeValue<Type> encode_value(const char* name, const Type& value)
{
    return EncodeValue<Type>(name, value);
}

template <typename Type>
EncodeVector<Type> encode_vector(const std::vector<Type>& values)
{
    return EncodeVector<Type>(values);
}

template <typename Type>
EncodeVector<Type> encode_vector(const char* name, const std::vector<Type>& values)
{
    return EncodeVector<Type>(name, values);
}

template <typename Type>
EncodeEnum<Type> encode_enum(Type value)
{
    return EncodeEnum<Type>(value);
}

template <typename Type>
EncodeEnum<Type> encode_enum(const char* name, Type value)
{
    return EncodeEnum<Type>(name, value);
}

template <typename Type>
DecodeValue<Type> decode_value(Type& value)
{
    return DecodeValue<Type>(value);
}

template <typename Type>
DecodeValue<Type> decode_value(const char* name, Type& value, bool required)
{
    return DecodeValue<Type>(name, value, required);
}

template <typename Type>
DecodeVector<Type> decode_vector(std::vector<Type>& value)
{
    return DecodeVector<Type>(value);
}

template <typename Type>
DecodeVector<Type> decode_vector(const char* name, std::vector<Type>& value, bool required)
{
    return DecodeVector<Type>(name, value, required);
}

template <typename Type>
DecodeEnum<Type> decode_enum(Type& value)
{
    return DecodeEnum<Type>(value);
}

template <typename Type>
DecodeEnum<Type> decode_enum(const char* name, Type& value, bool required)
{
    return DecodeEnum<Type>(name, value, required);
}

}