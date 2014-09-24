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
#include "Encoder.h"

namespace hect
{

Encoder& operator<<(Encoder& encoder, const BeginArray& beginArray)
{
    if (beginArray.name)
    {
        encoder.selectMember(beginArray.name);
    }
    return encoder.beginArray();
}

Encoder& operator<<(Encoder& encoder, const EndArray&)
{
    return encoder.endArray();
}

Encoder& operator<<(Encoder& encoder, const BeginObject& beginObject)
{
    if (beginObject.name)
    {
        encoder.selectMember(beginObject.name);
    }
    return encoder.beginObject();
}

Encoder& operator<<(Encoder& encoder, const EndObject&)
{
    return encoder.endObject();
}

Encoder& operator<<(Encoder& encoder, const char* value)
{
    return encoder.encodeString(value);
}

Encoder& operator<<(Encoder& encoder, const std::string& value)
{
    return encoder.encodeString(value);
}

Encoder& operator<<(Encoder& encoder, int8_t value)
{
    return encoder.encodeInt8(value);
}

Encoder& operator<<(Encoder& encoder, uint8_t value)
{
    return encoder.encodeUInt8(value);
}

Encoder& operator<<(Encoder& encoder, int16_t value)
{
    return encoder.encodeInt16(value);
}

Encoder& operator<<(Encoder& encoder, uint16_t value)
{
    return encoder.encodeUInt16(value);
}

Encoder& operator<<(Encoder& encoder, int32_t value)
{
    return encoder.encodeInt32(value);
}

Encoder& operator<<(Encoder& encoder, uint32_t value)
{
    return encoder.encodeUInt32(value);
}

Encoder& operator<<(Encoder& encoder, int64_t value)
{
    return encoder.encodeInt64(value);
}

Encoder& operator<<(Encoder& encoder, uint64_t value)
{
    return encoder.encodeUInt64(value);
}

Encoder& operator<<(Encoder& encoder, float value)
{
    return encoder.encodeFloat32(value);
}

Encoder& operator<<(Encoder& encoder, double value)
{
    return encoder.encodeFloat64(value);
}

Encoder& operator<<(Encoder& encoder, bool value)
{
    return encoder.encodeBool(value);
}

}