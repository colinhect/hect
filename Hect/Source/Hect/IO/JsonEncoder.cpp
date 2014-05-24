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

bool JsonEncoder::isBinaryStream() const
{
    return false;
}

WriteStream& JsonEncoder::binaryStream()
{
    throw Error("The encoder is not writing to a binary stream");
}

ArrayEncoder JsonEncoder::encodeArray()
{
    if (!_valueStack.empty())
    {
        throw Error("Current encoding object has not ended");
    }

    _valueStack.push(JsonValue(JsonValueType::Array));
    return ArrayEncoder(this);
}

ObjectEncoder JsonEncoder::encodeObject()
{
    if (!_valueStack.empty())
    {
        throw Error("Current encoding object has not ended");
    }

    _valueStack.push(JsonValue(JsonValueType::Object));
    return ObjectEncoder(this);
}

JsonValue::Array& JsonEncoder::jsonValues()
{
    return _completed;
}

void JsonEncoder::beginArray()
{
    assert(_valueStack.top().isArray());
    _valueStack.push(JsonValue(JsonValueType::Array));
}

void JsonEncoder::beginArray(const char* name)
{
    assert(_valueStack.top().isObject());
    _nameStack.push(name);
    _valueStack.push(JsonValue(JsonValueType::Array));
}

void JsonEncoder::endArray()
{
    endObject();
}

void JsonEncoder::beginObject()
{
    assert(_valueStack.top().isArray());
    _valueStack.push(JsonValue(JsonValueType::Object));
}

void JsonEncoder::beginObject(const char* name)
{
    assert(_valueStack.top().isObject());
    _nameStack.push(name);
    _valueStack.push(JsonValue(JsonValueType::Object));
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

void JsonEncoder::encodeString(const std::string& value)
{
    _encode(value);
}

void JsonEncoder::encodeString(const char* name, const std::string& value)
{
    _encode(name, value);
}

void JsonEncoder::encodeByte(int8_t value)
{
    _encode(value);
}

void JsonEncoder::encodeByte(const char* name, int8_t value)
{
    _encode(name, value);
}

void JsonEncoder::encodeUnsignedByte(uint8_t value)
{
    _encode(value);
}

void JsonEncoder::encodeUnsignedByte(const char* name, uint8_t value)
{
    _encode(name, value);
}

void JsonEncoder::encodeShort(int16_t value)
{
    _encode(value);
}

void JsonEncoder::encodeShort(const char* name, int16_t value)
{
    _encode(name, value);
}

void JsonEncoder::encodeUnsignedShort(uint16_t value)
{
    _encode(value);
}

void JsonEncoder::encodeUnsignedShort(const char* name, uint16_t value)
{
    _encode(name, value);
}

void JsonEncoder::encodeInt(int32_t value)
{
    _encode(value);
}

void JsonEncoder::encodeInt(const char* name, int32_t value)
{
    _encode(name, value);
}

void JsonEncoder::encodeUnsignedInt(uint32_t value)
{
    _encode(value);
}

void JsonEncoder::encodeUnsignedInt(const char* name, uint32_t value)
{
    _encode(name, value);
}

void JsonEncoder::encodeLong(int64_t value)
{
    _encode((double)value);
}

void JsonEncoder::encodeLong(const char* name, int64_t value)
{
    _encode(name, (double)value);
}

void JsonEncoder::encodeUnsignedLong(uint64_t value)
{
    _encode((double)value);
}

void JsonEncoder::encodeUnsignedLong(const char* name, uint64_t value)
{
    _encode(name, (double)value);
}

void JsonEncoder::encodeFloat(float value)
{
    _encode(value);
}

void JsonEncoder::encodeFloat(const char* name, float value)
{
    _encode(name, value);
}

void JsonEncoder::encodeDouble(double value)
{
    _encode(value);
}

void JsonEncoder::encodeDouble(const char* name, double value)
{
    _encode(name, value);
}

void JsonEncoder::encodeReal(Real value)
{
    _encode(value);
}

void JsonEncoder::encodeReal(const char* name, Real value)
{
    _encode(name, value);
}

void JsonEncoder::encodeBool(bool value)
{
    _encode(value);
}

void JsonEncoder::encodeBool(const char* name, bool value)
{
    _encode(name, value);
}

void JsonEncoder::encodeVector2(const Vector2& value)
{
    _encode(value);
}

void JsonEncoder::encodeVector2(const char* name, const Vector2& value)
{
    _encode(name, value);
}

void JsonEncoder::encodeVector3(const Vector3& value)
{
    _encode(value);
}

void JsonEncoder::encodeVector3(const char* name, const Vector3& value)
{
    _encode(name, value);
}

void JsonEncoder::encodeVector4(const Vector4& value)
{
    _encode(value);
}

void JsonEncoder::encodeVector4(const char* name, const Vector4& value)
{
    _encode(name, value);
}

void JsonEncoder::_encode(const JsonValue& value)
{
    JsonValue& top = _valueStack.top();
    assert(top.isArray());
    top.addElement(value);
}

void JsonEncoder::_encode(const char* name, const JsonValue& value)
{
    JsonValue& top = _valueStack.top();
    assert(top.isObject());
    top.addMember(name, value);
}