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

template <typename T>
Decoder& operator>>(Decoder& decoder, const DecodeValue<T>& decode_value)
{
    if (!decode_value.name || decoder.select_member(decode_value.name))
    {
        return decoder >> decode_value.value;
    }
    else
    {
        if (decode_value.name && decode_value.required)
        {
            throw DecodeError(format("Missing required value '%s'", decode_value.name));
        }
        return decoder;
    }
}

template <typename T>
Decoder& operator>>(Decoder& decoder, const DecodeVector<T>& decode_vector)
{
    if (!decode_vector.name || decoder.select_member(decode_vector.name))
    {
        decoder >> begin_array();

        while (decoder.has_more_elements())
        {
            decode_vector.values.push_back(T());
            decoder >> decode_value(decode_vector.values.back());
        }

        decoder >> end_array();
    }
    else
    {
        if (decode_vector.name && decode_vector.required)
        {
            throw DecodeError(format("Missing required value '%s'", decode_vector.name));
        }
    }
    return decoder;
}

template <typename T>
Decoder& operator>>(Decoder& decoder, const DecodeEnum<T>& decode_enum)
{
    if (!decode_enum.name || decoder.select_member(decode_enum.name))
    {
        if (decoder.is_binary_stream())
        {
            uint8_t value;
            decoder >> decode_value(value);
            decode_enum.value = static_cast<T>(value);
        }
        else
        {
            std::string string;
            decoder >> decode_value(string);
            decode_enum.value = Enum::from_string<T>(string);
        }
    }
    else
    {
        if (decode_enum.name && decode_enum.required)
        {
            throw DecodeError(format("Missing required value '%s'", decode_enum.name));
        }
    }
    return decoder;
}

}