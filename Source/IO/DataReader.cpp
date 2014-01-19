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
#include "DataReader.h"

using namespace hect;

ArrayReader::ArrayReader(const ArrayReader& reader) :
    _reader(nullptr)
{
    reader;
}

ArrayReader::ArrayReader(ArrayReader&& reader) :
    _reader(reader._reader)
{
    reader._reader = nullptr;
}

ArrayReader::~ArrayReader()
{
    if (_reader)
    {
        _reader->endArray();
    }
}

bool ArrayReader::hasMoreElements() const
{
    assert(_reader);
    return _reader->hasMoreElements();
}

ArrayReader ArrayReader::readArray()
{
    assert(_reader);
    if (_reader->beginArray())
    {
        return ArrayReader(_reader);
    }
    else
    {
        return ArrayReader();
    }
}

ObjectReader ArrayReader::readObject()
{
    assert(_reader);
    if (_reader->beginObject())
    {
        return ObjectReader(_reader);
    }
    else
    {
        return ObjectReader();
    }
}

std::string ArrayReader::readString()
{
    assert(_reader);
    return _reader->readString();
}

int8_t ArrayReader::readByte()
{
    assert(_reader);
    return _reader->readByte();
}

uint8_t ArrayReader::readUnsignedByte()
{
    assert(_reader);
    return _reader->readUnsignedByte();
}

int16_t ArrayReader::readShort()
{
    assert(_reader);
    return _reader->readShort();
}

uint16_t ArrayReader::readUnsignedShort()
{
    assert(_reader);
    return _reader->readUnsignedShort();
}

int32_t ArrayReader::readInt()
{
    assert(_reader);
    return _reader->readInt();
}

uint32_t ArrayReader::readUnsignedInt()
{
    assert(_reader);
    return _reader->readUnsignedInt();
}

int64_t ArrayReader::readLong()
{
    assert(_reader);
    return _reader->readLong();
}

uint64_t ArrayReader::readUnsignedLong()
{
    assert(_reader);
    return _reader->readUnsignedLong();
}

float ArrayReader::readFloat()
{
    assert(_reader);
    return _reader->readFloat();
}

double ArrayReader::readDouble()
{
    assert(_reader);
    return _reader->readDouble();
}

Real ArrayReader::readReal()
{
    assert(_reader);
    return _reader->readReal();
}

Vector2 ArrayReader::readVector2()
{
    assert(_reader);
    return _reader->readVector2();
}

Vector3 ArrayReader::readVector3()
{
    assert(_reader);
    return _reader->readVector3();
}

Vector4 ArrayReader::readVector4()
{
    assert(_reader);
    return _reader->readVector4();
}

ArrayReader::operator bool() const
{
    return _reader != nullptr;
}

ArrayReader::ArrayReader() :
    _reader(nullptr)
{
}

ArrayReader::ArrayReader(DataReader* reader) :
    _reader(reader)
{
}

ObjectReader::ObjectReader(const ObjectReader& reader) :
    _reader(nullptr)
{
    reader;
}

ObjectReader::ObjectReader(ObjectReader&& reader) :
    _reader(reader._reader)
{
    reader._reader = nullptr;
}

ObjectReader::~ObjectReader()
{
    if (_reader)
    {
        _reader->endObject();
    }
}

bool ObjectReader::hasMember(const char* name) const
{
    return _reader->hasMember(name);
}

ArrayReader ObjectReader::readArray(const char* name)
{
    assert(_reader);
    if (_reader->beginArray(name))
    {
        return ArrayReader(_reader);
    }
    else
    {
        return ArrayReader();
    }
}

ObjectReader ObjectReader::readObject(const char* name)
{
    assert(_reader);
    if (_reader->beginObject(name))
    {
        return ObjectReader(_reader);
    }
    else
    {
        return ObjectReader();
    }
}

std::string ObjectReader::readString(const char* name)
{
    assert(_reader);
    return _reader->readString(name);
}

int8_t ObjectReader::readByte(const char* name)
{
    assert(_reader);
    return _reader->readByte(name);
}

uint8_t ObjectReader::readUnsignedByte(const char* name)
{
    assert(_reader);
    return _reader->readUnsignedByte(name);
}

int16_t ObjectReader::readShort(const char* name)
{
    assert(_reader);
    return _reader->readShort(name);
}

uint16_t ObjectReader::readUnsignedShort(const char* name)
{
    assert(_reader);
    return _reader->readUnsignedShort(name);
}

int32_t ObjectReader::readInt(const char* name)
{
    assert(_reader);
    return _reader->readInt(name);
}

uint32_t ObjectReader::readUnsignedInt(const char* name)
{
    assert(_reader);
    return _reader->readUnsignedInt(name);
}

int64_t ObjectReader::readLong(const char* name)
{
    assert(_reader);
    return _reader->readLong(name);
}

uint64_t ObjectReader::readUnsignedLong(const char* name)
{
    assert(_reader);
    return _reader->readUnsignedLong(name);
}

float ObjectReader::readFloat(const char* name)
{
    assert(_reader);
    return _reader->readFloat(name);
}

double ObjectReader::readDouble(const char* name)
{
    assert(_reader);
    return _reader->readDouble(name);
}

Real ObjectReader::readReal(const char* name)
{
    assert(_reader);
    return _reader->readReal(name);
}

Vector2 ObjectReader::readVector2(const char* name)
{
    assert(_reader);
    return _reader->readVector2(name);
}

Vector3 ObjectReader::readVector3(const char* name)
{
    assert(_reader);
    return _reader->readVector3(name);
}

Vector4 ObjectReader::readVector4(const char* name)
{
    assert(_reader);
    return _reader->readVector4(name);
}

ObjectReader::operator bool() const
{
    return _reader != nullptr;
}

ObjectReader::ObjectReader() :
    _reader(nullptr)
{
}

ObjectReader::ObjectReader(DataReader* reader) :
    _reader(reader)
{
}

DataValueReader::DataValueReader(const DataValue& dataValue)
{
    _valueStack.push(dataValue);
}

bool DataValueReader::isHumanReadable() const
{
    return true;
}

ArrayReader DataValueReader::readArray()
{
    if (_valueStack.top().isArray())
    {
        _indexStack.push(0);
        return ArrayReader(this);
    }
    else
    {
        return ArrayReader();
    }
}

ObjectReader DataValueReader::readObject()
{
    if (_valueStack.top().isObject())
    {
        return ObjectReader(this);
    }
    else
    {
        return ObjectReader();
    }
}

bool DataValueReader::beginArray()
{
    DataValue& top = _valueStack.top();
    assert(top.isArray());

    DataValue value = _deserialize();
    if (value.isArray())
    {
        _indexStack.push(0);
        _valueStack.push(value);
        return true;
    }
    else
    {
        return false;
    }
}

bool DataValueReader::beginArray(const char* name)
{
    DataValue& top = _valueStack.top();
    assert(top.isObject());

    const DataValue& value = top[name];
    if (!value.isNull())
    {
        _indexStack.push(0);
        _valueStack.push(value);
        return true;
    }
    else
    {
        return false;
    }
}

void DataValueReader::endArray()
{
    const DataValue& top = _valueStack.top();
    assert(top.isArray());
    _indexStack.pop();
    _valueStack.pop();
}

bool DataValueReader::hasMoreElements() const
{
    const DataValue& top = _valueStack.top();
    assert(top.isArray());
    return _indexStack.top() < top.size();
}

bool DataValueReader::beginObject()
{
    DataValue& top = _valueStack.top();
    assert(top.isArray());

    DataValue value = _deserialize();
    if (value.isObject())
    {
        _valueStack.push(value);
        return true;
    }
    else
    {
        return false;
    }
}

bool DataValueReader::beginObject(const char* name)
{
    DataValue& top = _valueStack.top();
    assert(top.isObject());

    const DataValue& value = top[name];
    if (!value.isNull())
    {
        _valueStack.push(value);
        return true;
    }
    else
    {
        return false;
    }
}

void DataValueReader::endObject()
{
    const DataValue& top = _valueStack.top();
    assert(top.isObject());
    _valueStack.pop();
}

bool DataValueReader::hasMember(const char* name) const
{
    const DataValue& top = _valueStack.top();
    assert(top.isObject());
    return !top[name].isNull();
}

std::string DataValueReader::readString()
{
    return _deserialize().asString();
}

std::string DataValueReader::readString(const char* name)
{
    return _deserialize(name).asString();
}

int8_t DataValueReader::readByte()
{
    return (int8_t)_deserialize().asDouble();
}

int8_t DataValueReader::readByte(const char* name)
{
    return (int8_t)_deserialize(name).asDouble();
}

uint8_t DataValueReader::readUnsignedByte()
{
    return (uint8_t)_deserialize().asDouble();
}

uint8_t DataValueReader::readUnsignedByte(const char* name)
{
    return (uint8_t)_deserialize(name).asDouble();
}

int16_t DataValueReader::readShort()
{
    return (int16_t)_deserialize().asDouble();
}

int16_t DataValueReader::readShort(const char* name)
{
    return (int16_t)_deserialize(name).asDouble();
}

uint16_t DataValueReader::readUnsignedShort()
{
    return (uint16_t)_deserialize().asDouble();
}

uint16_t DataValueReader::readUnsignedShort(const char* name)
{
    return (uint16_t)_deserialize(name).asDouble();
}

int32_t DataValueReader::readInt()
{
    return (int32_t)_deserialize().asDouble();
}

int32_t DataValueReader::readInt(const char* name)
{
    return (int32_t)_deserialize(name).asDouble();
}

uint32_t DataValueReader::readUnsignedInt()
{
    return (uint32_t)_deserialize().asDouble();
}

uint32_t DataValueReader::readUnsignedInt(const char* name)
{
    return (uint32_t)_deserialize(name).asDouble();
}

int64_t DataValueReader::readLong()
{
    return (int64_t)_deserialize().asDouble();
}

int64_t DataValueReader::readLong(const char* name)
{
    return (int64_t)_deserialize(name).asDouble();
}

uint64_t DataValueReader::readUnsignedLong()
{
    return (uint64_t)_deserialize().asDouble();
}

uint64_t DataValueReader::readUnsignedLong(const char* name)
{
    return (uint64_t)_deserialize(name).asDouble();
}

float DataValueReader::readFloat()
{
    return (float)_deserialize().asDouble();
}

float DataValueReader::readFloat(const char* name)
{
    return (float)_deserialize(name).asDouble();
}

double DataValueReader::readDouble()
{
    return _deserialize().asDouble();
}

double DataValueReader::readDouble(const char* name)
{
    return _deserialize(name).asDouble();
}

Real DataValueReader::readReal()
{
    return (Real)_deserialize().asDouble();
}

Real DataValueReader::readReal(const char* name)
{
    return (Real)_deserialize(name).asDouble();
}

Vector2 DataValueReader::readVector2()
{
    return _deserialize().asVector2();
}

Vector2 DataValueReader::readVector2(const char* name)
{
    return _deserialize(name).asVector2();
}

Vector3 DataValueReader::readVector3()
{
    return _deserialize().asVector3();
}

Vector3 DataValueReader::readVector3(const char* name)
{
    return _deserialize(name).asVector3();
}

Vector4 DataValueReader::readVector4()
{
    return _deserialize().asVector4();
}

Vector4 DataValueReader::readVector4(const char* name)
{
    return _deserialize(name).asVector4();
}

const DataValue& DataValueReader::_deserialize()
{
    DataValue& top = _valueStack.top();
    assert(top.isArray());
    return top[_indexStack.top()++];
}

const DataValue& DataValueReader::_deserialize(const char* name)
{
    DataValue& top = _valueStack.top();
    assert(top.isObject());
    return top[name];
}

BinaryReader::BinaryReader(ReadStream& stream) :
    _stream(&stream)
{
}

bool BinaryReader::isHumanReadable() const
{
    return false;
}

ArrayReader BinaryReader::readArray()
{
    _countStack.push(_stream->readUnsignedInt());
    _indexStack.push(0);

    return ArrayReader(this);
}

ObjectReader BinaryReader::readObject()
{
    return ObjectReader(this);
}

bool BinaryReader::beginArray()
{
    ++_indexStack.top();

    _countStack.push(_stream->readUnsignedInt());
    _indexStack.push(0);
    return true;
}

bool BinaryReader::beginArray(const char* name)
{
    name;

    _countStack.push(_stream->readUnsignedInt());
    _indexStack.push(0);
    return true;
}

void BinaryReader::endArray()
{
    _indexStack.pop();
    _countStack.pop();
}

bool BinaryReader::hasMoreElements() const
{
    return _indexStack.top() < _countStack.top();
}

bool BinaryReader::beginObject()
{
    ++_indexStack.top();
    return true;
}

bool BinaryReader::beginObject(const char* name)
{
    name;
    return true;
}

void BinaryReader::endObject()
{
}

bool BinaryReader::hasMember(const char* name) const
{
    name;
    return true; // Assume that all values are written
}

std::string BinaryReader::readString()
{
    ++_indexStack.top();
    return _stream->readString();
}

std::string BinaryReader::readString(const char* name)
{
    name;
    return _stream->readString();
}

int8_t BinaryReader::readByte()
{
    ++_indexStack.top();
    return _stream->readByte();
}

int8_t BinaryReader::readByte(const char* name)
{
    name;
    return _stream->readByte();
}

uint8_t BinaryReader::readUnsignedByte()
{
    ++_indexStack.top();
    return _stream->readUnsignedByte();
}

uint8_t BinaryReader::readUnsignedByte(const char* name)
{
    name;
    return _stream->readUnsignedByte();
}

int16_t BinaryReader::readShort()
{
    ++_indexStack.top();
    return _stream->readShort();
}

int16_t BinaryReader::readShort(const char* name)
{
    name;
    return _stream->readShort();
}

uint16_t BinaryReader::readUnsignedShort()
{
    ++_indexStack.top();
    return _stream->readUnsignedShort();
}

uint16_t BinaryReader::readUnsignedShort(const char* name)
{
    name;
    return _stream->readUnsignedShort();
}

int32_t BinaryReader::readInt()
{
    ++_indexStack.top();
    return _stream->readInt();
}

int32_t BinaryReader::readInt(const char* name)
{
    name;
    return _stream->readInt();
}

uint32_t BinaryReader::readUnsignedInt()
{
    ++_indexStack.top();
    return _stream->readUnsignedInt();
}

uint32_t BinaryReader::readUnsignedInt(const char* name)
{
    name;
    return _stream->readUnsignedInt();
}

int64_t BinaryReader::readLong()
{
    ++_indexStack.top();
    return _stream->readLong();
}

int64_t BinaryReader::readLong(const char* name)
{
    name;
    return _stream->readLong();
}

uint64_t BinaryReader::readUnsignedLong()
{
    ++_indexStack.top();
    return _stream->readUnsignedLong();
}

uint64_t BinaryReader::readUnsignedLong(const char* name)
{
    name;
    return _stream->readUnsignedLong();
}

float BinaryReader::readFloat()
{
    ++_indexStack.top();
    return _stream->readFloat();
}

float BinaryReader::readFloat(const char* name)
{
    name;
    return _stream->readFloat();
}

double BinaryReader::readDouble()
{
    ++_indexStack.top();
    return _stream->readDouble();
}

double BinaryReader::readDouble(const char* name)
{
    name;
    return _stream->readDouble();
}

Real BinaryReader::readReal()
{
    ++_indexStack.top();
    return _stream->readReal();
}

Real BinaryReader::readReal(const char* name)
{
    name;
    return _stream->readReal();
}

Vector2 BinaryReader::readVector2()
{
    ++_indexStack.top();
    return _stream->readVector2();
}

Vector2 BinaryReader::readVector2(const char* name)
{
    name;
    return _stream->readVector2();
}

Vector3 BinaryReader::readVector3()
{
    ++_indexStack.top();
    return _stream->readVector3();
}

Vector3 BinaryReader::readVector3(const char* name)
{
    name;
    return _stream->readVector3();
}

Vector4 BinaryReader::readVector4()
{
    ++_indexStack.top();
    return _stream->readVector4();
}

Vector4 BinaryReader::readVector4(const char* name)
{
    name;
    return _stream->readVector4();
}
