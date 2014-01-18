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

void ArraySerializer::writeString(const std::string& value)
{
    assert(_serializer);
    _serializer->writeString(value);
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

void ObjectSerializer::writeString(const char* name, const std::string& value)
{
    assert(_serializer);
    _serializer->writeString(name, value);
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