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
#include "JsonEncoder.h"

using namespace hect;

JsonEncoder::JsonEncoder()
{
}

JsonEncoder::ValueSequence JsonEncoder::jsonValues()
{
    return _completed;
}

bool JsonEncoder::isBinaryStream() const
{
    return false;
}

WriteStream& JsonEncoder::binaryStream()
{
    throw Error("The encoder is not writing to a binary stream");
}

void JsonEncoder::beginArray()
{
    _memberSelected = false;
    _valueStack.push(JsonValue(JsonValueType_Array));
}

void JsonEncoder::endArray()
{
    endObject();
}

void JsonEncoder::beginObject()
{
    _memberSelected = false;
    _valueStack.push(JsonValue(JsonValueType_Object));
}

void JsonEncoder::endObject()
{
    JsonValue value = _valueStack.top();
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

void JsonEncoder::selectMember(const char* name)
{
    if (_valueStack.empty() || !_valueStack.top().isObject())
    {
        throw Error("Current encoding object cannot have named members");
    }
    _nameStack.push(name);
    _memberSelected = true;
}

void JsonEncoder::encodeString(const std::string& value)
{
    encode(value);
}

void JsonEncoder::encodeInt8(int8_t value)
{
    encode(value);
}

void JsonEncoder::encodeUInt8(uint8_t value)
{
    encode(value);
}

void JsonEncoder::encodeInt16(int16_t value)
{
    encode(value);
}

void JsonEncoder::encodeUInt16(uint16_t value)
{
    encode(value);
}

void JsonEncoder::encodeInt32(int32_t value)
{
    encode(value);
}

void JsonEncoder::encodeUInt32(uint32_t value)
{
    encode(value);
}

void JsonEncoder::encodeInt64(int64_t value)
{
    encode(static_cast<double>(value));
}

void JsonEncoder::encodeUInt64(uint64_t value)
{
    encode(static_cast<double>(value));
}

void JsonEncoder::encodeFloat32(float value)
{
    encode(value);
}

void JsonEncoder::encodeFloat64(double value)
{
    encode(value);
}

void JsonEncoder::encodeBool(bool value)
{
    encode(value);
}

void JsonEncoder::encode(const JsonValue& value)
{
    JsonValue& top = _valueStack.top();
    if (top.isArray())
    {
        top.addElement(value);
    }
    else if (top.isObject())
    {
        if (!_memberSelected)
        {
            throw Error("Cannot encode a value to an object without first selecting a member");
        }

        top.addMember(_nameStack.top(), value);

        _memberSelected = false;
        _nameStack.pop();
    }
}