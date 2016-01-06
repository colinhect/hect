///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2016 Colin Hill
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

#include "Hect/Core/Exception.h"
#include "Hect/Core/Format.h"
#include "Hect/IO/MemoryReadStream.h"

using namespace hect;

BinaryDecoder::BinaryDecoder(ReadStream& stream) :
    _stream(stream)
{
}

BinaryDecoder::BinaryDecoder(ReadStream& stream, AssetCache& assetCache) :
    Decoder(assetCache),
    _stream(stream)
{
}

BinaryDecoder::BinaryDecoder(const ByteVector& data) :
    _ownedStream(new MemoryReadStream(data)),
    _stream(*_ownedStream)
{
}

bool BinaryDecoder::isBinaryStream() const
{
    return true;
}

ReadStream& BinaryDecoder::binaryStream()
{
    return _stream;
}

void BinaryDecoder::beginArray()
{
    incrementIndex();

    uint32_t count;
    _stream >> count;

    _valueTypeStack.push(ValueType_Array);
    _countStack.push(count);
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
    incrementIndex();

    _valueTypeStack.push(ValueType_Object);
}

void BinaryDecoder::endObject()
{
    _valueTypeStack.pop();
}

bool BinaryDecoder::selectMember(const char* name)
{
    (void)name;
    return true;
}

std::vector<std::string> BinaryDecoder::memberNames() const
{
    throw InvalidOperation("Cannot enumerate member names from a binary data source");
}

std::string BinaryDecoder::decodeString()
{
    incrementIndex();

    std::string value;
    _stream >> value;
    return value;
}

int8_t BinaryDecoder::decodeInt8()
{
    incrementIndex();

    int8_t value;
    _stream >> value;
    return value;
}

uint8_t BinaryDecoder::decodeUInt8()
{
    incrementIndex();

    uint8_t value;
    _stream >> value;
    return value;
}

int16_t BinaryDecoder::decodeInt16()
{
    incrementIndex();

    int16_t value;
    _stream >> value;
    return value;
}

uint16_t BinaryDecoder::decodeUInt16()
{
    incrementIndex();

    uint16_t value;
    _stream >> value;
    return value;
}

int32_t BinaryDecoder::decodeInt32()
{
    incrementIndex();

    int32_t value;
    _stream >> value;
    return value;
}

uint32_t BinaryDecoder::decodeUInt32()
{
    incrementIndex();

    uint32_t value;
    _stream >> value;
    return value;
}

int64_t BinaryDecoder::decodeInt64()
{
    incrementIndex();

    int64_t value;
    _stream >> value;
    return value;
}

uint64_t BinaryDecoder::decodeUInt64()
{
    incrementIndex();

    uint64_t value;
    _stream >> value;
    return value;
}

float BinaryDecoder::decodeFloat32()
{
    incrementIndex();

    float value;
    _stream >> value;
    return value;
}

double BinaryDecoder::decodeFloat64()
{
    incrementIndex();

    double value;
    _stream >> value;
    return value;
}

bool BinaryDecoder::decodeBool()
{
    incrementIndex();

    bool value;
    _stream >> value;
    return value;
}

void BinaryDecoder::incrementIndex()
{
    if (!_valueTypeStack.empty() && _valueTypeStack.top() == ValueType_Array)
    {
        ++_indexStack.top();
    }
}