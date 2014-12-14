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
#include "WriteStream.h"

#include <cstring>

using namespace hect;

namespace hect
{

WriteStream& operator<<(WriteStream& stream, const char* value)
{
    size_t length = strlen(value);
    stream << static_cast<uint32_t>(length);
    if (length > 0)
    {
        stream.write(reinterpret_cast<const uint8_t*>(&value[0]), length);
    }
    return stream;
}

WriteStream& operator<<(WriteStream& stream, const std::string& value)
{
    size_t length = value.size();
    stream << static_cast<uint32_t>(length);
    if (length > 0)
    {
        stream.write(reinterpret_cast<const uint8_t*>(&value[0]), length);
    }
    return stream;
}

WriteStream& operator<<(WriteStream& stream, int8_t value)
{
    stream.write(reinterpret_cast<const uint8_t*>(&value), 1);
    return stream;
}

WriteStream& operator<<(WriteStream& stream, uint8_t value)
{
    stream.write(reinterpret_cast<const uint8_t*>(&value), 1);
    return stream;
}

WriteStream& operator<<(WriteStream& stream, int16_t value)
{
    stream.write(reinterpret_cast<const uint8_t*>(&value), 2);
    return stream;
}

WriteStream& operator<<(WriteStream& stream, uint16_t value)
{
    stream.write(reinterpret_cast<const uint8_t*>(&value), 2);
    return stream;
}

WriteStream& operator<<(WriteStream& stream, int32_t value)
{
    stream.write(reinterpret_cast<const uint8_t*>(&value), 4);
    return stream;
}

WriteStream& operator<<(WriteStream& stream, uint32_t value)
{
    stream.write(reinterpret_cast<const uint8_t*>(&value), 4);
    return stream;
}

WriteStream& operator<<(WriteStream& stream, int64_t value)
{
    stream.write(reinterpret_cast<const uint8_t*>(&value), 8);
    return stream;
}

WriteStream& operator<<(WriteStream& stream, uint64_t value)
{
    stream.write(reinterpret_cast<const uint8_t*>(&value), 8);
    return stream;
}

WriteStream& operator<<(WriteStream& stream, float value)
{
    stream.write(reinterpret_cast<const uint8_t*>(&value), 4);
    return stream;
}

WriteStream& operator<<(WriteStream& stream, double value)
{
    stream.write(reinterpret_cast<const uint8_t*>(&value), 8);
    return stream;
}

WriteStream& operator<<(WriteStream& stream, bool value)
{
    return stream << static_cast<uint8_t>(value ? 1 : 0);
}

}
