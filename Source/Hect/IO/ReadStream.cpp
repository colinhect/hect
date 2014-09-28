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

std::string ReadStream::readString()
{
    size_t byteCount = readUInt32();
    std::string string(byteCount, ' ');
    readBytes((uint8_t*)&string[0], byteCount);
    return string;
}

std::string ReadStream::readAllToString()
{
    size_t byteCount = length();
    std::string string(byteCount, ' ');
    readBytes((uint8_t*)&string[0], byteCount);
    return string;
}

int8_t ReadStream::readInt8()
{
    int8_t value = 0;
    readBytes((uint8_t*)&value, 1);
    return value;
}

uint8_t ReadStream::readUInt8()
{
    uint8_t value = 0;
    readBytes((uint8_t*)&value, 1);
    return value;
}

int16_t ReadStream::readInt16()
{
    int16_t value = 0;
    readBytes((uint8_t*)&value, 2);
    return value;
}

uint16_t ReadStream::readUInt16()
{
    uint16_t value = 0;
    readBytes((uint8_t*)&value, 2);
    return value;
}

int32_t ReadStream::readInt32()
{
    int32_t value = 0;
    readBytes((uint8_t*)&value, 4);
    return value;
}

uint32_t ReadStream::readUInt32()
{
    uint32_t value = 0;
    readBytes((uint8_t*)&value, 4);
    return value;
}

int64_t ReadStream::readInt64()
{
    int64_t value = 0;
    readBytes((uint8_t*)&value, 8);
    return value;
}

uint64_t ReadStream::readUInt64()
{
    uint64_t value = 0;
    readBytes((uint8_t*)&value, 8);
    return value;
}

float ReadStream::readFloat32()
{
    float value = 0;
    readBytes((uint8_t*)&value, 4);
    return value;
}

double ReadStream::readFloat64()
{
    double value = 0;
    readBytes((uint8_t*)&value, 8);
    return value;
}

bool ReadStream::readBool()
{
    return readUInt8() != 0;
}