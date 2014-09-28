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

BinaryDecoder::BinaryDecoder(ReadStream& stream, AssetCache& assetCache) :
    Decoder(assetCache),
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


void BinaryDecoder::beginArray()
{
    if (!_valueTypeStack.empty() && _valueTypeStack.top() == ValueType_Array)
    {
        ++_indexStack.top();
    }

    _valueTypeStack.push(ValueType_Array);
    _countStack.push(_stream->readUInt32());
    _indexStack.push(0);
}

void BinaryDecoder::endArray()
{
    _indexStack.pop();
    _countStack.pop();
    _valueTypeStack.pop();
}

bool BinaryDecoder::hasMoreElements() const
{
    return _indexStack.top() < _countStack.top();
}

void BinaryDecoder::beginObject()
{
    if (!_valueTypeStack.empty() && _valueTypeStack.top() == ValueType_Array)
    {
        ++_indexStack.top();
    }
    _valueTypeStack.push(ValueType_Object);
}

void BinaryDecoder::endObject()
{
    _valueTypeStack.pop();
}

bool BinaryDecoder::selectMember(const char* name)
{
    name;
    return true; // Assume that all values are written
}

std::vector<std::string> BinaryDecoder::memberNames() const
{
    throw Error("Cannot enumerate member names from a binary object");
}

std::string BinaryDecoder::decodeString()
{
    if (!_valueTypeStack.empty() && _valueTypeStack.top() == ValueType_Array)
    {
        ++_indexStack.top();
    }
    return _stream->readString();
}

int8_t BinaryDecoder::decodeInt8()
{
    if (!_valueTypeStack.empty() && _valueTypeStack.top() == ValueType_Array)
    {
        ++_indexStack.top();
    }
    return _stream->readInt8();
}

uint8_t BinaryDecoder::decodeUInt8()
{
    if (!_valueTypeStack.empty() && _valueTypeStack.top() == ValueType_Array)
    {
        ++_indexStack.top();
    }
    return _stream->readUInt8();
}

int16_t BinaryDecoder::decodeInt16()
{
    if (!_valueTypeStack.empty() && _valueTypeStack.top() == ValueType_Array)
    {
        ++_indexStack.top();
    }
    return _stream->readInt16();
}

uint16_t BinaryDecoder::decodeUInt16()
{
    if (!_valueTypeStack.empty() && _valueTypeStack.top() == ValueType_Array)
    {
        ++_indexStack.top();
    }
    return _stream->readUInt16();
}

int32_t BinaryDecoder::decodeInt32()
{
    if (!_valueTypeStack.empty() && _valueTypeStack.top() == ValueType_Array)
    {
        ++_indexStack.top();
    }
    return _stream->readInt32();
}

uint32_t BinaryDecoder::decodeUInt32()
{
    if (!_valueTypeStack.empty() && _valueTypeStack.top() == ValueType_Array)
    {
        ++_indexStack.top();
    }
    return _stream->readUInt32();
}

int64_t BinaryDecoder::decodeInt64()
{
    if (!_valueTypeStack.empty() && _valueTypeStack.top() == ValueType_Array)
    {
        ++_indexStack.top();
    }
    return _stream->readInt64();
}

uint64_t BinaryDecoder::decodeUInt64()
{
    if (!_valueTypeStack.empty() && _valueTypeStack.top() == ValueType_Array)
    {
        ++_indexStack.top();
    }
    return _stream->readUInt64();
}

float BinaryDecoder::decodeFloat32()
{
    if (!_valueTypeStack.empty() && _valueTypeStack.top() == ValueType_Array)
    {
        ++_indexStack.top();
    }
    return _stream->readFloat32();
}

double BinaryDecoder::decodeFloat64()
{
    if (!_valueTypeStack.empty() && _valueTypeStack.top() == ValueType_Array)
    {
        ++_indexStack.top();
    }
    return _stream->readFloat64();
}

bool BinaryDecoder::decodeBool()
{
    if (!_valueTypeStack.empty() && _valueTypeStack.top() == ValueType_Array)
    {
        ++_indexStack.top();
    }
    return _stream->readBool();
}