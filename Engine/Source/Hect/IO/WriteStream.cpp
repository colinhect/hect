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

WriteStream::WriteStream()
{
}

WriteStream::WriteStream(WriteStream* implementation) :
    _implementation(implementation)
{
}

WriteStream::~WriteStream()
{
}

void WriteStream::write(const uint8_t* bytes, size_t byteCount)
{
    if (_implementation)
    {
        _implementation->write(bytes, byteCount);
    }
}

size_t WriteStream::position() const
{
    size_t position = 0;
    if (_implementation)
    {
        position = _implementation->position();
    }
    return position;
}

void WriteStream::seek(size_t position)
{
    if (_implementation)
    {
        _implementation->seek(position);
    }
}

namespace hect
{

WriteStream& operator<<(WriteStream& stream, const char* value)
{
    size_t length = strlen(value);
    stream << (uint32_t)length;
    stream.write((uint8_t*)&value[0], length);
    return stream;
}

WriteStream& operator<<(WriteStream& stream, const std::string& value)
{
    size_t length = value.size();
    stream << (uint32_t)length;
    stream.write((uint8_t*)&value[0], length);
    return stream;
}

WriteStream& operator<<(WriteStream& stream, int8_t value)
{
    stream.write((const uint8_t*)&value, 1);
    return stream;
}

WriteStream& operator<<(WriteStream& stream, uint8_t value)
{
    stream.write((const uint8_t*)&value, 1);
    return stream;
}

WriteStream& operator<<(WriteStream& stream, int16_t value)
{
    stream.write((const uint8_t*)&value, 2);
    return stream;
}

WriteStream& operator<<(WriteStream& stream, uint16_t value)
{
    stream.write((const uint8_t*)&value, 2);
    return stream;
}

WriteStream& operator<<(WriteStream& stream, int32_t value)
{
    stream.write((const uint8_t*)&value, 4);
    return stream;
}

WriteStream& operator<<(WriteStream& stream, uint32_t value)
{
    stream.write((const uint8_t*)&value, 4);
    return stream;
}

WriteStream& operator<<(WriteStream& stream, int64_t value)
{
    stream.write((const uint8_t*)&value, 8);
    return stream;
}

WriteStream& operator<<(WriteStream& stream, uint64_t value)
{
    stream.write((const uint8_t*)&value, 8);
    return stream;
}

WriteStream& operator<<(WriteStream& stream, float value)
{
    stream.write((const uint8_t*)&value, 4);
    return stream;
}

WriteStream& operator<<(WriteStream& stream, double value)
{
    stream.write((const uint8_t*)&value, 8);
    return stream;
}

WriteStream& operator<<(WriteStream& stream, bool value)
{
    return stream << (uint8_t)(value ? 1 : 0);
}

}
