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
Encoder& operator<<(Encoder& encoder, const EncodeValue<Type>& encode_value)
{
    if (encode_value.name)
    {
        encoder.select_member(encode_value.name);
    }
    encoder << encode_value.value;
    return encoder;
}

template <typename Type>
Encoder& operator<<(Encoder& encoder, const EncodeVector<Type>& encode_vector)
{
    if (encode_vector.name)
    {
        encoder.select_member(encode_vector.name);
    }
    encoder << begin_array();

    for (const Type& value : encode_vector.values)
    {
        encoder << encode_value(value);
    }

    encoder << end_array();
    return encoder;
}

template <typename Type>
Encoder& operator<<(Encoder& encoder, const EncodeEnum<Type>& encode_enum)
{
    if (encode_enum.name)
    {
        encoder.select_member(encode_enum.name);
    }

    if (encoder.is_binary_stream())
    {
        encoder << encode_value(static_cast<uint8_t>(encode_enum.value));
    }
    else
    {
        encoder << encode_value(Enum::to_string<Type>(encode_enum.value));
    }
    return encoder;
}

}