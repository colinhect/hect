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

JsonDecoder::JsonDecoder(const JsonValue& jsonValue, AssetCache& assetCache) :
    Decoder(assetCache)
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

void JsonDecoder::endArray()
{
    assert(_valueStack.top().isArray());
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

void JsonDecoder::endObject()
{
    assert(_valueStack.top().isObject());
    _valueStack.pop();
}

bool JsonDecoder::selectMember(const char* name)
{
    const JsonValue& top = _valueStack.top();
    assert(top.isObject());

    if (!top[name].isNull())
    {
        _selectedMemberName = name;
        return true;
    }
    else
    {
        return false;
    }
}

std::vector<std::string> JsonDecoder::memberNames() const
{
    const JsonValue& top = _valueStack.top();
    assert(top.isObject());
    return top.memberNames();
}

std::string JsonDecoder::decodeString()
{
    return decode().asString();
}

int8_t JsonDecoder::decodeInt8()
{
    return (int8_t)decode().asReal();
}

uint8_t JsonDecoder::decodeUInt8()
{
    return (uint8_t)decode().asReal();
}

int16_t JsonDecoder::decodeInt16()
{
    return (int16_t)decode().asReal();
}

uint16_t JsonDecoder::decodeUInt16()
{
    return (uint16_t)decode().asReal();
}

int32_t JsonDecoder::decodeInt32()
{
    return (int32_t)decode().asReal();
}

uint32_t JsonDecoder::decodeUInt32()
{
    return (uint32_t)decode().asReal();
}

int64_t JsonDecoder::decodeInt64()
{
    return (int64_t)decode().asReal();
}

uint64_t JsonDecoder::decodeUInt64()
{
    return (uint64_t)decode().asReal();
}

float JsonDecoder::decodeFloat32()
{
    return (float)decode().asReal();
}

double JsonDecoder::decodeFloat64()
{
    return decode().asReal();
}

bool JsonDecoder::decodeBool()
{
    return decode().asBool();
}

const JsonValue& JsonDecoder::decode()
{
    JsonValue& top = _valueStack.top();
    if (top.isArray() && !_indexStack.empty())
    {
        return top[_indexStack.top()++];
    }
    else if (top.isObject() && !_selectedMemberName.empty())
    {
        const JsonValue& result = top[_selectedMemberName];
        if (result.isNull())
        {
            throw Error(format("No member value '%s'", _selectedMemberName.c_str()));
        }
        _selectedMemberName = std::string();
        return result;
    }
    else
    {
        return top;
    }
}
