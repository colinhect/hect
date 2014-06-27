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

using namespace hect;

void WriteStream::writeString(const std::string& value, bool prependLength)
{
    size_t length = value.size();
    if (prependLength)
    {
        writeUnsignedInt((uint32_t)length);
    }
    writeBytes((uint8_t*)&value[0], length);
}

void WriteStream::writeByte(int8_t value)
{
    writeBytes((const uint8_t*)&value, 1);
}

void WriteStream::writeUnsignedByte(uint8_t value)
{
    writeBytes((const uint8_t*)&value, 1);
}

void WriteStream::writeShort(int16_t value)
{
    writeBytes((const uint8_t*)&value, 2);
}

void WriteStream::writeUnsignedShort(uint16_t value)
{
    writeBytes((const uint8_t*)&value, 2);
}

void WriteStream::writeInt(int32_t value)
{
    writeBytes((const uint8_t*)&value, 4);
}

void WriteStream::writeUnsignedInt(uint32_t value)
{
    writeBytes((const uint8_t*)&value, 4);
}

void WriteStream::writeLong(int64_t value)
{
    writeBytes((const uint8_t*)&value, 8);
}

void WriteStream::writeUnsignedLong(uint64_t value)
{
    writeBytes((const uint8_t*)&value, 8);
}

void WriteStream::writeFloat(float value)
{
    writeBytes((const uint8_t*)&value, 4);
}

void WriteStream::writeDouble(double value)
{
    writeBytes((const uint8_t*)&value, 8);
}

void WriteStream::writeReal(Real value)
{
#ifdef HECT_DOUBLE_PRECISION
    return writeDouble(value);
#else
    return writeFloat(value);
#endif
}

void WriteStream::writeBool(bool value)
{
    writeUnsignedByte(value ? 1 : 0);
}

void WriteStream::writeVector2(const Vector2& value)
{
    writeReal(value.x);
    writeReal(value.y);
}

void WriteStream::writeVector3(const Vector3& value)
{
    writeReal(value.x);
    writeReal(value.y);
    writeReal(value.z);
}

void WriteStream::writeVector4(const Vector4& value)
{
    writeReal(value.x);
    writeReal(value.y);
    writeReal(value.z);
    writeReal(value.w);
}