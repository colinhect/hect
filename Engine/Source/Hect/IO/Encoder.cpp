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
#include "Encoder.h"

namespace hect
{

Encoder& operator<<(Encoder& encoder, const BeginArray& begin_array)
{
    if (begin_array.name)
    {
        encoder.select_member(begin_array.name);
    }
    encoder.begin_array();
    return encoder;
}

Encoder& operator<<(Encoder& encoder, const EndArray&)
{
    encoder.end_array();
    return encoder;
}

Encoder& operator<<(Encoder& encoder, const BeginObject& begin_object)
{
    if (begin_object.name)
    {
        encoder.select_member(begin_object.name);
    }
    encoder.begin_object();
    return encoder;
}

Encoder& operator<<(Encoder& encoder, const EndObject&)
{
    encoder.end_object();
    return encoder;
}

Encoder& operator<<(Encoder& encoder, const char* value)
{
    encoder.encode_string(value);
    return encoder;
}

Encoder& operator<<(Encoder& encoder, const std::string& value)
{
    encoder.encode_string(value);
    return encoder;
}

Encoder& operator<<(Encoder& encoder, int8_t value)
{
    encoder.encode_int8(value);
    return encoder;
}

Encoder& operator<<(Encoder& encoder, uint8_t value)
{
    encoder.encode_u_int8(value);
    return encoder;
}

Encoder& operator<<(Encoder& encoder, int16_t value)
{
    encoder.encode_int16(value);
    return encoder;
}

Encoder& operator<<(Encoder& encoder, uint16_t value)
{
    encoder.encode_u_int16(value);
    return encoder;
}

Encoder& operator<<(Encoder& encoder, int32_t value)
{
    encoder.encode_int32(value);
    return encoder;
}

Encoder& operator<<(Encoder& encoder, uint32_t value)
{
    encoder.encode_u_int32(value);
    return encoder;
}

Encoder& operator<<(Encoder& encoder, int64_t value)
{
    encoder.encode_int64(value);
    return encoder;
}

Encoder& operator<<(Encoder& encoder, uint64_t value)
{
    encoder.encode_u_int64(value);
    return encoder;
}

Encoder& operator<<(Encoder& encoder, float value)
{
    encoder.encode_float32(value);
    return encoder;
}

Encoder& operator<<(Encoder& encoder, double value)
{
    encoder.encode_float64(value);
    return encoder;
}

Encoder& operator<<(Encoder& encoder, bool value)
{
    encoder.encode_bool(value);
    return encoder;
}

}