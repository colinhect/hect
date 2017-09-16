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
#include "DataValueEncoder.h"

using namespace hect;

DataValueEncoder::DataValueEncoder()
{
}

DataValueEncoder::ValueSequence DataValueEncoder::data_values()
{
    return _completed;
}

bool DataValueEncoder::is_binary_stream() const
{
    return false;
}

WriteStream& DataValueEncoder::binary_stream()
{
    throw InvalidOperation("The encoder is not writing to a binary stream");
}

void DataValueEncoder::begin_array()
{
    _member_selected = false;
    _value_stack.push(DataValue(DataValueType::Array));
}

void DataValueEncoder::end_array()
{
    end_object();
}

void DataValueEncoder::begin_object()
{
    _member_selected = false;
    _value_stack.push(DataValue(DataValueType::Object));
}

void DataValueEncoder::end_object()
{
    DataValue value = _value_stack.top();
    _value_stack.pop();

    if (_value_stack.empty())
    {
        _completed.push_back(value);
    }
    else if (_value_stack.top().is_array())
    {
        _value_stack.top().add_element(value);
    }
    else if (_value_stack.top().is_object())
    {
        _value_stack.top().add_member(_name_stack.top(), value);
        _name_stack.pop();
    }
}

void DataValueEncoder::select_member(const char* name)
{
    if (_value_stack.empty() || !_value_stack.top().is_object())
    {
        throw InvalidOperation("Current encoding object cannot have named members");
    }
    _name_stack.push(name);
    _member_selected = true;
}

void DataValueEncoder::encode_string(const std::string& value)
{
    encode(value);
}

void DataValueEncoder::encode_int8(int8_t value)
{
    encode(value);
}

void DataValueEncoder::encode_u_int8(uint8_t value)
{
    encode(value);
}

void DataValueEncoder::encode_int16(int16_t value)
{
    encode(value);
}

void DataValueEncoder::encode_u_int16(uint16_t value)
{
    encode(value);
}

void DataValueEncoder::encode_int32(int32_t value)
{
    encode(value);
}

void DataValueEncoder::encode_u_int32(uint32_t value)
{
    encode(value);
}

void DataValueEncoder::encode_int64(int64_t value)
{
    encode(static_cast<double>(value));
}

void DataValueEncoder::encode_u_int64(uint64_t value)
{
    encode(static_cast<double>(value));
}

void DataValueEncoder::encode_float32(float value)
{
    encode(value);
}

void DataValueEncoder::encode_float64(double value)
{
    encode(value);
}

void DataValueEncoder::encode_bool(bool value)
{
    encode(value);
}

void DataValueEncoder::encode(const DataValue& value)
{
    DataValue& top = _value_stack.top();
    if (top.is_array())
    {
        top.add_element(value);
    }
    else if (top.is_object())
    {
        if (!_member_selected)
        {
            throw InvalidOperation("Cannot encode a value to an object without first selecting a member");
        }

        top.add_member(_name_stack.top(), value);

        _member_selected = false;
        _name_stack.pop();
    }
}