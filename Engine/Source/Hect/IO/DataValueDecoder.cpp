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
#include "DataValueDecoder.h"

#include "Hect/Core/Format.h"

using namespace hect;

DataValueDecoder::DataValueDecoder(const DataValue& dataValue)
{
    _valueStack.push(dataValue);
}

DataValueDecoder::DataValueDecoder(const DataValue& dataValue, AssetCache& assetCache) :
    Decoder(assetCache)
{
    _valueStack.push(dataValue);
}

bool DataValueDecoder::isBinaryStream() const
{
    return false;
}

ReadStream& DataValueDecoder::binaryStream()
{
    throw Error("The decoder is not reading from a binary stream");
}

void DataValueDecoder::beginArray()
{
    DataValue value = decode();
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

void DataValueDecoder::endArray()
{
    assert(_valueStack.top().isArray());
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
    DataValue value = decode();
    if (value.isObject())
    {
        _valueStack.push(value);
    }
    else
    {
        throw Error("The next value is not an object");
    }
}

void DataValueDecoder::endObject()
{
    assert(_valueStack.top().isObject());
    _valueStack.pop();
}

bool DataValueDecoder::selectMember(const char* name)
{
    const DataValue& top = _valueStack.top();
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

std::vector<std::string> DataValueDecoder::memberNames() const
{
    const DataValue& top = _valueStack.top();
    assert(top.isObject());
    return top.memberNames();
}

std::string DataValueDecoder::decodeString()
{
    return decode().asString();
}

int8_t DataValueDecoder::decodeInt8()
{
    return (int8_t)decode().asReal();
}

uint8_t DataValueDecoder::decodeUInt8()
{
    return (uint8_t)decode().asReal();
}

int16_t DataValueDecoder::decodeInt16()
{
    return (int16_t)decode().asReal();
}

uint16_t DataValueDecoder::decodeUInt16()
{
    return (uint16_t)decode().asReal();
}

int32_t DataValueDecoder::decodeInt32()
{
    return (int32_t)decode().asReal();
}

uint32_t DataValueDecoder::decodeUInt32()
{
    return (uint32_t)decode().asReal();
}

int64_t DataValueDecoder::decodeInt64()
{
    return (int64_t)decode().asReal();
}

uint64_t DataValueDecoder::decodeUInt64()
{
    return (uint64_t)decode().asReal();
}

float DataValueDecoder::decodeFloat32()
{
    return (float)decode().asReal();
}

double DataValueDecoder::decodeFloat64()
{
    return decode().asReal();
}

bool DataValueDecoder::decodeBool()
{
    return decode().asBool();
}

const DataValue& DataValueDecoder::decode()
{
    DataValue& top = _valueStack.top();
    if (top.isArray() && !_indexStack.empty())
    {
        return top[_indexStack.top()++];
    }
    else if (top.isObject() && !_selectedMemberName.empty())
    {
        const DataValue& result = top[_selectedMemberName];
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
