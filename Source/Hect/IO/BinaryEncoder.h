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

#include "Hect/IO/Encoder.h"

namespace hect
{

///
/// Provides access for encoding structured data to a binary stream.
class BinaryEncoder :
    public Encoder
{
public:

    ///
    /// Constructs an encoder given the stream to encode to.
    ///
    /// \param stream The stream to encode to.
    BinaryEncoder(WriteStream& stream);

    bool isBinaryStream() const;
    WriteStream& binaryStream();

    ArrayEncoder encodeArray();
    ObjectEncoder encodeObject();

private:
    void beginArray();
    void beginArray(const char* name);
    void endArray();

    void beginObject();
    void beginObject(const char* name);
    void endObject();

    void encodeString(const std::string& value);
    void encodeString(const char* name, const std::string& value);
    void encodeInt8(int8_t value);
    void encodeInt8(const char* name, int8_t value);
    void encodeUInt8(uint8_t value);
    void encodeUInt8(const char* name, uint8_t value);
    void encodeInt16(int16_t value);
    void encodeInt16(const char* name, int16_t value);
    void encodeUInt16(uint16_t value);
    void encodeUInt16(const char* name, uint16_t value);
    void encodeInt32(int32_t value);
    void encodeInt32(const char* name, int32_t value);
    void encodeUInt32(uint32_t value);
    void encodeUInt32(const char* name, uint32_t value);
    void encodeInt64(int64_t value);
    void encodeInt64(const char* name, int64_t value);
    void encodeUInt64(uint64_t value);
    void encodeUInt64(const char* name, uint64_t value);
    void encodeFloat32(float value);
    void encodeFloat32(const char* name, float value);
    void encodeFloat64(double value);
    void encodeFloat64(const char* name, double value);
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

    std::stack<size_t> _countPositionStack;
    std::stack<unsigned> _countStack;

    WriteStream* _stream;
};

}