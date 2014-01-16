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
#include "Hect.h"

using namespace hect;

std::string ReadStream::readString()
{
    size_t byteCount = readUnsignedInt();
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

int8_t ReadStream::readByte()
{
    int8_t value = 0;
    readBytes((uint8_t*)&value, 1);
    return value;
}

uint8_t ReadStream::readUnsignedByte()
{
    uint8_t value = 0;
    readBytes((uint8_t*)&value, 1);
    return value;
}

int16_t ReadStream::readShort()
{
    int16_t value = 0;
    readBytes((uint8_t*)&value, 2);
    return value;
}

uint16_t ReadStream::readUnsignedShort()
{
    uint16_t value = 0;
    readBytes((uint8_t*)&value, 2);
    return value;
}

int32_t ReadStream::readInt()
{
    int32_t value = 0;
    readBytes((uint8_t*)&value, 4);
    return value;
}

uint32_t ReadStream::readUnsignedInt()
{
    uint32_t value = 0;
    readBytes((uint8_t*)&value, 4);
    return value;
}

int64_t ReadStream::readLong()
{
    int64_t value = 0;
    readBytes((uint8_t*)&value, 8);
    return value;
}

uint64_t ReadStream::readUnsignedLong()
{
    uint64_t value = 0;
    readBytes((uint8_t*)&value, 8);
    return value;
}

float ReadStream::readFloat()
{
    float value = 0;
    readBytes((uint8_t*)&value, 4);
    return value;
}

double ReadStream::readDouble()
{
    double value = 0;
    readBytes((uint8_t*)&value, 8);
    return value;
}

Vector2<> ReadStream::readVector2()
{
    double x = readDouble();
    double y = readDouble();
    return Vector2<>(x, y);
}

Vector3<> ReadStream::readVector3()
{
    double x = readDouble();
    double y = readDouble();
    double z = readDouble();
    return Vector3<>(x, y, z);
}

Vector4<> ReadStream::readVector4()
{
    double x = readDouble();
    double y = readDouble();
    double z = readDouble();
    double w = readDouble();
    return Vector4<>(x, y, z, w);
}

Quaternion<> ReadStream::readQuaternion()
{
    double x = readDouble();
    double y = readDouble();
    double z = readDouble();
    double w = readDouble();
    return Quaternion<>(x, y, z, w);
}