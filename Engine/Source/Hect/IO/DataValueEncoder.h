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
#include "Hect/Core/Sequence.h"
#include "Hect/IO/DataValue.h"
#include "Hect/IO/Encoder.h"

namespace hect
{

///
/// Provides access for encoding structured data to a DataValue.
class HECT_EXPORT DataValueEncoder :
    public Encoder
{
    typedef std::vector<DataValue> ValueContainer;
public:

    ///
    /// A sequence of DataValue%s.
    typedef Sequence<DataValue, ValueContainer> ValueSequence;

    ///
    /// Constructs a DataValue encoder.
    DataValueEncoder();

    ///
    /// Returns the encoded DataValue%s.
    ValueSequence data_values();

    bool is_binary_stream() const override;
    WriteStream& binary_stream() override;
    void begin_array() override;
    void end_array() override;
    void begin_object() override;
    void end_object() override;
    void select_member(const char* name) override;
    void encode_string(const std::string& value) override;
    void encode_int8(int8_t value) override;
    void encode_uint8(uint8_t value) override;
    void encode_int16(int16_t value) override;
    void encode_uint16(uint16_t value) override;
    void encode_int32(int32_t value) override;
    void encode_uint32(uint32_t value) override;
    void encode_int64(int64_t value) override;
    void encode_uint64(uint64_t value) override;
    void encode_float32(float value) override;
    void encode_float64(double value) override;
    void encode_bool(bool value) override;

private:
    void encode(const DataValue& value);

    std::stack<std::string> _name_stack;
    std::stack<DataValue> _value_stack;

    bool _member_selected { false };

    ValueContainer _completed;
};

}