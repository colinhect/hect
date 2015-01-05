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
Decoder& operator>>(Decoder& decoder, const DecodeValue<T>& decodeValue)
{
    if (!decodeValue.name || decoder.selectMember(decodeValue.name))
    {
        return decoder >> decodeValue.value;
    }
    else
    {
        if (decodeValue.name && decodeValue.required)
        {
            throw DecodeError(format("Missing required value '%s'", decodeValue.name));
        }
        return decoder;
    }
}

template <typename T>
Decoder& operator>>(Decoder& decoder, const DecodeVector<T>& decodeVector)
{
    if (!decodeVector.name || decoder.selectMember(decodeVector.name))
    {
        decoder >> beginArray();

        while (decoder.hasMoreElements())
        {
            decodeVector.values.push_back(T());
            decoder >> decodeValue(decodeVector.values.back());
        }

        decoder >> endArray();
    }
    else
    {
        if (decodeVector.name && decodeVector.required)
        {
            throw DecodeError(format("Missing required value '%s'", decodeVector.name));
        }
    }
    return decoder;
}

template <typename T>
Decoder& operator>>(Decoder& decoder, const DecodeEnum<T>& decodeEnum)
{
    if (!decodeEnum.name || decoder.selectMember(decodeEnum.name))
    {
        if (decoder.isBinaryStream())
        {
            uint8_t value;
            decoder >> decodeValue(value);
            decodeEnum.value = static_cast<T>(value);
        }
        else
        {
            std::string string;
            decoder >> decodeValue(string);
            decodeEnum.value = Enum::fromString<T>(string);
        }
    }
    else
    {
        if (decodeEnum.name && decodeEnum.required)
        {
            throw DecodeError(format("Missing required value '%s'", decodeEnum.name));
        }
    }
    return decoder;
}

}