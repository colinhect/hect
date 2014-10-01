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

#include "Hect/Core/Sequence.h"
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
    JsonEncoder();

    ///
    /// Returns the encoded JSON values.
    Sequence<JsonValue> jsonValues();

    bool isBinaryStream() const;

    WriteStream& binaryStream();

    void beginArray();
    void endArray();

    void beginObject();
    void endObject();

    void selectMember(const char* name);

    void encodeString(const std::string& value);
    void encodeInt8(int8_t value);
    void encodeUInt8(uint8_t value);
    void encodeInt16(int16_t value);
    void encodeUInt16(uint16_t value);
    void encodeInt32(int32_t value);
    void encodeUInt32(uint32_t value);
    void encodeInt64(int64_t value);
    void encodeUInt64(uint64_t value);
    void encodeFloat32(float value);
    void encodeFloat64(double value);
    void encodeBool(bool value);

private:
    void encode(const JsonValue& value);

    std::stack<std::string> _nameStack;
    std::stack<JsonValue> _valueStack;

    bool _memberSelected;

    JsonValue::Array _completed;
};

}