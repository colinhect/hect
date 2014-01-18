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
#include "Serializer.h"

using namespace hect;

ArraySerializer::ArraySerializer(const ArraySerializer& serializer) :
    _serializer(nullptr)
{
    serializer;
}

ArraySerializer::ArraySerializer(ArraySerializer&& serializer) :
    _serializer(serializer._serializer)
{
    serializer._serializer = nullptr;
}

ArraySerializer::~ArraySerializer()
{
    if (_serializer)
    {
        _serializer->endArray();
    }
}

ArraySerializer ArraySerializer::writeArray()
{
    assert(_serializer);
    _serializer->beginArray();
    return ArraySerializer(_serializer);
}

ObjectSerializer ArraySerializer::writeObject()
{
    assert(_serializer);
    _serializer->beginObject();
    return ObjectSerializer(_serializer);
}

void ArraySerializer::writeString(const std::string& value)
{
    assert(_serializer);
    _serializer->writeString(value);
}

void ArraySerializer::writeByte(int8_t value)
{
    assert(_serializer);
    _serializer->writeByte(value);
}

void ArraySerializer::writeUnsignedByte(uint8_t value)
{
    assert(_serializer);
    _serializer->writeUnsignedByte(value);
}

void ArraySerializer::writeShort(int16_t value)
{
    assert(_serializer);
    _serializer->writeShort(value);
}

void ArraySerializer::writeUnsignedShort(uint16_t value)
{
    assert(_serializer);
    _serializer->writeUnsignedShort(value);
}

void ArraySerializer::writeInt(int32_t value)
{
    assert(_serializer);
    _serializer->writeInt(value);
}

void ArraySerializer::writeUnsignedInt(uint32_t value)
{
    assert(_serializer);
    _serializer->writeUnsignedInt(value);
}

void ArraySerializer::writeLong(int64_t value)
{
    assert(_serializer);
    _serializer->writeLong(value);
}

void ArraySerializer::writeUnsignedLong(uint64_t value)
{
    assert(_serializer);
    _serializer->writeUnsignedLong(value);
}

void ArraySerializer::writeFloat(float value)
{
    assert(_serializer);
    _serializer->writeFloat(value);
}

void ArraySerializer::writeDouble(double value)
{
    assert(_serializer);
    _serializer->writeDouble(value);
}

void ArraySerializer::writeReal(Real value)
{
    assert(_serializer);
    _serializer->writeReal(value);
}

void ArraySerializer::writeVector2(const Vector2& value)
{
    assert(_serializer);
    _serializer->writeVector2(value);
}

void ArraySerializer::writeVector3(const Vector3& value)
{
    assert(_serializer);
    _serializer->writeVector3(value);
}

void ArraySerializer::writeVector4(const Vector4& value)
{
    assert(_serializer);
    _serializer->writeVector4(value);
}

ArraySerializer::ArraySerializer() :
    _serializer(nullptr)
{
}

ArraySerializer::ArraySerializer(Serializer* serializer) :
    _serializer(serializer)
{
}

ObjectSerializer::ObjectSerializer(const ObjectSerializer& serializer) :
    _serializer(nullptr)
{
    serializer;
}

ObjectSerializer::ObjectSerializer(ObjectSerializer&& serializer) :
    _serializer(serializer._serializer)
{
    serializer._serializer = nullptr;
}

ObjectSerializer::~ObjectSerializer()
{
    if (_serializer)
    {
        _serializer->endObject();
    }
}

ArraySerializer ObjectSerializer::writeArray(const char* name)
{
    assert(_serializer);
    _serializer->beginArray(name);
    return ArraySerializer(_serializer);
}

ObjectSerializer ObjectSerializer::writeObject(const char* name)
{
    assert(_serializer);
    _serializer->beginObject(name);
    return ObjectSerializer(_serializer);
}

void ObjectSerializer::writeString(const char* name, const std::string& value)
{
    assert(_serializer);
    _serializer->writeString(name, value);
}

void ObjectSerializer::writeByte(const char* name, int8_t value)
{
    assert(_serializer);
    _serializer->writeByte(name, value);
}

void ObjectSerializer::writeUnsignedByte(const char* name, uint8_t value)
{
    assert(_serializer);
    _serializer->writeUnsignedByte(name, value);
}

void ObjectSerializer::writeShort(const char* name, int16_t value)
{
    assert(_serializer);
    _serializer->writeShort(name, value);
}

void ObjectSerializer::writeUnsignedShort(const char* name, uint16_t value)
{
    assert(_serializer);
    _serializer->writeUnsignedShort(name, value);
}

void ObjectSerializer::writeInt(const char* name, int32_t value)
{
    assert(_serializer);
    _serializer->writeInt(name, value);
}

void ObjectSerializer::writeUnsignedInt(const char* name, uint32_t value)
{
    assert(_serializer);
    _serializer->writeUnsignedInt(name, value);
}

void ObjectSerializer::writeLong(const char* name, int64_t value)
{
    assert(_serializer);
    _serializer->writeLong(name, value);
}

void ObjectSerializer::writeUnsignedLong(const char* name, uint64_t value)
{
    assert(_serializer);
    _serializer->writeUnsignedLong(name, value);
}

void ObjectSerializer::writeFloat(const char* name, float value)
{
    assert(_serializer);
    _serializer->writeFloat(name, value);
}

void ObjectSerializer::writeDouble(const char* name, double value)
{
    assert(_serializer);
    _serializer->writeDouble(name, value);
}

void ObjectSerializer::writeReal(const char* name, Real value)
{
    assert(_serializer);
    _serializer->writeReal(name, value);
}

void ObjectSerializer::writeVector2(const char* name, const Vector2& value)
{
    assert(_serializer);
    _serializer->writeVector2(name, value);
}

void ObjectSerializer::writeVector3(const char* name, const Vector3& value)
{
    assert(_serializer);
    _serializer->writeVector3(name, value);
}

void ObjectSerializer::writeVector4(const char* name, const Vector4& value)
{
    assert(_serializer);
    _serializer->writeVector4(name, value);
}

ObjectSerializer::ObjectSerializer() :
    _serializer(nullptr)
{
}

ObjectSerializer::ObjectSerializer(Serializer* serializer) :
    _serializer(serializer)
{
}

bool DataValueSerializer::isHumanReadable() const
{
    return true;
}

ArraySerializer DataValueSerializer::writeArray()
{
    if (!_valueStack.empty())
    {
        throw Error("Current encoding object has not ended");
    }

    _valueStack.push(DataValue(DataValueType::Array));
    return ArraySerializer(this);
}

ObjectSerializer DataValueSerializer::writeObject()
{
    if (!_valueStack.empty())
    {
        throw Error("Current encoding object has not ended");
    }

    _valueStack.push(DataValue(DataValueType::Object));
    return ObjectSerializer(this);
}

DataValue::Array& DataValueSerializer::serializedDataValues()
{
    return _completed;
}

void DataValueSerializer::beginArray()
{
    assert(_valueStack.top().isArray());
    _valueStack.push(DataValue(DataValueType::Array));
}

void DataValueSerializer::beginArray(const char* name)
{
    assert(_valueStack.top().isObject());
    _nameStack.push(name);
    _valueStack.push(DataValue(DataValueType::Array));
}

void DataValueSerializer::endArray()
{
    endObject();
}

void DataValueSerializer::beginObject()
{
    assert(_valueStack.top().isArray());
    _valueStack.push(DataValue(DataValueType::Object));
}

void DataValueSerializer::beginObject(const char* name)
{
    assert(_valueStack.top().isObject());
    _nameStack.push(name);
    _valueStack.push(DataValue(DataValueType::Object));
}

void DataValueSerializer::endObject()
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

void DataValueSerializer::writeString(const std::string& value)
{
    _serialize(value);
}

void DataValueSerializer::writeString(const char* name, const std::string& value)
{
    _serialize(name, value);
}

void DataValueSerializer::writeByte(int8_t value)
{
    _serialize(value);
}

void DataValueSerializer::writeByte(const char* name, int8_t value)
{
    _serialize(name, value);
}

void DataValueSerializer::writeUnsignedByte(uint8_t value)
{
    _serialize(value);
}

void DataValueSerializer::writeUnsignedByte(const char* name, uint8_t value)
{
    _serialize(name, value);
}

void DataValueSerializer::writeShort(int16_t value)
{
    _serialize(value);
}

void DataValueSerializer::writeShort(const char* name, int16_t value)
{
    _serialize(name, value);
}

void DataValueSerializer::writeUnsignedShort(uint16_t value)
{
    _serialize(value);
}

void DataValueSerializer::writeUnsignedShort(const char* name, uint16_t value)
{
    _serialize(name, value);
}

void DataValueSerializer::writeInt(int32_t value)
{
    _serialize(value);
}

void DataValueSerializer::writeInt(const char* name, int32_t value)
{
    _serialize(name, value);
}

void DataValueSerializer::writeUnsignedInt(uint32_t value)
{
    _serialize(value);
}

void DataValueSerializer::writeUnsignedInt(const char* name, uint32_t value)
{
    _serialize(name, value);
}

void DataValueSerializer::writeLong(int64_t value)
{
    _serialize((double)value);
}

void DataValueSerializer::writeLong(const char* name, int64_t value)
{
    _serialize(name, (double)value);
}

void DataValueSerializer::writeUnsignedLong(uint64_t value)
{
    _serialize((double)value);
}

void DataValueSerializer::writeUnsignedLong(const char* name, uint64_t value)
{
    _serialize(name, (double)value);
}

void DataValueSerializer::writeFloat(float value)
{
    _serialize(value);
}

void DataValueSerializer::writeFloat(const char* name, float value)
{
    _serialize(name, value);
}

void DataValueSerializer::writeDouble(double value)
{
    _serialize(value);
}

void DataValueSerializer::writeDouble(const char* name, double value)
{
    _serialize(name, value);
}

void DataValueSerializer::writeReal(Real value)
{
    _serialize(value);
}

void DataValueSerializer::writeReal(const char* name, Real value)
{
    _serialize(name, value);
}

void DataValueSerializer::writeVector2(const Vector2& value)
{
    _serialize(value);
}

void DataValueSerializer::writeVector2(const char* name, const Vector2& value)
{
    _serialize(name, value);
}

void DataValueSerializer::writeVector3(const Vector3& value)
{
    _serialize(value);
}

void DataValueSerializer::writeVector3(const char* name, const Vector3& value)
{
    _serialize(name, value);
}

void DataValueSerializer::writeVector4(const Vector4& value)
{
    _serialize(value);
}

void DataValueSerializer::writeVector4(const char* name, const Vector4& value)
{
    _serialize(name, value);
}

void DataValueSerializer::_serialize(const DataValue& value)
{
    DataValue& top = _valueStack.top();
    assert(top.isArray());
    top.addElement(value);
}

void DataValueSerializer::_serialize(const char* name, const DataValue& value)
{
    DataValue& top = _valueStack.top();
    assert(top.isObject());
    top.addMember(name, value);
}

BinarySerializer::BinarySerializer(WriteStream& stream) :
    _stream(&stream)
{
}

bool BinarySerializer::isHumanReadable() const
{
    return false;
}

ArraySerializer BinarySerializer::writeArray()
{
    _countStack.push(0);
    _countPositionStack.push(_stream->position());
    _stream->writeUnsignedInt(0);
    return ArraySerializer(this);
}

ObjectSerializer BinarySerializer::writeObject()
{
    _countStack.push(0);
    return ObjectSerializer(this);
}

void BinarySerializer::beginArray()
{
    ++_countStack.top();
    _countStack.push(0);
    _countPositionStack.push(_stream->position());
    _stream->writeUnsignedInt(0);
}

void BinarySerializer::beginArray(const char* name)
{
    name;
    _countStack.push(0);
    _countPositionStack.push(_stream->position());
    _stream->writeUnsignedInt(0);
}

void BinarySerializer::endArray()
{
    size_t currentPosition = _stream->position();
    _stream->seek(_countPositionStack.top());
    _stream->writeUnsignedInt(_countStack.top());
    _stream->seek(currentPosition);

    _countPositionStack.pop();
    _countStack.pop();
}

void BinarySerializer::beginObject()
{
    ++_countStack.top();
}

void BinarySerializer::beginObject(const char* name)
{
    name;
}

void BinarySerializer::endObject()
{
}

void BinarySerializer::writeString(const std::string& value)
{
    ++_countStack.top();
    _stream->writeString(value);
}

void BinarySerializer::writeString(const char* name, const std::string& value)
{
    name;
    _stream->writeString(value);
}

void BinarySerializer::writeByte(int8_t value)
{
    ++_countStack.top();
    _stream->writeByte(value);
}

void BinarySerializer::writeByte(const char* name, int8_t value)
{
    name;
    _stream->writeByte(value);
}

void BinarySerializer::writeUnsignedByte(uint8_t value)
{
    ++_countStack.top();
    _stream->writeUnsignedByte(value);
}

void BinarySerializer::writeUnsignedByte(const char* name, uint8_t value)
{
    name;
    _stream->writeUnsignedByte(value);
}

void BinarySerializer::writeShort(int16_t value)
{
    ++_countStack.top();
    _stream->writeShort(value);
}

void BinarySerializer::writeShort(const char* name, int16_t value)
{
    name;
    _stream->writeShort(value);
}

void BinarySerializer::writeUnsignedShort(uint16_t value)
{
    ++_countStack.top();
    _stream->writeUnsignedShort(value);
}

void BinarySerializer::writeUnsignedShort(const char* name, uint16_t value)
{
    name;
    _stream->writeUnsignedShort(value);
}

void BinarySerializer::writeInt(int32_t value)
{
    ++_countStack.top();
    _stream->writeInt(value);
}

void BinarySerializer::writeInt(const char* name, int32_t value)
{
    name;
    _stream->writeInt(value);
}

void BinarySerializer::writeUnsignedInt(uint32_t value)
{
    ++_countStack.top();
    _stream->writeUnsignedInt(value);
}

void BinarySerializer::writeUnsignedInt(const char* name, uint32_t value)
{
    name;
    _stream->writeUnsignedInt(value);
}

void BinarySerializer::writeLong(int64_t value)
{
    ++_countStack.top();
    _stream->writeLong(value);
}

void BinarySerializer::writeLong(const char* name, int64_t value)
{
    name;
    _stream->writeLong(value);
}

void BinarySerializer::writeUnsignedLong(uint64_t value)
{
    ++_countStack.top();
    _stream->writeUnsignedLong(value);
}

void BinarySerializer::writeUnsignedLong(const char* name, uint64_t value)
{
    name;
    _stream->writeUnsignedLong(value);
}

void BinarySerializer::writeFloat(float value)
{
    ++_countStack.top();
    _stream->writeFloat(value);
}

void BinarySerializer::writeFloat(const char* name, float value)
{
    name;
    _stream->writeFloat(value);
}

void BinarySerializer::writeDouble(double value)
{
    ++_countStack.top();
    _stream->writeDouble(value);
}

void BinarySerializer::writeDouble(const char* name, double value)
{
    name;
    _stream->writeDouble(value);
}

void BinarySerializer::writeReal(Real value)
{
    ++_countStack.top();
    _stream->writeReal(value);
}

void BinarySerializer::writeReal(const char* name, Real value)
{
    name;
    _stream->writeReal(value);
}

void BinarySerializer::writeVector2(const Vector2& value)
{
    ++_countStack.top();
    _stream->writeVector2(value);
}

void BinarySerializer::writeVector2(const char* name, const Vector2& value)
{
    name;
    _stream->writeVector2(value);
}

void BinarySerializer::writeVector3(const Vector3& value)
{
    ++_countStack.top();
    _stream->writeVector3(value);
}

void BinarySerializer::writeVector3(const char* name, const Vector3& value)
{
    name;
    _stream->writeVector3(value);
}

void BinarySerializer::writeVector4(const Vector4& value)
{
    ++_countStack.top();
    _stream->writeVector4(value);
}

void BinarySerializer::writeVector4(const char* name, const Vector4& value)
{
    name;
    _stream->writeVector4(value);
}