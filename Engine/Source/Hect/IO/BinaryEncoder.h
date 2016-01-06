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
#pragma once

#include <stack>

#include "Hect/Core/Export.h"
#include "Hect/IO/ByteVector.h"
#include "Hect/IO/Encoder.h"

namespace hect
{

///
/// Provides access for encoding structured data to binary data.
class HECT_EXPORT BinaryEncoder :
    public Encoder
{
public:

    ///
    /// Constructs a binary encoder given a stream to write to.
    ///
    /// \param stream The stream to encode to.
    BinaryEncoder(WriteStream& stream);

    ///
    /// Constructs a binary encoder given data to append to.
    ///
    /// \param data The data to append the encoded data to.
    BinaryEncoder(ByteVector& data);

    bool isBinaryStream() const override;
    WriteStream& binaryStream() override;
    void beginArray() override;
    void endArray() override;
    void beginObject() override;
    void endObject() override;
    void selectMember(const char* name) override;
    void encodeString(const std::string& value) override;
    void encodeInt8(int8_t value) override;
    void encodeUInt8(uint8_t value) override;
    void encodeInt16(int16_t value) override;
    void encodeUInt16(uint16_t value) override;
    void encodeInt32(int32_t value) override;
    void encodeUInt32(uint32_t value) override;
    void encodeInt64(int64_t value) override;
    void encodeUInt64(uint64_t value) override;
    void encodeFloat32(float value) override;
    void encodeFloat64(double value) override;
    void encodeBool(bool value) override;

private:
    void incrementCount();

    enum ValueType
    {
        ValueType_Array,
        ValueType_Object
    };

    std::stack<size_t> _countPositionStack;
    std::stack<uint32_t> _countStack;
    std::stack<ValueType> _valueTypeStack;

    std::unique_ptr<WriteStream> _ownedStream;
    WriteStream& _stream;
};

}