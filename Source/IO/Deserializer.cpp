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

std::string ArrayDeserializer::readString()
{
    assert(_deserializer);
    return _deserializer->readString();
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

std::string ObjectDeserializer::readString(const char* name)
{
    assert(_deserializer);
    return _deserializer->readString(name);
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