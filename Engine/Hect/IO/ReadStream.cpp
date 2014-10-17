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
#include "ReadStream.h"

using namespace hect;

ReadStream::ReadStream()
{
}

ReadStream::ReadStream(ReadStream* implementation) :
    _implementation(implementation)
{
}

ReadStream::~ReadStream()
{
}

void ReadStream::read(uint8_t* bytes, size_t byteCount)
{
    if (_implementation)
    {
        _implementation->read(bytes, byteCount);
    }
}

std::string ReadStream::readAllToString()
{
    size_t byteCount = length();
    std::string string(byteCount, ' ');
    read((uint8_t*)&string[0], byteCount);
    return string;
}

bool ReadStream::endOfStream() const
{
    bool endOfStream = true;
    if (_implementation)
    {
        endOfStream = _implementation->endOfStream();
    }
    return endOfStream;
}

size_t ReadStream::length() const
{
    size_t length = true;
    if (_implementation)
    {
        length = _implementation->length();
    }
    return length;
}

size_t ReadStream::position() const
{
    size_t position = true;
    if (_implementation)
    {
        position = _implementation->position();
    }
    return position;
}

void ReadStream::seek(size_t position)
{
    if (_implementation)
    {
        _implementation->seek(position);
    }
}

namespace hect
{

ReadStream& operator>>(ReadStream& stream, std::string& value)
{
    uint32_t byteCount;
    stream >> byteCount;
    value = std::string(byteCount, ' ');
    stream.read((uint8_t*)&value[0], byteCount);
    return stream;
}

ReadStream& operator>>(ReadStream& stream, int8_t& value)
{
    stream.read((uint8_t*)&value, 1);
    return stream;
}

ReadStream& operator>>(ReadStream& stream, uint8_t& value)
{
    stream.read((uint8_t*)&value, 1);
    return stream;
}

ReadStream& operator>>(ReadStream& stream, int16_t& value)
{
    stream.read((uint8_t*)&value, 2);
    return stream;
}

ReadStream& operator>>(ReadStream& stream, uint16_t& value)
{
    stream.read((uint8_t*)&value, 2);
    return stream;
}

ReadStream& operator>>(ReadStream& stream, int32_t& value)
{
    stream.read((uint8_t*)&value, 4);
    return stream;
}

ReadStream& operator>>(ReadStream& stream, uint32_t& value)
{
    stream.read((uint8_t*)&value, 4);
    return stream;
}

ReadStream& operator>>(ReadStream& stream, int64_t& value)
{
    stream.read((uint8_t*)&value, 8);
    return stream;
}

ReadStream& operator>>(ReadStream& stream, uint64_t& value)
{
    stream.read((uint8_t*)&value, 8);
    return stream;
}

ReadStream& operator>>(ReadStream& stream, float& value)
{
    stream.read((uint8_t*)&value, 4);
    return stream;
}

ReadStream& operator>>(ReadStream& stream, double& value)
{
    stream.read((uint8_t*)&value, 8);
    return stream;
}

ReadStream& operator>>(ReadStream& stream, bool& value)
{
    uint8_t byte;
    stream.read(&byte, 1);
    value = byte != 0;
    return stream;
}

}