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
#include "DataDecoder.h"

#include "Hect/Core/Format.h"

using namespace hect;

ArrayDecoder::ArrayDecoder(const ArrayDecoder& decoder) :
    _decoder(nullptr)
{
    decoder;
}

ArrayDecoder::ArrayDecoder(ArrayDecoder&& decoder) :
    _decoder(decoder._decoder)
{
    decoder._decoder = nullptr;
}

ArrayDecoder::~ArrayDecoder()
{
    if (_decoder)
    {
        _decoder->endArray();
    }
}

bool ArrayDecoder::isHumanReadable() const
{
    assert(_decoder);
    return _decoder->isHumanReadable();
}

bool ArrayDecoder::hasMoreElements() const
{
    assert(_decoder);
    return _decoder->hasMoreElements();
}

ArrayDecoder ArrayDecoder::decodeArray()
{
    assert(_decoder);
    _decoder->beginArray();
    return ArrayDecoder(_decoder);
}

ObjectDecoder ArrayDecoder::decodeObject()
{
    assert(_decoder);
    _decoder->beginObject();
    return ObjectDecoder(_decoder);
}

std::string ArrayDecoder::decodeString()
{
    assert(_decoder);
    return _decoder->decodeString();
}

int8_t ArrayDecoder::decodeByte()
{
    assert(_decoder);
    return _decoder->decodeByte();
}

uint8_t ArrayDecoder::decodeUnsignedByte()
{
    assert(_decoder);
    return _decoder->decodeUnsignedByte();
}

int16_t ArrayDecoder::decodeShort()
{
    assert(_decoder);
    return _decoder->decodeShort();
}

uint16_t ArrayDecoder::decodeUnsignedShort()
{
    assert(_decoder);
    return _decoder->decodeUnsignedShort();
}

int32_t ArrayDecoder::decodeInt()
{
    assert(_decoder);
    return _decoder->decodeInt();
}

uint32_t ArrayDecoder::decodeUnsignedInt()
{
    assert(_decoder);
    return _decoder->decodeUnsignedInt();
}

int64_t ArrayDecoder::decodeLong()
{
    assert(_decoder);
    return _decoder->decodeLong();
}

uint64_t ArrayDecoder::decodeUnsignedLong()
{
    assert(_decoder);
    return _decoder->decodeUnsignedLong();
}

float ArrayDecoder::decodeFloat()
{
    assert(_decoder);
    return _decoder->decodeFloat();
}

double ArrayDecoder::decodeDouble()
{
    assert(_decoder);
    return _decoder->decodeDouble();
}

Real ArrayDecoder::decodeReal()
{
    assert(_decoder);
    return _decoder->decodeReal();
}

bool ArrayDecoder::decodeBool()
{
    assert(_decoder);
    return _decoder->decodeBool();
}

Vector2 ArrayDecoder::decodeVector2()
{
    assert(_decoder);
    return _decoder->decodeVector2();
}

Vector3 ArrayDecoder::decodeVector3()
{
    assert(_decoder);
    return _decoder->decodeVector3();
}

Vector4 ArrayDecoder::decodeVector4()
{
    assert(_decoder);
    return _decoder->decodeVector4();
}

ArrayDecoder::ArrayDecoder() :
    _decoder(nullptr)
{
}

ArrayDecoder::ArrayDecoder(DataDecoder* decoder) :
    _decoder(decoder)
{
}

ObjectDecoder::ObjectDecoder(const ObjectDecoder& decoder) :
    _decoder(nullptr)
{
    decoder;
}

ObjectDecoder::ObjectDecoder(ObjectDecoder&& decoder) :
    _decoder(decoder._decoder)
{
    decoder._decoder = nullptr;
}

ObjectDecoder::~ObjectDecoder()
{
    if (_decoder)
    {
        _decoder->endObject();
    }
}

bool ObjectDecoder::isHumanReadable() const
{
    assert(_decoder);
    return _decoder->isHumanReadable();
}

bool ObjectDecoder::hasMember(const char* name) const
{
    return _decoder->hasMember(name);
}

ArrayDecoder ObjectDecoder::decodeArray(const char* name)
{
    assert(_decoder);
    _decoder->beginArray(name);
    return ArrayDecoder(_decoder);
}

ObjectDecoder ObjectDecoder::decodeObject(const char* name)
{
    assert(_decoder);
    _decoder->beginObject(name);
    return ObjectDecoder(_decoder);
}

std::string ObjectDecoder::decodeString(const char* name)
{
    assert(_decoder);
    return _decoder->decodeString(name);
}

int8_t ObjectDecoder::decodeByte(const char* name)
{
    assert(_decoder);
    return _decoder->decodeByte(name);
}

uint8_t ObjectDecoder::decodeUnsignedByte(const char* name)
{
    assert(_decoder);
    return _decoder->decodeUnsignedByte(name);
}

int16_t ObjectDecoder::decodeShort(const char* name)
{
    assert(_decoder);
    return _decoder->decodeShort(name);
}

uint16_t ObjectDecoder::decodeUnsignedShort(const char* name)
{
    assert(_decoder);
    return _decoder->decodeUnsignedShort(name);
}

int32_t ObjectDecoder::decodeInt(const char* name)
{
    assert(_decoder);
    return _decoder->decodeInt(name);
}

uint32_t ObjectDecoder::decodeUnsignedInt(const char* name)
{
    assert(_decoder);
    return _decoder->decodeUnsignedInt(name);
}

int64_t ObjectDecoder::decodeLong(const char* name)
{
    assert(_decoder);
    return _decoder->decodeLong(name);
}

uint64_t ObjectDecoder::decodeUnsignedLong(const char* name)
{
    assert(_decoder);
    return _decoder->decodeUnsignedLong(name);
}

float ObjectDecoder::decodeFloat(const char* name)
{
    assert(_decoder);
    return _decoder->decodeFloat(name);
}

double ObjectDecoder::decodeDouble(const char* name)
{
    assert(_decoder);
    return _decoder->decodeDouble(name);
}

Real ObjectDecoder::decodeReal(const char* name)
{
    assert(_decoder);
    return _decoder->decodeReal(name);
}

bool ObjectDecoder::decodeBool(const char* name)
{
    assert(_decoder);
    return _decoder->decodeBool(name);
}

Vector2 ObjectDecoder::decodeVector2(const char* name)
{
    assert(_decoder);
    return _decoder->decodeVector2(name);
}

Vector3 ObjectDecoder::decodeVector3(const char* name)
{
    assert(_decoder);
    return _decoder->decodeVector3(name);
}

Vector4 ObjectDecoder::decodeVector4(const char* name)
{
    assert(_decoder);
    return _decoder->decodeVector4(name);
}

ObjectDecoder::ObjectDecoder() :
    _decoder(nullptr)
{
}

ObjectDecoder::ObjectDecoder(DataDecoder* decoder) :
    _decoder(decoder)
{
}

DataValueDecoder::DataValueDecoder(const DataValue& dataValue)
{
    _valueStack.push(dataValue);
}

bool DataValueDecoder::isHumanReadable() const
{
    return true;
}

ArrayDecoder DataValueDecoder::decodeArray()
{
    if (_valueStack.top().isArray())
    {
        _indexStack.push(0);
        return ArrayDecoder(this);
    }
    else
    {
        throw Error("The root value is not an array");
    }
}

ObjectDecoder DataValueDecoder::decodeObject()
{
    if (_valueStack.top().isObject())
    {
        return ObjectDecoder(this);
    }
    else
    {
        throw Error("The root value is not an object");
    }
}

void DataValueDecoder::beginArray()
{
    DataValue value = _decode();
    if (value.isArray())
    {
        _indexStack.push(0);
        _valueStack.push(value);
    }
    else
    {
        throw Error("The next value is not an array");
    }
}

void DataValueDecoder::beginArray(const char* name)
{
    DataValue& top = _valueStack.top();
    assert(top.isObject());

    const DataValue& value = top[name];
    if (!value.isNull())
    {
        _indexStack.push(0);
        _valueStack.push(value);
    }
    else
    {
        throw Error(format("No member value '%s'", name));
    }
}

void DataValueDecoder::endArray()
{
    const DataValue& top = _valueStack.top();
    assert(top.isArray());
    _indexStack.pop();
    _valueStack.pop();
}

bool DataValueDecoder::hasMoreElements() const
{
    const DataValue& top = _valueStack.top();
    assert(top.isArray());
    return _indexStack.top() < top.size();
}

void DataValueDecoder::beginObject()
{
    DataValue& top = _valueStack.top();
    assert(top.isArray());

    DataValue value = _decode();
    if (value.isObject())
    {
        _valueStack.push(value);
    }
    else
    {
        throw Error("The next value is not an object");
    }
}

void DataValueDecoder::beginObject(const char* name)
{
    DataValue& top = _valueStack.top();
    assert(top.isObject());

    const DataValue& value = top[name];
    if (!value.isNull())
    {
        _valueStack.push(value);
    }
    else
    {
        throw Error(format("No member value '%s'", name));
    }
}

void DataValueDecoder::endObject()
{
    const DataValue& top = _valueStack.top();
    assert(top.isObject());
    _valueStack.pop();
}

bool DataValueDecoder::hasMember(const char* name) const
{
    const DataValue& top = _valueStack.top();
    assert(top.isObject());
    return !top[name].isNull();
}

std::string DataValueDecoder::decodeString()
{
    return _decode().asString();
}

std::string DataValueDecoder::decodeString(const char* name)
{
    return _decode(name).asString();
}

int8_t DataValueDecoder::decodeByte()
{
    return (int8_t)_decode().asDouble();
}

int8_t DataValueDecoder::decodeByte(const char* name)
{
    return (int8_t)_decode(name).asDouble();
}

uint8_t DataValueDecoder::decodeUnsignedByte()
{
    return (uint8_t)_decode().asDouble();
}

uint8_t DataValueDecoder::decodeUnsignedByte(const char* name)
{
    return (uint8_t)_decode(name).asDouble();
}

int16_t DataValueDecoder::decodeShort()
{
    return (int16_t)_decode().asDouble();
}

int16_t DataValueDecoder::decodeShort(const char* name)
{
    return (int16_t)_decode(name).asDouble();
}

uint16_t DataValueDecoder::decodeUnsignedShort()
{
    return (uint16_t)_decode().asDouble();
}

uint16_t DataValueDecoder::decodeUnsignedShort(const char* name)
{
    return (uint16_t)_decode(name).asDouble();
}

int32_t DataValueDecoder::decodeInt()
{
    return (int32_t)_decode().asDouble();
}

int32_t DataValueDecoder::decodeInt(const char* name)
{
    return (int32_t)_decode(name).asDouble();
}

uint32_t DataValueDecoder::decodeUnsignedInt()
{
    return (uint32_t)_decode().asDouble();
}

uint32_t DataValueDecoder::decodeUnsignedInt(const char* name)
{
    return (uint32_t)_decode(name).asDouble();
}

int64_t DataValueDecoder::decodeLong()
{
    return (int64_t)_decode().asDouble();
}

int64_t DataValueDecoder::decodeLong(const char* name)
{
    return (int64_t)_decode(name).asDouble();
}

uint64_t DataValueDecoder::decodeUnsignedLong()
{
    return (uint64_t)_decode().asDouble();
}

uint64_t DataValueDecoder::decodeUnsignedLong(const char* name)
{
    return (uint64_t)_decode(name).asDouble();
}

float DataValueDecoder::decodeFloat()
{
    return (float)_decode().asDouble();
}

float DataValueDecoder::decodeFloat(const char* name)
{
    return (float)_decode(name).asDouble();
}

double DataValueDecoder::decodeDouble()
{
    return _decode().asDouble();
}

double DataValueDecoder::decodeDouble(const char* name)
{
    return _decode(name).asDouble();
}

Real DataValueDecoder::decodeReal()
{
    return (Real)_decode().asDouble();
}

Real DataValueDecoder::decodeReal(const char* name)
{
    return (Real)_decode(name).asDouble();
}

bool DataValueDecoder::decodeBool()
{
    return _decode().asBool();
}

bool DataValueDecoder::decodeBool(const char* name)
{
    return _decode(name).asBool();
}

Vector2 DataValueDecoder::decodeVector2()
{
    return _decode().asVector2();
}

Vector2 DataValueDecoder::decodeVector2(const char* name)
{
    return _decode(name).asVector2();
}

Vector3 DataValueDecoder::decodeVector3()
{
    return _decode().asVector3();
}

Vector3 DataValueDecoder::decodeVector3(const char* name)
{
    return _decode(name).asVector3();
}

Vector4 DataValueDecoder::decodeVector4()
{
    return _decode().asVector4();
}

Vector4 DataValueDecoder::decodeVector4(const char* name)
{
    return _decode(name).asVector4();
}

const DataValue& DataValueDecoder::_decode()
{
    DataValue& top = _valueStack.top();
    assert(top.isArray());
    return top[_indexStack.top()++];
}

const DataValue& DataValueDecoder::_decode(const char* name)
{
    DataValue& top = _valueStack.top();
    assert(top.isObject());

    const DataValue& result = top[name];
    if (result.isNull())
    {
        throw Error(format("No member value '%s'", name));
    }

    return result;
}

BinaryDecoder::BinaryDecoder(ReadStream& stream) :
    _stream(&stream)
{
}

bool BinaryDecoder::isHumanReadable() const
{
    return false;
}

ArrayDecoder BinaryDecoder::decodeArray()
{
    _countStack.push(_stream->readUnsignedInt());
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

    _countStack.push(_stream->readUnsignedInt());
    _indexStack.push(0);
}

void BinaryDecoder::beginArray(const char* name)
{
    name;

    _countStack.push(_stream->readUnsignedInt());
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
