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
#include "Decoder.h"

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

bool ArrayDecoder::hasMoreElements() const
{
    assert(_decoder);
    return _decoder->hasMoreElements();
}

std::string ArrayDecoder::decodeString()
{
    assert(_decoder);
    return _decoder->decodeString();
}

ArrayDecoder ArrayDecoder::decodeArray()
{
    assert(_decoder);
    if (_decoder->beginArray())
    {
        return ArrayDecoder(_decoder);
    }
    else
    {
        return ArrayDecoder();
    }
}

ObjectDecoder ArrayDecoder::decodeObject()
{
    assert(_decoder);
    if (_decoder->beginObject())
    {
        return ObjectDecoder(_decoder);
    }
    else
    {
        return ObjectDecoder();
    }
}

ArrayDecoder::operator bool() const
{
    return _decoder != nullptr;
}

ArrayDecoder::ArrayDecoder() :
    _decoder(nullptr)
{
}

ArrayDecoder::ArrayDecoder(Decoder* decoder) :
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

bool ObjectDecoder::hasMember(const char* name) const
{
    return _decoder->hasMember(name);
}

std::string ObjectDecoder::decodeString(const char* name)
{
    assert(_decoder);
    return _decoder->decodeString(name);
}

ArrayDecoder ObjectDecoder::decodeArray(const char* name)
{
    assert(_decoder);
    if (_decoder->beginArray(name))
    {
        return ArrayDecoder(_decoder);
    }
    else
    {
        return ArrayDecoder();
    }
}

ObjectDecoder ObjectDecoder::decodeObject(const char* name)
{
    assert(_decoder);
    if (_decoder->beginObject(name))
    {
        return ObjectDecoder(_decoder);
    }
    else
    {
        return ObjectDecoder();
    }
}

ObjectDecoder::operator bool() const
{
    return _decoder != nullptr;
}

ObjectDecoder::ObjectDecoder() :
    _decoder(nullptr)
{
}

ObjectDecoder::ObjectDecoder(Decoder* decoder) :
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
        return ArrayDecoder();
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
        return ObjectDecoder();
    }
}

bool DataValueDecoder::beginArray()
{
    DataValue& top = _valueStack.top();
    assert(top.isArray());

    DataValue value = _decode();
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

bool DataValueDecoder::beginArray(const char* name)
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

bool DataValueDecoder::beginObject()
{
    DataValue& top = _valueStack.top();
    assert(top.isArray());

    DataValue value = _decode();
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

bool DataValueDecoder::beginObject(const char* name)
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
    return top[name];
}