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
#include "DataWriter.h"

using namespace hect;

ArrayWriter::ArrayWriter(const ArrayWriter& writer) :
    _writer(nullptr)
{
    writer;
}

ArrayWriter::ArrayWriter(ArrayWriter&& writer) :
    _writer(writer._writer)
{
    writer._writer = nullptr;
}

ArrayWriter::~ArrayWriter()
{
    if (_writer)
    {
        _writer->endArray();
    }
}

ArrayWriter ArrayWriter::writeArray()
{
    assert(_writer);
    _writer->beginArray();
    return ArrayWriter(_writer);
}

ObjectWriter ArrayWriter::writeObject()
{
    assert(_writer);
    _writer->beginObject();
    return ObjectWriter(_writer);
}

void ArrayWriter::writeString(const std::string& value)
{
    assert(_writer);
    _writer->writeString(value);
}

void ArrayWriter::writeByte(int8_t value)
{
    assert(_writer);
    _writer->writeByte(value);
}

void ArrayWriter::writeUnsignedByte(uint8_t value)
{
    assert(_writer);
    _writer->writeUnsignedByte(value);
}

void ArrayWriter::writeShort(int16_t value)
{
    assert(_writer);
    _writer->writeShort(value);
}

void ArrayWriter::writeUnsignedShort(uint16_t value)
{
    assert(_writer);
    _writer->writeUnsignedShort(value);
}

void ArrayWriter::writeInt(int32_t value)
{
    assert(_writer);
    _writer->writeInt(value);
}

void ArrayWriter::writeUnsignedInt(uint32_t value)
{
    assert(_writer);
    _writer->writeUnsignedInt(value);
}

void ArrayWriter::writeLong(int64_t value)
{
    assert(_writer);
    _writer->writeLong(value);
}

void ArrayWriter::writeUnsignedLong(uint64_t value)
{
    assert(_writer);
    _writer->writeUnsignedLong(value);
}

void ArrayWriter::writeFloat(float value)
{
    assert(_writer);
    _writer->writeFloat(value);
}

void ArrayWriter::writeDouble(double value)
{
    assert(_writer);
    _writer->writeDouble(value);
}

void ArrayWriter::writeReal(Real value)
{
    assert(_writer);
    _writer->writeReal(value);
}

void ArrayWriter::writeVector2(const Vector2& value)
{
    assert(_writer);
    _writer->writeVector2(value);
}

void ArrayWriter::writeVector3(const Vector3& value)
{
    assert(_writer);
    _writer->writeVector3(value);
}

void ArrayWriter::writeVector4(const Vector4& value)
{
    assert(_writer);
    _writer->writeVector4(value);
}

ArrayWriter::ArrayWriter() :
    _writer(nullptr)
{
}

ArrayWriter::ArrayWriter(DataWriter* writer) :
    _writer(writer)
{
}

ObjectWriter::ObjectWriter(const ObjectWriter& writer) :
    _writer(nullptr)
{
    writer;
}

ObjectWriter::ObjectWriter(ObjectWriter&& writer) :
    _writer(writer._writer)
{
    writer._writer = nullptr;
}

ObjectWriter::~ObjectWriter()
{
    if (_writer)
    {
        _writer->endObject();
    }
}

ArrayWriter ObjectWriter::writeArray(const char* name)
{
    assert(_writer);
    _writer->beginArray(name);
    return ArrayWriter(_writer);
}

ObjectWriter ObjectWriter::writeObject(const char* name)
{
    assert(_writer);
    _writer->beginObject(name);
    return ObjectWriter(_writer);
}

void ObjectWriter::writeString(const char* name, const std::string& value)
{
    assert(_writer);
    _writer->writeString(name, value);
}

void ObjectWriter::writeByte(const char* name, int8_t value)
{
    assert(_writer);
    _writer->writeByte(name, value);
}

void ObjectWriter::writeUnsignedByte(const char* name, uint8_t value)
{
    assert(_writer);
    _writer->writeUnsignedByte(name, value);
}

void ObjectWriter::writeShort(const char* name, int16_t value)
{
    assert(_writer);
    _writer->writeShort(name, value);
}

void ObjectWriter::writeUnsignedShort(const char* name, uint16_t value)
{
    assert(_writer);
    _writer->writeUnsignedShort(name, value);
}

void ObjectWriter::writeInt(const char* name, int32_t value)
{
    assert(_writer);
    _writer->writeInt(name, value);
}

void ObjectWriter::writeUnsignedInt(const char* name, uint32_t value)
{
    assert(_writer);
    _writer->writeUnsignedInt(name, value);
}

void ObjectWriter::writeLong(const char* name, int64_t value)
{
    assert(_writer);
    _writer->writeLong(name, value);
}

void ObjectWriter::writeUnsignedLong(const char* name, uint64_t value)
{
    assert(_writer);
    _writer->writeUnsignedLong(name, value);
}

void ObjectWriter::writeFloat(const char* name, float value)
{
    assert(_writer);
    _writer->writeFloat(name, value);
}

void ObjectWriter::writeDouble(const char* name, double value)
{
    assert(_writer);
    _writer->writeDouble(name, value);
}

void ObjectWriter::writeReal(const char* name, Real value)
{
    assert(_writer);
    _writer->writeReal(name, value);
}

void ObjectWriter::writeVector2(const char* name, const Vector2& value)
{
    assert(_writer);
    _writer->writeVector2(name, value);
}

void ObjectWriter::writeVector3(const char* name, const Vector3& value)
{
    assert(_writer);
    _writer->writeVector3(name, value);
}

void ObjectWriter::writeVector4(const char* name, const Vector4& value)
{
    assert(_writer);
    _writer->writeVector4(name, value);
}

ObjectWriter::ObjectWriter() :
    _writer(nullptr)
{
}

ObjectWriter::ObjectWriter(DataWriter* writer) :
    _writer(writer)
{
}

bool DataValueWriter::isHumanReadable() const
{
    return true;
}

ArrayWriter DataValueWriter::writeArray()
{
    if (!_valueStack.empty())
    {
        throw Error("Current encoding object has not ended");
    }

    _valueStack.push(DataValue(DataValueType::Array));
    return ArrayWriter(this);
}

ObjectWriter DataValueWriter::writeObject()
{
    if (!_valueStack.empty())
    {
        throw Error("Current encoding object has not ended");
    }

    _valueStack.push(DataValue(DataValueType::Object));
    return ObjectWriter(this);
}

DataValue::Array& DataValueWriter::serializedDataValues()
{
    return _completed;
}

void DataValueWriter::beginArray()
{
    assert(_valueStack.top().isArray());
    _valueStack.push(DataValue(DataValueType::Array));
}

void DataValueWriter::beginArray(const char* name)
{
    assert(_valueStack.top().isObject());
    _nameStack.push(name);
    _valueStack.push(DataValue(DataValueType::Array));
}

void DataValueWriter::endArray()
{
    endObject();
}

void DataValueWriter::beginObject()
{
    assert(_valueStack.top().isArray());
    _valueStack.push(DataValue(DataValueType::Object));
}

void DataValueWriter::beginObject(const char* name)
{
    assert(_valueStack.top().isObject());
    _nameStack.push(name);
    _valueStack.push(DataValue(DataValueType::Object));
}

void DataValueWriter::endObject()
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

void DataValueWriter::writeString(const std::string& value)
{
    _serialize(value);
}

void DataValueWriter::writeString(const char* name, const std::string& value)
{
    _serialize(name, value);
}

void DataValueWriter::writeByte(int8_t value)
{
    _serialize(value);
}

void DataValueWriter::writeByte(const char* name, int8_t value)
{
    _serialize(name, value);
}

void DataValueWriter::writeUnsignedByte(uint8_t value)
{
    _serialize(value);
}

void DataValueWriter::writeUnsignedByte(const char* name, uint8_t value)
{
    _serialize(name, value);
}

void DataValueWriter::writeShort(int16_t value)
{
    _serialize(value);
}

void DataValueWriter::writeShort(const char* name, int16_t value)
{
    _serialize(name, value);
}

void DataValueWriter::writeUnsignedShort(uint16_t value)
{
    _serialize(value);
}

void DataValueWriter::writeUnsignedShort(const char* name, uint16_t value)
{
    _serialize(name, value);
}

void DataValueWriter::writeInt(int32_t value)
{
    _serialize(value);
}

void DataValueWriter::writeInt(const char* name, int32_t value)
{
    _serialize(name, value);
}

void DataValueWriter::writeUnsignedInt(uint32_t value)
{
    _serialize(value);
}

void DataValueWriter::writeUnsignedInt(const char* name, uint32_t value)
{
    _serialize(name, value);
}

void DataValueWriter::writeLong(int64_t value)
{
    _serialize((double)value);
}

void DataValueWriter::writeLong(const char* name, int64_t value)
{
    _serialize(name, (double)value);
}

void DataValueWriter::writeUnsignedLong(uint64_t value)
{
    _serialize((double)value);
}

void DataValueWriter::writeUnsignedLong(const char* name, uint64_t value)
{
    _serialize(name, (double)value);
}

void DataValueWriter::writeFloat(float value)
{
    _serialize(value);
}

void DataValueWriter::writeFloat(const char* name, float value)
{
    _serialize(name, value);
}

void DataValueWriter::writeDouble(double value)
{
    _serialize(value);
}

void DataValueWriter::writeDouble(const char* name, double value)
{
    _serialize(name, value);
}

void DataValueWriter::writeReal(Real value)
{
    _serialize(value);
}

void DataValueWriter::writeReal(const char* name, Real value)
{
    _serialize(name, value);
}

void DataValueWriter::writeVector2(const Vector2& value)
{
    _serialize(value);
}

void DataValueWriter::writeVector2(const char* name, const Vector2& value)
{
    _serialize(name, value);
}

void DataValueWriter::writeVector3(const Vector3& value)
{
    _serialize(value);
}

void DataValueWriter::writeVector3(const char* name, const Vector3& value)
{
    _serialize(name, value);
}

void DataValueWriter::writeVector4(const Vector4& value)
{
    _serialize(value);
}

void DataValueWriter::writeVector4(const char* name, const Vector4& value)
{
    _serialize(name, value);
}

void DataValueWriter::_serialize(const DataValue& value)
{
    DataValue& top = _valueStack.top();
    assert(top.isArray());
    top.addElement(value);
}

void DataValueWriter::_serialize(const char* name, const DataValue& value)
{
    DataValue& top = _valueStack.top();
    assert(top.isObject());
    top.addMember(name, value);
}

BinaryWriter::BinaryWriter(WriteStream& stream) :
    _stream(&stream)
{
}

bool BinaryWriter::isHumanReadable() const
{
    return false;
}

ArrayWriter BinaryWriter::writeArray()
{
    _countStack.push(0);
    _countPositionStack.push(_stream->position());
    _stream->writeUnsignedInt(0);
    return ArrayWriter(this);
}

ObjectWriter BinaryWriter::writeObject()
{
    _countStack.push(0);
    return ObjectWriter(this);
}

void BinaryWriter::beginArray()
{
    ++_countStack.top();
    _countStack.push(0);
    _countPositionStack.push(_stream->position());
    _stream->writeUnsignedInt(0);
}

void BinaryWriter::beginArray(const char* name)
{
    name;
    _countStack.push(0);
    _countPositionStack.push(_stream->position());
    _stream->writeUnsignedInt(0);
}

void BinaryWriter::endArray()
{
    size_t currentPosition = _stream->position();
    _stream->seek(_countPositionStack.top());
    _stream->writeUnsignedInt(_countStack.top());
    _stream->seek(currentPosition);

    _countPositionStack.pop();
    _countStack.pop();
}

void BinaryWriter::beginObject()
{
    ++_countStack.top();
}

void BinaryWriter::beginObject(const char* name)
{
    name;
}

void BinaryWriter::endObject()
{
}

void BinaryWriter::writeString(const std::string& value)
{
    ++_countStack.top();
    _stream->writeString(value);
}

void BinaryWriter::writeString(const char* name, const std::string& value)
{
    name;
    _stream->writeString(value);
}

void BinaryWriter::writeByte(int8_t value)
{
    ++_countStack.top();
    _stream->writeByte(value);
}

void BinaryWriter::writeByte(const char* name, int8_t value)
{
    name;
    _stream->writeByte(value);
}

void BinaryWriter::writeUnsignedByte(uint8_t value)
{
    ++_countStack.top();
    _stream->writeUnsignedByte(value);
}

void BinaryWriter::writeUnsignedByte(const char* name, uint8_t value)
{
    name;
    _stream->writeUnsignedByte(value);
}

void BinaryWriter::writeShort(int16_t value)
{
    ++_countStack.top();
    _stream->writeShort(value);
}

void BinaryWriter::writeShort(const char* name, int16_t value)
{
    name;
    _stream->writeShort(value);
}

void BinaryWriter::writeUnsignedShort(uint16_t value)
{
    ++_countStack.top();
    _stream->writeUnsignedShort(value);
}

void BinaryWriter::writeUnsignedShort(const char* name, uint16_t value)
{
    name;
    _stream->writeUnsignedShort(value);
}

void BinaryWriter::writeInt(int32_t value)
{
    ++_countStack.top();
    _stream->writeInt(value);
}

void BinaryWriter::writeInt(const char* name, int32_t value)
{
    name;
    _stream->writeInt(value);
}

void BinaryWriter::writeUnsignedInt(uint32_t value)
{
    ++_countStack.top();
    _stream->writeUnsignedInt(value);
}

void BinaryWriter::writeUnsignedInt(const char* name, uint32_t value)
{
    name;
    _stream->writeUnsignedInt(value);
}

void BinaryWriter::writeLong(int64_t value)
{
    ++_countStack.top();
    _stream->writeLong(value);
}

void BinaryWriter::writeLong(const char* name, int64_t value)
{
    name;
    _stream->writeLong(value);
}

void BinaryWriter::writeUnsignedLong(uint64_t value)
{
    ++_countStack.top();
    _stream->writeUnsignedLong(value);
}

void BinaryWriter::writeUnsignedLong(const char* name, uint64_t value)
{
    name;
    _stream->writeUnsignedLong(value);
}

void BinaryWriter::writeFloat(float value)
{
    ++_countStack.top();
    _stream->writeFloat(value);
}

void BinaryWriter::writeFloat(const char* name, float value)
{
    name;
    _stream->writeFloat(value);
}

void BinaryWriter::writeDouble(double value)
{
    ++_countStack.top();
    _stream->writeDouble(value);
}

void BinaryWriter::writeDouble(const char* name, double value)
{
    name;
    _stream->writeDouble(value);
}

void BinaryWriter::writeReal(Real value)
{
    ++_countStack.top();
    _stream->writeReal(value);
}

void BinaryWriter::writeReal(const char* name, Real value)
{
    name;
    _stream->writeReal(value);
}

void BinaryWriter::writeVector2(const Vector2& value)
{
    ++_countStack.top();
    _stream->writeVector2(value);
}

void BinaryWriter::writeVector2(const char* name, const Vector2& value)
{
    name;
    _stream->writeVector2(value);
}

void BinaryWriter::writeVector3(const Vector3& value)
{
    ++_countStack.top();
    _stream->writeVector3(value);
}

void BinaryWriter::writeVector3(const char* name, const Vector3& value)
{
    name;
    _stream->writeVector3(value);
}

void BinaryWriter::writeVector4(const Vector4& value)
{
    ++_countStack.top();
    _stream->writeVector4(value);
}

void BinaryWriter::writeVector4(const char* name, const Vector4& value)
{
    name;
    _stream->writeVector4(value);
}