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
#include "BinaryDecoder.h"

#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"

using namespace hect;

BinaryDecoder::BinaryDecoder(ReadStream& stream) :
    _stream(&stream)
{
}

bool BinaryDecoder::isBinaryStream() const
{
    return true;
}

ReadStream& BinaryDecoder::binaryStream()
{
    return *_stream;
}

ArrayDecoder BinaryDecoder::decodeArray()
{
    _countStack.push(_stream->readUInt32());
    _indexStack.push(0);

    return ArrayDecoder(this);
}

ObjectDecoder BinaryDecoder::decodeObject()
{
    return ObjectDecoder(this);
}

void BinaryDecoder::beginArray()
{
    ++_indexStack.top();

    _countStack.push(_stream->readUInt32());
    _indexStack.push(0);
}

void BinaryDecoder::beginArray(const char* name)
{
    name;

    _countStack.push(_stream->readUInt32());
    _indexStack.push(0);
}

void BinaryDecoder::endArray()
{
    _indexStack.pop();
    _countStack.pop();
}

bool BinaryDecoder::hasMoreElements() const
{
    return _indexStack.top() < _countStack.top();
}

void BinaryDecoder::beginObject()
{
    ++_indexStack.top();
}

void BinaryDecoder::beginObject(const char* name)
{
    name;
}

void BinaryDecoder::endObject()
{
}

bool BinaryDecoder::hasMember(const char* name) const
{
    name;
    return true; // Assume that all values are written
}

std::string BinaryDecoder::decodeString()
{
    ++_indexStack.top();
    return _stream->readString();
}

std::string BinaryDecoder::decodeString(const char* name)
{
    name;
    return _stream->readString();
}

int8_t BinaryDecoder::decodeInt8()
{
    ++_indexStack.top();
    return _stream->readInt8();
}

int8_t BinaryDecoder::decodeInt8(const char* name)
{
    name;
    return _stream->readInt8();
}

uint8_t BinaryDecoder::decodeUInt8()
{
    ++_indexStack.top();
    return _stream->readUInt8();
}

uint8_t BinaryDecoder::decodeUInt8(const char* name)
{
    name;
    return _stream->readUInt8();
}

int16_t BinaryDecoder::decodeInt16()
{
    ++_indexStack.top();
    return _stream->readInt16();
}

int16_t BinaryDecoder::decodeInt16(const char* name)
{
    name;
    return _stream->readInt16();
}

uint16_t BinaryDecoder::decodeUInt16()
{
    ++_indexStack.top();
    return _stream->readUInt16();
}

uint16_t BinaryDecoder::decodeUInt16(const char* name)
{
    name;
    return _stream->readUInt16();
}

int32_t BinaryDecoder::decodeInt32()
{
    ++_indexStack.top();
    return _stream->readInt32();
}

int32_t BinaryDecoder::decodeInt32(const char* name)
{
    name;
    return _stream->readInt32();
}

uint32_t BinaryDecoder::decodeUInt32()
{
    ++_indexStack.top();
    return _stream->readUInt32();
}

uint32_t BinaryDecoder::decodeUInt32(const char* name)
{
    name;
    return _stream->readUInt32();
}

int64_t BinaryDecoder::decodeInt64()
{
    ++_indexStack.top();
    return _stream->readInt64();
}

int64_t BinaryDecoder::decodeInt64(const char* name)
{
    name;
    return _stream->readInt64();
}

uint64_t BinaryDecoder::decodeUInt64()
{
    ++_indexStack.top();
    return _stream->readUInt64();
}

uint64_t BinaryDecoder::decodeUInt64(const char* name)
{
    name;
    return _stream->readUInt64();
}

float BinaryDecoder::decodeFloat32()
{
    ++_indexStack.top();
    return _stream->readFloat32();
}

float BinaryDecoder::decodeFloat32(const char* name)
{
    name;
    return _stream->readFloat32();
}

double BinaryDecoder::decodeFloat64()
{
    ++_indexStack.top();
    return _stream->readFloat64();
}

double BinaryDecoder::decodeFloat64(const char* name)
{
    name;
    return _stream->readFloat64();
}

Real BinaryDecoder::decodeReal()
{
    ++_indexStack.top();
    return _stream->readReal();
}

Real BinaryDecoder::decodeReal(const char* name)
{
    name;
    return _stream->readReal();
}

bool BinaryDecoder::decodeBool()
{
    ++_indexStack.top();
    return _stream->readBool();
}

bool BinaryDecoder::decodeBool(const char* name)
{
    name;
    return _stream->readBool();
}

Vector2 BinaryDecoder::decodeVector2()
{
    ++_indexStack.top();
    return _stream->readVector2();
}

Vector2 BinaryDecoder::decodeVector2(const char* name)
{
    name;
    return _stream->readVector2();
}

Vector3 BinaryDecoder::decodeVector3()
{
    ++_indexStack.top();
    return _stream->readVector3();
}

Vector3 BinaryDecoder::decodeVector3(const char* name)
{
    name;
    return _stream->readVector3();
}

Vector4 BinaryDecoder::decodeVector4()
{
    ++_indexStack.top();
    return _stream->readVector4();
}

Vector4 BinaryDecoder::decodeVector4(const char* name)
{
    name;
    return _stream->readVector4();
}
