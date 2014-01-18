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
#include "Deserializer.h"

using namespace hect;

ArrayDeserializer::ArrayDeserializer(const ArrayDeserializer& deserializer) :
    _deserializer(nullptr)
{
    deserializer;
}

ArrayDeserializer::ArrayDeserializer(ArrayDeserializer&& deserializer) :
    _deserializer(deserializer._deserializer)
{
    deserializer._deserializer = nullptr;
}

ArrayDeserializer::~ArrayDeserializer()
{
    if (_deserializer)
    {
        _deserializer->endArray();
    }
}

bool ArrayDeserializer::hasMoreElements() const
{
    assert(_deserializer);
    return _deserializer->hasMoreElements();
}

ArrayDeserializer ArrayDeserializer::readArray()
{
    assert(_deserializer);
    if (_deserializer->beginArray())
    {
        return ArrayDeserializer(_deserializer);
    }
    else
    {
        return ArrayDeserializer();
    }
}

ObjectDeserializer ArrayDeserializer::readObject()
{
    assert(_deserializer);
    if (_deserializer->beginObject())
    {
        return ObjectDeserializer(_deserializer);
    }
    else
    {
        return ObjectDeserializer();
    }
}

std::string ArrayDeserializer::readString()
{
    assert(_deserializer);
    return _deserializer->readString();
}

int8_t ArrayDeserializer::readByte()
{
    assert(_deserializer);
    return _deserializer->readByte();
}

uint8_t ArrayDeserializer::readUnsignedByte()
{
    assert(_deserializer);
    return _deserializer->readUnsignedByte();
}

int16_t ArrayDeserializer::readShort()
{
    assert(_deserializer);
    return _deserializer->readShort();
}

uint16_t ArrayDeserializer::readUnsignedShort()
{
    assert(_deserializer);
    return _deserializer->readUnsignedShort();
}

int32_t ArrayDeserializer::readInt()
{
    assert(_deserializer);
    return _deserializer->readInt();
}

uint32_t ArrayDeserializer::readUnsignedInt()
{
    assert(_deserializer);
    return _deserializer->readUnsignedInt();
}

int64_t ArrayDeserializer::readLong()
{
    assert(_deserializer);
    return _deserializer->readLong();
}

uint64_t ArrayDeserializer::readUnsignedLong()
{
    assert(_deserializer);
    return _deserializer->readUnsignedLong();
}

float ArrayDeserializer::readFloat()
{
    assert(_deserializer);
    return _deserializer->readFloat();
}

double ArrayDeserializer::readDouble()
{
    assert(_deserializer);
    return _deserializer->readDouble();
}

Real ArrayDeserializer::readReal()
{
    assert(_deserializer);
    return _deserializer->readReal();
}

Vector2 ArrayDeserializer::readVector2()
{
    assert(_deserializer);
    return _deserializer->readVector2();
}

Vector3 ArrayDeserializer::readVector3()
{
    assert(_deserializer);
    return _deserializer->readVector3();
}

Vector4 ArrayDeserializer::readVector4()
{
    assert(_deserializer);
    return _deserializer->readVector4();
}

ArrayDeserializer::operator bool() const
{
    return _deserializer != nullptr;
}

ArrayDeserializer::ArrayDeserializer() :
    _deserializer(nullptr)
{
}

ArrayDeserializer::ArrayDeserializer(Deserializer* deserializer) :
    _deserializer(deserializer)
{
}

ObjectDeserializer::ObjectDeserializer(const ObjectDeserializer& deserializer) :
    _deserializer(nullptr)
{
    deserializer;
}

ObjectDeserializer::ObjectDeserializer(ObjectDeserializer&& deserializer) :
    _deserializer(deserializer._deserializer)
{
    deserializer._deserializer = nullptr;
}

ObjectDeserializer::~ObjectDeserializer()
{
    if (_deserializer)
    {
        _deserializer->endObject();
    }
}

bool ObjectDeserializer::hasMember(const char* name) const
{
    return _deserializer->hasMember(name);
}

ArrayDeserializer ObjectDeserializer::readArray(const char* name)
{
    assert(_deserializer);
    if (_deserializer->beginArray(name))
    {
        return ArrayDeserializer(_deserializer);
    }
    else
    {
        return ArrayDeserializer();
    }
}

ObjectDeserializer ObjectDeserializer::readObject(const char* name)
{
    assert(_deserializer);
    if (_deserializer->beginObject(name))
    {
        return ObjectDeserializer(_deserializer);
    }
    else
    {
        return ObjectDeserializer();
    }
}

std::string ObjectDeserializer::readString(const char* name)
{
    assert(_deserializer);
    return _deserializer->readString(name);
}

int8_t ObjectDeserializer::readByte(const char* name)
{
    assert(_deserializer);
    return _deserializer->readByte(name);
}

uint8_t ObjectDeserializer::readUnsignedByte(const char* name)
{
    assert(_deserializer);
    return _deserializer->readUnsignedByte(name);
}

int16_t ObjectDeserializer::readShort(const char* name)
{
    assert(_deserializer);
    return _deserializer->readShort(name);
}

uint16_t ObjectDeserializer::readUnsignedShort(const char* name)
{
    assert(_deserializer);
    return _deserializer->readUnsignedShort(name);
}

int32_t ObjectDeserializer::readInt(const char* name)
{
    assert(_deserializer);
    return _deserializer->readInt(name);
}

uint32_t ObjectDeserializer::readUnsignedInt(const char* name)
{
    assert(_deserializer);
    return _deserializer->readUnsignedInt(name);
}

int64_t ObjectDeserializer::readLong(const char* name)
{
    assert(_deserializer);
    return _deserializer->readLong(name);
}

uint64_t ObjectDeserializer::readUnsignedLong(const char* name)
{
    assert(_deserializer);
    return _deserializer->readUnsignedLong(name);
}

float ObjectDeserializer::readFloat(const char* name)
{
    assert(_deserializer);
    return _deserializer->readFloat(name);
}

double ObjectDeserializer::readDouble(const char* name)
{
    assert(_deserializer);
    return _deserializer->readDouble(name);
}

Real ObjectDeserializer::readReal(const char* name)
{
    assert(_deserializer);
    return _deserializer->readReal(name);
}

Vector2 ObjectDeserializer::readVector2(const char* name)
{
    assert(_deserializer);
    return _deserializer->readVector2(name);
}

Vector3 ObjectDeserializer::readVector3(const char* name)
{
    assert(_deserializer);
    return _deserializer->readVector3(name);
}

Vector4 ObjectDeserializer::readVector4(const char* name)
{
    assert(_deserializer);
    return _deserializer->readVector4(name);
}

ObjectDeserializer::operator bool() const
{
    return _deserializer != nullptr;
}

ObjectDeserializer::ObjectDeserializer() :
    _deserializer(nullptr)
{
}

ObjectDeserializer::ObjectDeserializer(Deserializer* deserializer) :
    _deserializer(deserializer)
{
}

DataValueDeserializer::DataValueDeserializer(const DataValue& dataValue)
{
    _valueStack.push(dataValue);
}

bool DataValueDeserializer::isHumanReadable() const
{
    return true;
}

ArrayDeserializer DataValueDeserializer::readArray()
{
    if (_valueStack.top().isArray())
    {
        _indexStack.push(0);
        return ArrayDeserializer(this);
    }
    else
    {
        return ArrayDeserializer();
    }
}

ObjectDeserializer DataValueDeserializer::readObject()
{
    if (_valueStack.top().isObject())
    {
        return ObjectDeserializer(this);
    }
    else
    {
        return ObjectDeserializer();
    }
}

bool DataValueDeserializer::beginArray()
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

bool DataValueDeserializer::beginArray(const char* name)
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

void DataValueDeserializer::endArray()
{
    const DataValue& top = _valueStack.top();
    assert(top.isArray());
    _indexStack.pop();
    _valueStack.pop();
}

bool DataValueDeserializer::hasMoreElements() const
{
    const DataValue& top = _valueStack.top();
    assert(top.isArray());
    return _indexStack.top() < top.size();
}

bool DataValueDeserializer::beginObject()
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

bool DataValueDeserializer::beginObject(const char* name)
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

void DataValueDeserializer::endObject()
{
    const DataValue& top = _valueStack.top();
    assert(top.isObject());
    _valueStack.pop();
}

bool DataValueDeserializer::hasMember(const char* name) const
{
    const DataValue& top = _valueStack.top();
    assert(top.isObject());
    return !top[name].isNull();
}

std::string DataValueDeserializer::readString()
{
    return _deserialize().asString();
}

std::string DataValueDeserializer::readString(const char* name)
{
    return _deserialize(name).asString();
}

int8_t DataValueDeserializer::readByte()
{
    return (int8_t)_deserialize().asDouble();
}

int8_t DataValueDeserializer::readByte(const char* name)
{
    return (int8_t)_deserialize(name).asDouble();
}

uint8_t DataValueDeserializer::readUnsignedByte()
{
    return (uint8_t)_deserialize().asDouble();
}

uint8_t DataValueDeserializer::readUnsignedByte(const char* name)
{
    return (uint8_t)_deserialize(name).asDouble();
}

int16_t DataValueDeserializer::readShort()
{
    return (int16_t)_deserialize().asDouble();
}

int16_t DataValueDeserializer::readShort(const char* name)
{
    return (int16_t)_deserialize(name).asDouble();
}

uint16_t DataValueDeserializer::readUnsignedShort()
{
    return (uint16_t)_deserialize().asDouble();
}

uint16_t DataValueDeserializer::readUnsignedShort(const char* name)
{
    return (uint16_t)_deserialize(name).asDouble();
}

int32_t DataValueDeserializer::readInt()
{
    return (int32_t)_deserialize().asDouble();
}

int32_t DataValueDeserializer::readInt(const char* name)
{
    return (int32_t)_deserialize(name).asDouble();
}

uint32_t DataValueDeserializer::readUnsignedInt()
{
    return (uint32_t)_deserialize().asDouble();
}

uint32_t DataValueDeserializer::readUnsignedInt(const char* name)
{
    return (uint32_t)_deserialize(name).asDouble();
}

int64_t DataValueDeserializer::readLong()
{
    return (int64_t)_deserialize().asDouble();
}

int64_t DataValueDeserializer::readLong(const char* name)
{
    return (int64_t)_deserialize(name).asDouble();
}

uint64_t DataValueDeserializer::readUnsignedLong()
{
    return (uint64_t)_deserialize().asDouble();
}

uint64_t DataValueDeserializer::readUnsignedLong(const char* name)
{
    return (uint64_t)_deserialize(name).asDouble();
}

float DataValueDeserializer::readFloat()
{
    return (float)_deserialize().asDouble();
}

float DataValueDeserializer::readFloat(const char* name)
{
    return (float)_deserialize(name).asDouble();
}

double DataValueDeserializer::readDouble()
{
    return _deserialize().asDouble();
}

double DataValueDeserializer::readDouble(const char* name)
{
    return _deserialize(name).asDouble();
}

Real DataValueDeserializer::readReal()
{
    return (Real)_deserialize().asDouble();
}

Real DataValueDeserializer::readReal(const char* name)
{
    return (Real)_deserialize(name).asDouble();
}

Vector2 DataValueDeserializer::readVector2()
{
    return _deserialize().asVector2();
}

Vector2 DataValueDeserializer::readVector2(const char* name)
{
    return _deserialize(name).asVector2();
}

Vector3 DataValueDeserializer::readVector3()
{
    return _deserialize().asVector3();
}

Vector3 DataValueDeserializer::readVector3(const char* name)
{
    return _deserialize(name).asVector3();
}

Vector4 DataValueDeserializer::readVector4()
{
    return _deserialize().asVector4();
}

Vector4 DataValueDeserializer::readVector4(const char* name)
{
    return _deserialize(name).asVector4();
}

const DataValue& DataValueDeserializer::_deserialize()
{
    DataValue& top = _valueStack.top();
    assert(top.isArray());
    return top[_indexStack.top()++];
}

const DataValue& DataValueDeserializer::_deserialize(const char* name)
{
    DataValue& top = _valueStack.top();
    assert(top.isObject());
    return top[name];
}

BinaryDeserializer::BinaryDeserializer(ReadStream& stream) :
    _stream(&stream)
{
}

bool BinaryDeserializer::isHumanReadable() const
{
    return false;
}

ArrayDeserializer BinaryDeserializer::readArray()
{
    _countStack.push(_stream->readUnsignedInt());
    _indexStack.push(0);

    return ArrayDeserializer(this);
}

ObjectDeserializer BinaryDeserializer::readObject()
{
    return ObjectDeserializer(this);
}

bool BinaryDeserializer::beginArray()
{
    ++_indexStack.top();

    _countStack.push(_stream->readUnsignedInt());
    _indexStack.push(0);
    return true;
}

bool BinaryDeserializer::beginArray(const char* name)
{
    name;

    _countStack.push(_stream->readUnsignedInt());
    _indexStack.push(0);
    return true;
}

void BinaryDeserializer::endArray()
{
    _indexStack.pop();
    _countStack.pop();
}

bool BinaryDeserializer::hasMoreElements() const
{
    return _indexStack.top() < _countStack.top();
}

bool BinaryDeserializer::beginObject()
{
    ++_indexStack.top();
    return true;
}

bool BinaryDeserializer::beginObject(const char* name)
{
    name;
    return true;
}

void BinaryDeserializer::endObject()
{
}

bool BinaryDeserializer::hasMember(const char* name) const
{
    name;
    return true; // Assume that all values are written
}

std::string BinaryDeserializer::readString()
{
    ++_indexStack.top();
    return _stream->readString();
}

std::string BinaryDeserializer::readString(const char* name)
{
    name;
    return _stream->readString();
}

int8_t BinaryDeserializer::readByte()
{
    ++_indexStack.top();
    return _stream->readByte();
}

int8_t BinaryDeserializer::readByte(const char* name)
{
    name;
    return _stream->readByte();
}

uint8_t BinaryDeserializer::readUnsignedByte()
{
    ++_indexStack.top();
    return _stream->readUnsignedByte();
}

uint8_t BinaryDeserializer::readUnsignedByte(const char* name)
{
    name;
    return _stream->readUnsignedByte();
}

int16_t BinaryDeserializer::readShort()
{
    ++_indexStack.top();
    return _stream->readShort();
}

int16_t BinaryDeserializer::readShort(const char* name)
{
    name;
    return _stream->readShort();
}

uint16_t BinaryDeserializer::readUnsignedShort()
{
    ++_indexStack.top();
    return _stream->readUnsignedShort();
}

uint16_t BinaryDeserializer::readUnsignedShort(const char* name)
{
    name;
    return _stream->readUnsignedShort();
}

int32_t BinaryDeserializer::readInt()
{
    ++_indexStack.top();
    return _stream->readInt();
}

int32_t BinaryDeserializer::readInt(const char* name)
{
    name;
    return _stream->readInt();
}

uint32_t BinaryDeserializer::readUnsignedInt()
{
    ++_indexStack.top();
    return _stream->readUnsignedInt();
}

uint32_t BinaryDeserializer::readUnsignedInt(const char* name)
{
    name;
    return _stream->readUnsignedInt();
}

int64_t BinaryDeserializer::readLong()
{
    ++_indexStack.top();
    return _stream->readLong();
}

int64_t BinaryDeserializer::readLong(const char* name)
{
    name;
    return _stream->readLong();
}

uint64_t BinaryDeserializer::readUnsignedLong()
{
    ++_indexStack.top();
    return _stream->readUnsignedLong();
}

uint64_t BinaryDeserializer::readUnsignedLong(const char* name)
{
    name;
    return _stream->readUnsignedLong();
}

float BinaryDeserializer::readFloat()
{
    ++_indexStack.top();
    return _stream->readFloat();
}

float BinaryDeserializer::readFloat(const char* name)
{
    name;
    return _stream->readFloat();
}

double BinaryDeserializer::readDouble()
{
    ++_indexStack.top();
    return _stream->readDouble();
}

double BinaryDeserializer::readDouble(const char* name)
{
    name;
    return _stream->readDouble();
}

Real BinaryDeserializer::readReal()
{
    ++_indexStack.top();
    return _stream->readReal();
}

Real BinaryDeserializer::readReal(const char* name)
{
    name;
    return _stream->readReal();
}

Vector2 BinaryDeserializer::readVector2()
{
    ++_indexStack.top();
    return _stream->readVector2();
}

Vector2 BinaryDeserializer::readVector2(const char* name)
{
    name;
    return _stream->readVector2();
}

Vector3 BinaryDeserializer::readVector3()
{
    ++_indexStack.top();
    return _stream->readVector3();
}

Vector3 BinaryDeserializer::readVector3(const char* name)
{
    name;
    return _stream->readVector3();
}

Vector4 BinaryDeserializer::readVector4()
{
    ++_indexStack.top();
    return _stream->readVector4();
}

Vector4 BinaryDeserializer::readVector4(const char* name)
{
    name;
    return _stream->readVector4();
}
