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

    bool is_binary_stream() const override;
    WriteStream& binary_stream() override;
    void begin_array() override;
    void end_array() override;
    void begin_object() override;
    void end_object() override;
    void select_member(const char* name) override;
    void encode_string(const std::string& value) override;
    void encode_int8(int8_t value) override;
    void encode_u_int8(uint8_t value) override;
    void encode_int16(int16_t value) override;
    void encode_u_int16(uint16_t value) override;
    void encode_int32(int32_t value) override;
    void encode_u_int32(uint32_t value) override;
    void encode_int64(int64_t value) override;
    void encode_u_int64(uint64_t value) override;
    void encode_float32(float value) override;
    void encode_float64(double value) override;
    void encode_bool(bool value) override;

private:
    void increment_count();

    enum ValueType
    {
        ValueType_Array,
        ValueType_Object
    };

    std::stack<size_t> _count_position_stack;
    std::stack<uint32_t> _count_stack;
    std::stack<ValueType> _value_type_stack;

    std::unique_ptr<WriteStream> _owned_stream;
    WriteStream& _stream;
};

}