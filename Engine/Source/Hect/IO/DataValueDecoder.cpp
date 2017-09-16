///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2016 Colin Hill
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

DataValueDecoder::DataValueDecoder(const DataValue& data_value)
{
    _value_stack.push(data_value);
}

DataValueDecoder::DataValueDecoder(const DataValue& data_value, AssetCache& asset_cache) :
    Decoder(asset_cache)
{
    _value_stack.push(data_value);
}

bool DataValueDecoder::is_binary_stream() const
{
    return false;
}

ReadStream& DataValueDecoder::binary_stream()
{
    throw InvalidOperation("The decoder is not reading from a binary stream");
}

void DataValueDecoder::begin_array()
{
    DataValue value = decode();
    if (value.is_array())
    {
        _index_stack.push(0);
        _value_stack.push(value);
    }
    else
    {
        throw InvalidOperation("The next value is not an array");
    }
}

void DataValueDecoder::end_array()
{
    assert(_value_stack.top().is_array());
    _index_stack.pop();
    _value_stack.pop();
}

bool DataValueDecoder::has_more_elements() const
{
    const DataValue& top = _value_stack.top();
    assert(top.is_array());
    return _index_stack.top() < top.size();
}

void DataValueDecoder::begin_object()
{
    DataValue value = decode();
    if (value.is_object())
    {
        _value_stack.push(value);
    }
    else
    {
        throw InvalidOperation("The next value is not an object");
    }
}

void DataValueDecoder::end_object()
{
    assert(_value_stack.top().is_object());
    _value_stack.pop();
}

bool DataValueDecoder::select_member(const char* name)
{
    const DataValue& top = _value_stack.top();
    assert(top.is_object());

    if (!top[name].is_null())
    {
        _selected_member_name = name;
        return true;
    }
    else
    {
        return false;
    }
}

std::vector<std::string> DataValueDecoder::member_names() const
{
    const DataValue& top = _value_stack.top();
    assert(top.is_object());
    return top.member_names();
}

std::string DataValueDecoder::decode_string()
{
    return decode().as_string();
}

int8_t DataValueDecoder::decode_int8()
{
    return static_cast<int8_t>(decode().as_double());
}

uint8_t DataValueDecoder::decode_u_int8()
{
    return static_cast<uint8_t>(decode().as_double());
}

int16_t DataValueDecoder::decode_int16()
{
    return static_cast<int16_t>(decode().as_double());
}

uint16_t DataValueDecoder::decode_u_int16()
{
    return static_cast<uint16_t>(decode().as_double());
}

int32_t DataValueDecoder::decode_int32()
{
    return static_cast<int32_t>(decode().as_double());
}

uint32_t DataValueDecoder::decode_u_int32()
{
    return static_cast<uint32_t>(decode().as_double());
}

int64_t DataValueDecoder::decode_int64()
{
    return static_cast<int64_t>(decode().as_double());
}

uint64_t DataValueDecoder::decode_u_int64()
{
    return static_cast<uint64_t>(decode().as_double());
}

float DataValueDecoder::decode_float32()
{
    return static_cast<float>(decode().as_double());
}

double DataValueDecoder::decode_float64()
{
    return decode().as_double();
}

bool DataValueDecoder::decode_bool()
{
    return decode().as_bool();
}

const DataValue& DataValueDecoder::decode()
{
    DataValue& top = _value_stack.top();
    if (top.is_array() && !_index_stack.empty())
    {
        return top[_index_stack.top()++];
    }
    else if (top.is_object() && !_selected_member_name.empty())
    {
        const DataValue& result = top[_selected_member_name];
        if (result.is_null())
        {
            throw DecodeError(format("No member value '%s'", _selected_member_name.data()));
        }
        _selected_member_name = std::string();
        return result;
    }
    else
    {
        return top;
    }
}
