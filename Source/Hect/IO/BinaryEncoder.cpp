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
#include "BinaryEncoder.h"

#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"

using namespace hect;

BinaryEncoder::BinaryEncoder(WriteStream& stream) :
    _stream(&stream)
{
}

bool BinaryEncoder::isBinaryStream() const
{
    return true;
}

WriteStream& BinaryEncoder::binaryStream()
{
    return *_stream;
}

ArrayEncoder BinaryEncoder::encodeArray()
{
    _countStack.push(0);
    _countPositionStack.push(_stream->position());
    _stream->writeUInt32(0);
    return ArrayEncoder(this);
}

ObjectEncoder BinaryEncoder::encodeObject()
{
    _countStack.push(0);
    return ObjectEncoder(this);
}

void BinaryEncoder::beginArray()
{
    ++_countStack.top();
    _countStack.push(0);
    _countPositionStack.push(_stream->position());
    _stream->writeUInt32(0);
}

void BinaryEncoder::beginArray(const char* name)
{
    name;
    _countStack.push(0);
    _countPositionStack.push(_stream->position());
    _stream->writeUInt32(0);
}

void BinaryEncoder::endArray()
{
    size_t currentPosition = _stream->position();
    _stream->seek(_countPositionStack.top());
    _stream->writeUInt32(_countStack.top());
    _stream->seek(currentPosition);

    _countPositionStack.pop();
    _countStack.pop();
}

void BinaryEncoder::beginObject()
{
    ++_countStack.top();
}

void BinaryEncoder::beginObject(const char* name)
{
    name;
}

void BinaryEncoder::endObject()
{
}

void BinaryEncoder::encodeString(const std::string& value)
{
    ++_countStack.top();
    _stream->writeString(value);
}

void BinaryEncoder::encodeString(const char* name, const std::string& value)
{
    name;
    _stream->writeString(value);
}

void BinaryEncoder::encodeInt8(int8_t value)
{
    ++_countStack.top();
    _stream->writeInt8(value);
}

void BinaryEncoder::encodeInt8(const char* name, int8_t value)
{
    name;
    _stream->writeInt8(value);
}

void BinaryEncoder::encodeUInt8(uint8_t value)
{
    ++_countStack.top();
    _stream->writeUInt8(value);
}

void BinaryEncoder::encodeUInt8(const char* name, uint8_t value)
{
    name;
    _stream->writeUInt8(value);
}

void BinaryEncoder::encodeInt16(int16_t value)
{
    ++_countStack.top();
    _stream->writeInt16(value);
}

void BinaryEncoder::encodeInt16(const char* name, int16_t value)
{
    name;
    _stream->writeInt16(value);
}

void BinaryEncoder::encodeUInt16(uint16_t value)
{
    ++_countStack.top();
    _stream->writeUInt16(value);
}

void BinaryEncoder::encodeUInt16(const char* name, uint16_t value)
{
    name;
    _stream->writeUInt16(value);
}

void BinaryEncoder::encodeInt32(int32_t value)
{
    ++_countStack.top();
    _stream->writeInt32(value);
}

void BinaryEncoder::encodeInt32(const char* name, int32_t value)
{
    name;
    _stream->writeInt32(value);
}

void BinaryEncoder::encodeUInt32(uint32_t value)
{
    ++_countStack.top();
    _stream->writeUInt32(value);
}

void BinaryEncoder::encodeUInt32(const char* name, uint32_t value)
{
    name;
    _stream->writeUInt32(value);
}

void BinaryEncoder::encodeInt64(int64_t value)
{
    ++_countStack.top();
    _stream->writeInt64(value);
}

void BinaryEncoder::encodeInt64(const char* name, int64_t value)
{
    name;
    _stream->writeInt64(value);
}

void BinaryEncoder::encodeUInt64(uint64_t value)
{
    ++_countStack.top();
    _stream->writeUInt64(value);
}

void BinaryEncoder::encodeUInt64(const char* name, uint64_t value)
{
    name;
    _stream->writeUInt64(value);
}

void BinaryEncoder::encodeFloat32(float value)
{
    ++_countStack.top();
    _stream->writeFloat32(value);
}

void BinaryEncoder::encodeFloat32(const char* name, float value)
{
    name;
    _stream->writeFloat32(value);
}

void BinaryEncoder::encodeFloat64(double value)
{
    ++_countStack.top();
    _stream->writeFloat64(value);
}

void BinaryEncoder::encodeFloat64(const char* name, double value)
{
    name;
    _stream->writeFloat64(value);
}

void BinaryEncoder::encodeReal(Real value)
{
    ++_countStack.top();
    _stream->writeReal(value);
}

void BinaryEncoder::encodeReal(const char* name, Real value)
{
    name;
    _stream->writeReal(value);
}

void BinaryEncoder::encodeBool(bool value)
{
    ++_countStack.top();
    _stream->writeBool(value);
}

void BinaryEncoder::encodeBool(const char* name, bool value)
{
    name;
    _stream->writeBool(value);
}

void BinaryEncoder::encodeVector2(const Vector2& value)
{
    ++_countStack.top();
    _stream->writeVector2(value);
}

void BinaryEncoder::encodeVector2(const char* name, const Vector2& value)
{
    name;
    _stream->writeVector2(value);
}

void BinaryEncoder::encodeVector3(const Vector3& value)
{
    ++_countStack.top();
    _stream->writeVector3(value);
}

void BinaryEncoder::encodeVector3(const char* name, const Vector3& value)
{
    name;
    _stream->writeVector3(value);
}

void BinaryEncoder::encodeVector4(const Vector4& value)
{
    ++_countStack.top();
    _stream->writeVector4(value);
}

void BinaryEncoder::encodeVector4(const char* name, const Vector4& value)
{
    name;
    _stream->writeVector4(value);
}