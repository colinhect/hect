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
#include "DataEncoder.h"

using namespace hect;

ArrayEncoder::ArrayEncoder(const ArrayEncoder& encoder) :
    _encoder(nullptr)
{
    encoder;
}

ArrayEncoder::ArrayEncoder(ArrayEncoder&& encoder) :
    _encoder(encoder._encoder)
{
    encoder._encoder = nullptr;
}

ArrayEncoder::~ArrayEncoder()
{
    if (_encoder)
    {
        _encoder->endArray();
    }
}

bool ArrayEncoder::isHumanReadable() const
{
    assert(_encoder);
    return _encoder->isHumanReadable();
}

ArrayEncoder ArrayEncoder::encodeArray()
{
    assert(_encoder);
    _encoder->beginArray();
    return ArrayEncoder(_encoder);
}

ObjectEncoder ArrayEncoder::encodeObject()
{
    assert(_encoder);
    _encoder->beginObject();
    return ObjectEncoder(_encoder);
}

void ArrayEncoder::encodeString(const std::string& value)
{
    assert(_encoder);
    _encoder->encodeString(value);
}

void ArrayEncoder::encodeByte(int8_t value)
{
    assert(_encoder);
    _encoder->encodeByte(value);
}

void ArrayEncoder::encodeUnsignedByte(uint8_t value)
{
    assert(_encoder);
    _encoder->encodeUnsignedByte(value);
}

void ArrayEncoder::encodeShort(int16_t value)
{
    assert(_encoder);
    _encoder->encodeShort(value);
}

void ArrayEncoder::encodeUnsignedShort(uint16_t value)
{
    assert(_encoder);
    _encoder->encodeUnsignedShort(value);
}

void ArrayEncoder::encodeInt(int32_t value)
{
    assert(_encoder);
    _encoder->encodeInt(value);
}

void ArrayEncoder::encodeUnsignedInt(uint32_t value)
{
    assert(_encoder);
    _encoder->encodeUnsignedInt(value);
}

void ArrayEncoder::encodeLong(int64_t value)
{
    assert(_encoder);
    _encoder->encodeLong(value);
}

void ArrayEncoder::encodeUnsignedLong(uint64_t value)
{
    assert(_encoder);
    _encoder->encodeUnsignedLong(value);
}

void ArrayEncoder::encodeFloat(float value)
{
    assert(_encoder);
    _encoder->encodeFloat(value);
}

void ArrayEncoder::encodeDouble(double value)
{
    assert(_encoder);
    _encoder->encodeDouble(value);
}

void ArrayEncoder::encodeReal(Real value)
{
    assert(_encoder);
    _encoder->encodeReal(value);
}

void ArrayEncoder::encodeBool(bool value)
{
    assert(_encoder);
    _encoder->encodeBool(value);
}

void ArrayEncoder::encodeVector2(const Vector2& value)
{
    assert(_encoder);
    _encoder->encodeVector2(value);
}

void ArrayEncoder::encodeVector3(const Vector3& value)
{
    assert(_encoder);
    _encoder->encodeVector3(value);
}

void ArrayEncoder::encodeVector4(const Vector4& value)
{
    assert(_encoder);
    _encoder->encodeVector4(value);
}

ArrayEncoder::ArrayEncoder() :
    _encoder(nullptr)
{
}

ArrayEncoder::ArrayEncoder(DataEncoder* encoder) :
    _encoder(encoder)
{
}

ObjectEncoder::ObjectEncoder(const ObjectEncoder& encoder) :
    _encoder(nullptr)
{
    encoder;
}

ObjectEncoder::ObjectEncoder(ObjectEncoder&& encoder) :
    _encoder(encoder._encoder)
{
    encoder._encoder = nullptr;
}

ObjectEncoder::~ObjectEncoder()
{
    if (_encoder)
    {
        _encoder->endObject();
    }
}

bool ObjectEncoder::isHumanReadable() const
{
    assert(_encoder);
    return _encoder->isHumanReadable();
}

ArrayEncoder ObjectEncoder::encodeArray(const char* name)
{
    assert(_encoder);
    _encoder->beginArray(name);
    return ArrayEncoder(_encoder);
}

ObjectEncoder ObjectEncoder::encodeObject(const char* name)
{
    assert(_encoder);
    _encoder->beginObject(name);
    return ObjectEncoder(_encoder);
}

void ObjectEncoder::encodeString(const char* name, const std::string& value)
{
    assert(_encoder);
    _encoder->encodeString(name, value);
}

void ObjectEncoder::encodeByte(const char* name, int8_t value)
{
    assert(_encoder);
    _encoder->encodeByte(name, value);
}

void ObjectEncoder::encodeUnsignedByte(const char* name, uint8_t value)
{
    assert(_encoder);
    _encoder->encodeUnsignedByte(name, value);
}

void ObjectEncoder::encodeShort(const char* name, int16_t value)
{
    assert(_encoder);
    _encoder->encodeShort(name, value);
}

void ObjectEncoder::encodeUnsignedShort(const char* name, uint16_t value)
{
    assert(_encoder);
    _encoder->encodeUnsignedShort(name, value);
}

void ObjectEncoder::encodeInt(const char* name, int32_t value)
{
    assert(_encoder);
    _encoder->encodeInt(name, value);
}

void ObjectEncoder::encodeUnsignedInt(const char* name, uint32_t value)
{
    assert(_encoder);
    _encoder->encodeUnsignedInt(name, value);
}

void ObjectEncoder::encodeLong(const char* name, int64_t value)
{
    assert(_encoder);
    _encoder->encodeLong(name, value);
}

void ObjectEncoder::encodeUnsignedLong(const char* name, uint64_t value)
{
    assert(_encoder);
    _encoder->encodeUnsignedLong(name, value);
}

void ObjectEncoder::encodeFloat(const char* name, float value)
{
    assert(_encoder);
    _encoder->encodeFloat(name, value);
}

void ObjectEncoder::encodeDouble(const char* name, double value)
{
    assert(_encoder);
    _encoder->encodeDouble(name, value);
}

void ObjectEncoder::encodeReal(const char* name, Real value)
{
    assert(_encoder);
    _encoder->encodeReal(name, value);
}

void ObjectEncoder::encodeBool(const char* name, bool value)
{
    assert(_encoder);
    _encoder->encodeBool(name, value);
}

void ObjectEncoder::encodeVector2(const char* name, const Vector2& value)
{
    assert(_encoder);
    _encoder->encodeVector2(name, value);
}

void ObjectEncoder::encodeVector3(const char* name, const Vector3& value)
{
    assert(_encoder);
    _encoder->encodeVector3(name, value);
}

void ObjectEncoder::encodeVector4(const char* name, const Vector4& value)
{
    assert(_encoder);
    _encoder->encodeVector4(name, value);
}

ObjectEncoder::ObjectEncoder() :
    _encoder(nullptr)
{
}

ObjectEncoder::ObjectEncoder(DataEncoder* encoder) :
    _encoder(encoder)
{
}

bool DataValueEncoder::isHumanReadable() const
{
    return true;
}

ArrayEncoder DataValueEncoder::encodeArray()
{
    if (!_valueStack.empty())
    {
        throw Error("Current encoding object has not ended");
    }

    _valueStack.push(DataValue(DataValueType::Array));
    return ArrayEncoder(this);
}

ObjectEncoder DataValueEncoder::encodeObject()
{
    if (!_valueStack.empty())
    {
        throw Error("Current encoding object has not ended");
    }

    _valueStack.push(DataValue(DataValueType::Object));
    return ObjectEncoder(this);
}

DataValue::Array& DataValueEncoder::dataValues()
{
    return _completed;
}

void DataValueEncoder::beginArray()
{
    assert(_valueStack.top().isArray());
    _valueStack.push(DataValue(DataValueType::Array));
}

void DataValueEncoder::beginArray(const char* name)
{
    assert(_valueStack.top().isObject());
    _nameStack.push(name);
    _valueStack.push(DataValue(DataValueType::Array));
}

void DataValueEncoder::endArray()
{
    endObject();
}

void DataValueEncoder::beginObject()
{
    assert(_valueStack.top().isArray());
    _valueStack.push(DataValue(DataValueType::Object));
}

void DataValueEncoder::beginObject(const char* name)
{
    assert(_valueStack.top().isObject());
    _nameStack.push(name);
    _valueStack.push(DataValue(DataValueType::Object));
}

void DataValueEncoder::endObject()
{
    DataValue value = _valueStack.top();
    _valueStack.pop();

    if (_valueStack.empty())
    {
        _completed.push_back(value);
    }
    else if (_valueStack.top().isArray())
    {
        _valueStack.top().addElement(value);
    }
    else if (_valueStack.top().isObject())
    {
        _valueStack.top().addMember(_nameStack.top(), value);
        _nameStack.pop();
    }
}

void DataValueEncoder::encodeString(const std::string& value)
{
    _encode(value);
}

void DataValueEncoder::encodeString(const char* name, const std::string& value)
{
    _encode(name, value);
}

void DataValueEncoder::encodeByte(int8_t value)
{
    _encode(value);
}

void DataValueEncoder::encodeByte(const char* name, int8_t value)
{
    _encode(name, value);
}

void DataValueEncoder::encodeUnsignedByte(uint8_t value)
{
    _encode(value);
}

void DataValueEncoder::encodeUnsignedByte(const char* name, uint8_t value)
{
    _encode(name, value);
}

void DataValueEncoder::encodeShort(int16_t value)
{
    _encode(value);
}

void DataValueEncoder::encodeShort(const char* name, int16_t value)
{
    _encode(name, value);
}

void DataValueEncoder::encodeUnsignedShort(uint16_t value)
{
    _encode(value);
}

void DataValueEncoder::encodeUnsignedShort(const char* name, uint16_t value)
{
    _encode(name, value);
}

void DataValueEncoder::encodeInt(int32_t value)
{
    _encode(value);
}

void DataValueEncoder::encodeInt(const char* name, int32_t value)
{
    _encode(name, value);
}

void DataValueEncoder::encodeUnsignedInt(uint32_t value)
{
    _encode(value);
}

void DataValueEncoder::encodeUnsignedInt(const char* name, uint32_t value)
{
    _encode(name, value);
}

void DataValueEncoder::encodeLong(int64_t value)
{
    _encode((double)value);
}

void DataValueEncoder::encodeLong(const char* name, int64_t value)
{
    _encode(name, (double)value);
}

void DataValueEncoder::encodeUnsignedLong(uint64_t value)
{
    _encode((double)value);
}

void DataValueEncoder::encodeUnsignedLong(const char* name, uint64_t value)
{
    _encode(name, (double)value);
}

void DataValueEncoder::encodeFloat(float value)
{
    _encode(value);
}

void DataValueEncoder::encodeFloat(const char* name, float value)
{
    _encode(name, value);
}

void DataValueEncoder::encodeDouble(double value)
{
    _encode(value);
}

void DataValueEncoder::encodeDouble(const char* name, double value)
{
    _encode(name, value);
}

void DataValueEncoder::encodeReal(Real value)
{
    _encode(value);
}

void DataValueEncoder::encodeReal(const char* name, Real value)
{
    _encode(name, value);
}

void DataValueEncoder::encodeBool(bool value)
{
    _encode(value);
}

void DataValueEncoder::encodeBool(const char* name, bool value)
{
    _encode(name, value);
}

void DataValueEncoder::encodeVector2(const Vector2& value)
{
    _encode(value);
}

void DataValueEncoder::encodeVector2(const char* name, const Vector2& value)
{
    _encode(name, value);
}

void DataValueEncoder::encodeVector3(const Vector3& value)
{
    _encode(value);
}

void DataValueEncoder::encodeVector3(const char* name, const Vector3& value)
{
    _encode(name, value);
}

void DataValueEncoder::encodeVector4(const Vector4& value)
{
    _encode(value);
}

void DataValueEncoder::encodeVector4(const char* name, const Vector4& value)
{
    _encode(name, value);
}

void DataValueEncoder::_encode(const DataValue& value)
{
    DataValue& top = _valueStack.top();
    assert(top.isArray());
    top.addElement(value);
}

void DataValueEncoder::_encode(const char* name, const DataValue& value)
{
    DataValue& top = _valueStack.top();
    assert(top.isObject());
    top.addMember(name, value);
}

BinaryEncoder::BinaryEncoder(WriteStream& stream) :
    _stream(&stream)
{
}

bool BinaryEncoder::isHumanReadable() const
{
    return false;
}

ArrayEncoder BinaryEncoder::encodeArray()
{
    _countStack.push(0);
    _countPositionStack.push(_stream->position());
    _stream->writeUnsignedInt(0);
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
    _stream->writeUnsignedInt(0);
}

void BinaryEncoder::beginArray(const char* name)
{
    name;
    _countStack.push(0);
    _countPositionStack.push(_stream->position());
    _stream->writeUnsignedInt(0);
}

void BinaryEncoder::endArray()
{
    size_t currentPosition = _stream->position();
    _stream->seek(_countPositionStack.top());
    _stream->writeUnsignedInt(_countStack.top());
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

void BinaryEncoder::encodeByte(int8_t value)
{
    ++_countStack.top();
    _stream->writeByte(value);
}

void BinaryEncoder::encodeByte(const char* name, int8_t value)
{
    name;
    _stream->writeByte(value);
}

void BinaryEncoder::encodeUnsignedByte(uint8_t value)
{
    ++_countStack.top();
    _stream->writeUnsignedByte(value);
}

void BinaryEncoder::encodeUnsignedByte(const char* name, uint8_t value)
{
    name;
    _stream->writeUnsignedByte(value);
}

void BinaryEncoder::encodeShort(int16_t value)
{
    ++_countStack.top();
    _stream->writeShort(value);
}

void BinaryEncoder::encodeShort(const char* name, int16_t value)
{
    name;
    _stream->writeShort(value);
}

void BinaryEncoder::encodeUnsignedShort(uint16_t value)
{
    ++_countStack.top();
    _stream->writeUnsignedShort(value);
}

void BinaryEncoder::encodeUnsignedShort(const char* name, uint16_t value)
{
    name;
    _stream->writeUnsignedShort(value);
}

void BinaryEncoder::encodeInt(int32_t value)
{
    ++_countStack.top();
    _stream->writeInt(value);
}

void BinaryEncoder::encodeInt(const char* name, int32_t value)
{
    name;
    _stream->writeInt(value);
}

void BinaryEncoder::encodeUnsignedInt(uint32_t value)
{
    ++_countStack.top();
    _stream->writeUnsignedInt(value);
}

void BinaryEncoder::encodeUnsignedInt(const char* name, uint32_t value)
{
    name;
    _stream->writeUnsignedInt(value);
}

void BinaryEncoder::encodeLong(int64_t value)
{
    ++_countStack.top();
    _stream->writeLong(value);
}

void BinaryEncoder::encodeLong(const char* name, int64_t value)
{
    name;
    _stream->writeLong(value);
}

void BinaryEncoder::encodeUnsignedLong(uint64_t value)
{
    ++_countStack.top();
    _stream->writeUnsignedLong(value);
}

void BinaryEncoder::encodeUnsignedLong(const char* name, uint64_t value)
{
    name;
    _stream->writeUnsignedLong(value);
}

void BinaryEncoder::encodeFloat(float value)
{
    ++_countStack.top();
    _stream->writeFloat(value);
}

void BinaryEncoder::encodeFloat(const char* name, float value)
{
    name;
    _stream->writeFloat(value);
}

void BinaryEncoder::encodeDouble(double value)
{
    ++_countStack.top();
    _stream->writeDouble(value);
}

void BinaryEncoder::encodeDouble(const char* name, double value)
{
    name;
    _stream->writeDouble(value);
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