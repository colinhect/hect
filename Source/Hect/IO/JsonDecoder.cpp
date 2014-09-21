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
#include "JsonDecoder.h"

#include "Hect/Core/Format.h"

using namespace hect;

JsonDecoder::JsonDecoder(const JsonValue& jsonValue)
{
    _valueStack.push(jsonValue);
}

bool JsonDecoder::isBinaryStream() const
{
    return false;
}

ReadStream& JsonDecoder::binaryStream()
{
    throw Error("The decoder is not reading from a binary stream");
}

ArrayDecoder JsonDecoder::decodeArray()
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

ObjectDecoder JsonDecoder::decodeObject()
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

void JsonDecoder::beginArray()
{
    JsonValue value = decode();
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

void JsonDecoder::beginArray(const char* name)
{
    JsonValue& top = _valueStack.top();
    assert(top.isObject());

    const JsonValue& value = top[name];
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

void JsonDecoder::endArray()
{
    const JsonValue& top = _valueStack.top();
    assert(top.isArray());
    _indexStack.pop();
    _valueStack.pop();
}

bool JsonDecoder::hasMoreElements() const
{
    const JsonValue& top = _valueStack.top();
    assert(top.isArray());
    return _indexStack.top() < top.size();
}

void JsonDecoder::beginObject()
{
    JsonValue& top = _valueStack.top();
    assert(top.isArray());

    JsonValue value = decode();
    if (value.isObject())
    {
        _valueStack.push(value);
    }
    else
    {
        throw Error("The next value is not an object");
    }
}

void JsonDecoder::beginObject(const char* name)
{
    JsonValue& top = _valueStack.top();
    assert(top.isObject());

    const JsonValue& value = top[name];
    if (!value.isNull())
    {
        _valueStack.push(value);
    }
    else
    {
        throw Error(format("No member value '%s'", name));
    }
}

void JsonDecoder::endObject()
{
    const JsonValue& top = _valueStack.top();
    assert(top.isObject());
    _valueStack.pop();
}

bool JsonDecoder::hasMember(const char* name) const
{
    const JsonValue& top = _valueStack.top();
    assert(top.isObject());
    return !top[name].isNull();
}

std::string JsonDecoder::decodeString()
{
    return decode().asString();
}

std::string JsonDecoder::decodeString(const char* name)
{
    return decode(name).asString();
}

int8_t JsonDecoder::decodeInt8()
{
    return (int8_t)decode().asReal();
}

int8_t JsonDecoder::decodeInt8(const char* name)
{
    return (int8_t)decode(name).asReal();
}

uint8_t JsonDecoder::decodeUInt8()
{
    return (uint8_t)decode().asReal();
}

uint8_t JsonDecoder::decodeUInt8(const char* name)
{
    return (uint8_t)decode(name).asReal();
}

int16_t JsonDecoder::decodeInt16()
{
    return (int16_t)decode().asReal();
}

int16_t JsonDecoder::decodeInt16(const char* name)
{
    return (int16_t)decode(name).asReal();
}

uint16_t JsonDecoder::decodeUInt16()
{
    return (uint16_t)decode().asReal();
}

uint16_t JsonDecoder::decodeUInt16(const char* name)
{
    return (uint16_t)decode(name).asReal();
}

int32_t JsonDecoder::decodeInt32()
{
    return (int32_t)decode().asReal();
}

int32_t JsonDecoder::decodeInt32(const char* name)
{
    return (int32_t)decode(name).asReal();
}

uint32_t JsonDecoder::decodeUInt32()
{
    return (uint32_t)decode().asReal();
}

uint32_t JsonDecoder::decodeUInt32(const char* name)
{
    return (uint32_t)decode(name).asReal();
}

int64_t JsonDecoder::decodeInt64()
{
    return (int64_t)decode().asReal();
}

int64_t JsonDecoder::decodeInt64(const char* name)
{
    return (int64_t)decode(name).asReal();
}

uint64_t JsonDecoder::decodeUInt64()
{
    return (uint64_t)decode().asReal();
}

uint64_t JsonDecoder::decodeUInt64(const char* name)
{
    return (uint64_t)decode(name).asReal();
}

float JsonDecoder::decodeFloat32()
{
    return (float)decode().asReal();
}

float JsonDecoder::decodeFloat32(const char* name)
{
    return (float)decode(name).asReal();
}

double JsonDecoder::decodeFloat64()
{
    return decode().asReal();
}

double JsonDecoder::decodeFloat64(const char* name)
{
    return decode(name).asReal();
}

Real JsonDecoder::decodeReal()
{
    return (Real)decode().asReal();
}

Real JsonDecoder::decodeReal(const char* name)
{
    return (Real)decode(name).asReal();
}

bool JsonDecoder::decodeBool()
{
    return decode().asBool();
}

bool JsonDecoder::decodeBool(const char* name)
{
    return decode(name).asBool();
}

Vector2 JsonDecoder::decodeVector2()
{
    return decode().asVector2();
}

Vector2 JsonDecoder::decodeVector2(const char* name)
{
    return decode(name).asVector2();
}

Vector3 JsonDecoder::decodeVector3()
{
    return decode().asVector3();
}

Vector3 JsonDecoder::decodeVector3(const char* name)
{
    return decode(name).asVector3();
}

Vector4 JsonDecoder::decodeVector4()
{
    return decode().asVector4();
}

Vector4 JsonDecoder::decodeVector4(const char* name)
{
    return decode(name).asVector4();
}

const JsonValue& JsonDecoder::decode()
{
    JsonValue& top = _valueStack.top();
    assert(top.isArray());
    return top[_indexStack.top()++];
}

const JsonValue& JsonDecoder::decode(const char* name)
{
    JsonValue& top = _valueStack.top();
    assert(top.isObject());

    const JsonValue& result = top[name];
    if (result.isNull())
    {
        throw Error(format("No member value '%s'", name));
    }

    return result;
}