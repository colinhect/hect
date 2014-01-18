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
#include "Encoder.h"

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

void ArrayEncoder::encodeString(const std::string& value)
{
    assert(_encoder);
    _encoder->encodeString(value);
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

ArrayEncoder::ArrayEncoder() :
    _encoder(nullptr)
{
}

ArrayEncoder::ArrayEncoder(Encoder* encoder) :
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

void ObjectEncoder::encodeString(const char* name, const std::string& value)
{
    assert(_encoder);
    _encoder->encodeString(name, value);
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

ObjectEncoder::ObjectEncoder() :
    _encoder(nullptr)
{
}

ObjectEncoder::ObjectEncoder(Encoder* encoder) :
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

DataValue::Array& DataValueEncoder::encodedDataValue()
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