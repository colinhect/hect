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
#include "Hect/IO/EncodingMagicNumber.h"

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
    uint8_t magicNumber = _stream->readUnsignedByte();
    if (magicNumber != EncodingMagicNumber::BeginArray)
    {
        throw Error("Expected beginning of array");
    }

    _countStack.push(_stream->readUnsignedInt());
    _indexStack.push(0);

    return ArrayDecoder(this);
}

ObjectDecoder BinaryDecoder::decodeObject()
{
    uint8_t magicNumber = _stream->readUnsignedByte();
    if (magicNumber != EncodingMagicNumber::BeginObject)
    {
        throw Error("Expected beginning of object");
    }

    return ObjectDecoder(this);
}

void BinaryDecoder::beginArray()
{
    uint8_t magicNumber = _stream->readUnsignedByte();
    if (magicNumber != EncodingMagicNumber::BeginArray)
    {
        throw Error("Expected beginning of array");
    }

    ++_indexStack.top();

    _countStack.push(_stream->readUnsignedInt());
    _indexStack.push(0);
}

void BinaryDecoder::beginArray(const char* name)
{
    uint8_t magicNumber = _stream->readUnsignedByte();
    if (magicNumber != EncodingMagicNumber::BeginArray)
    {
        throw Error("Expected beginning of array");
    }

    name;

    _countStack.push(_stream->readUnsignedInt());
    _indexStack.push(0);
}

void BinaryDecoder::endArray()
{
    uint8_t magicNumber = _stream->readUnsignedByte();
    if (magicNumber != EncodingMagicNumber::EndArray)
    {
        throw Error("Expected end of array");
    }

    _indexStack.pop();
    _countStack.pop();
}

bool BinaryDecoder::hasMoreElements() const
{
    return _indexStack.top() < _countStack.top();
}

void BinaryDecoder::beginObject()
{
    uint8_t magicNumber = _stream->readUnsignedByte();
    if (magicNumber != EncodingMagicNumber::BeginObject)
    {
        throw Error("Expected beginning of object");
    }

    ++_indexStack.top();
}

void BinaryDecoder::beginObject(const char* name)
{
    uint8_t magicNumber = _stream->readUnsignedByte();
    if (magicNumber != EncodingMagicNumber::BeginObject)
    {
        throw Error("Expected beginning of object");
    }

    name;
}

void BinaryDecoder::endObject()
{
    uint8_t magicNumber = _stream->readUnsignedByte();
    if (magicNumber != EncodingMagicNumber::EndObject)
    {
        throw Error("Expected end of object");
    }
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

int8_t BinaryDecoder::decodeByte()
{
    ++_indexStack.top();
    return _stream->readByte();
}

int8_t BinaryDecoder::decodeByte(const char* name)
{
    name;
    return _stream->readByte();
}

uint8_t BinaryDecoder::decodeUnsignedByte()
{
    ++_indexStack.top();
    return _stream->readUnsignedByte();
}

uint8_t BinaryDecoder::decodeUnsignedByte(const char* name)
{
    name;
    return _stream->readUnsignedByte();
}

int16_t BinaryDecoder::decodeShort()
{
    ++_indexStack.top();
    return _stream->readShort();
}

int16_t BinaryDecoder::decodeShort(const char* name)
{
    name;
    return _stream->readShort();
}

uint16_t BinaryDecoder::decodeUnsignedShort()
{
    ++_indexStack.top();
    return _stream->readUnsignedShort();
}

uint16_t BinaryDecoder::decodeUnsignedShort(const char* name)
{
    name;
    return _stream->readUnsignedShort();
}

int32_t BinaryDecoder::decodeInt()
{
    ++_indexStack.top();
    return _stream->readInt();
}

int32_t BinaryDecoder::decodeInt(const char* name)
{
    name;
    return _stream->readInt();
}

uint32_t BinaryDecoder::decodeUnsignedInt()
{
    ++_indexStack.top();
    return _stream->readUnsignedInt();
}

uint32_t BinaryDecoder::decodeUnsignedInt(const char* name)
{
    name;
    return _stream->readUnsignedInt();
}

int64_t BinaryDecoder::decodeLong()
{
    ++_indexStack.top();
    return _stream->readLong();
}

int64_t BinaryDecoder::decodeLong(const char* name)
{
    name;
    return _stream->readLong();
}

uint64_t BinaryDecoder::decodeUnsignedLong()
{
    ++_indexStack.top();
    return _stream->readUnsignedLong();
}

uint64_t BinaryDecoder::decodeUnsignedLong(const char* name)
{
    name;
    return _stream->readUnsignedLong();
}

float BinaryDecoder::decodeFloat()
{
    ++_indexStack.top();
    return _stream->readFloat();
}

float BinaryDecoder::decodeFloat(const char* name)
{
    name;
    return _stream->readFloat();
}

double BinaryDecoder::decodeDouble()
{
    ++_indexStack.top();
    return _stream->readDouble();
}

double BinaryDecoder::decodeDouble(const char* name)
{
    name;
    return _stream->readDouble();
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
