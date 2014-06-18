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
#pragma once

#include <stack>

#include "Hect/Core/Export.h"
#include "Hect/IO/JsonValue.h"
#include "Hect/IO/Encoder.h"

namespace hect
{

///
/// Provides access for encoding structured data to a JSON value.
class JsonEncoder :
    public Encoder
{
public:
    bool isBinaryStream() const;
    WriteStream& binaryStream();

    ArrayEncoder encodeArray();
    ObjectEncoder encodeObject();

    ///
    /// Returns the encoded data values.
    JsonValue::Array& jsonValues();

private:
    void beginArray();
    void beginArray(const char* name);
    void endArray();

    void beginObject();
    void beginObject(const char* name);
    void endObject();

    void encodeString(const std::string& value);
    void encodeString(const char* name, const std::string& value);
    void encodeByte(int8_t value);
    void encodeByte(const char* name, int8_t value);
    void encodeUnsignedByte(uint8_t value);
    void encodeUnsignedByte(const char* name, uint8_t value);
    void encodeShort(int16_t value);
    void encodeShort(const char* name, int16_t value);
    void encodeUnsignedShort(uint16_t value);
    void encodeUnsignedShort(const char* name, uint16_t value);
    void encodeInt(int32_t value);
    void encodeInt(const char* name, int32_t value);
    void encodeUnsignedInt(uint32_t value);
    void encodeUnsignedInt(const char* name, uint32_t value);
    void encodeLong(int64_t value);
    void encodeLong(const char* name, int64_t value);
    void encodeUnsignedLong(uint64_t value);
    void encodeUnsignedLong(const char* name, uint64_t value);
    void encodeFloat(float value);
    void encodeFloat(const char* name, float value);
    void encodeDouble(double value);
    void encodeDouble(const char* name, double value);
    void encodeReal(Real value);
    void encodeReal(const char* name, Real value);
    void encodeBool(bool value);
    void encodeBool(const char* name, bool value);
    void encodeVector2(const Vector2& value);
    void encodeVector2(const char* name, const Vector2& value);
    void encodeVector3(const Vector3& value);
    void encodeVector3(const char* name, const Vector3& value);
    void encodeVector4(const Vector4& value);
    void encodeVector4(const char* name, const Vector4& value);

    void _encode(const JsonValue& value);
    void _encode(const char* name, const JsonValue& value);

    std::stack<std::string> _nameStack;
    std::stack<JsonValue> _valueStack;

    JsonValue::Array _completed;
};

}