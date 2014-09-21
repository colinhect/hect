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

#include "Hect/IO/Decoder.h"
#include "Hect/IO/ReadStream.h"

namespace hect
{

///
/// Provides access for decoding structured data from a binary stream.
class BinaryDecoder :
    public Decoder
{
public:

    ///
    /// Constructs a decoder given the stream to decode from.
    ///
    /// \param stream The stream to decode from.
    BinaryDecoder(ReadStream& stream);

    bool isBinaryStream() const;
    ReadStream& binaryStream();

    ArrayDecoder decodeArray();
    ObjectDecoder decodeObject();

private:
    void beginArray();
    void beginArray(const char* name);
    void endArray();

    bool hasMoreElements() const;

    void beginObject();
    void beginObject(const char* name);
    void endObject();

    bool hasMember(const char* name) const;

    std::string decodeString();
    std::string decodeString(const char* name);
    int8_t decodeInt8();
    int8_t decodeInt8(const char* name);
    uint8_t decodeUInt8();
    uint8_t decodeUInt8(const char* name);
    int16_t decodeInt16();
    int16_t decodeInt16(const char* name);
    uint16_t decodeUInt16();
    uint16_t decodeUInt16(const char* name);
    int32_t decodeInt32();
    int32_t decodeInt32(const char* name);
    uint32_t decodeUInt32();
    uint32_t decodeUInt32(const char* name);
    int64_t decodeInt64();
    int64_t decodeInt64(const char* name);
    uint64_t decodeUInt64();
    uint64_t decodeUInt64(const char* name);
    float decodeFloat32();
    float decodeFloat32(const char* name);
    double decodeFloat64();
    double decodeFloat64(const char* name);
    Real decodeReal();
    Real decodeReal(const char* name);
    bool decodeBool();
    bool decodeBool(const char* name);
    Vector2 decodeVector2();
    Vector2 decodeVector2(const char* name);
    Vector3 decodeVector3();
    Vector3 decodeVector3(const char* name);
    Vector4 decodeVector4();
    Vector4 decodeVector4(const char* name);

    std::stack<unsigned> _indexStack;
    std::stack<unsigned> _countStack;
    ReadStream* _stream;
};

}