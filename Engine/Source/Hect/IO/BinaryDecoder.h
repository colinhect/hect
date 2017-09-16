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
#include "Hect/IO/Decoder.h"
#include "Hect/IO/ReadStream.h"

namespace hect
{

///
/// Provides access for decoding structured data from a binary stream.
class HECT_EXPORT BinaryDecoder :
    public Decoder
{
public:

    ///
    /// Constructs a binary decoder given a stream to decode from.
    ///
    /// \param stream The stream to decode from.
    BinaryDecoder(ReadStream& stream);

    ///
    /// Constructs a binary decoder given a stream to decode from.
    ///
    /// \param stream The stream to decode from.
    /// \param asset_cache The asset cache to load further assets from.
    BinaryDecoder(ReadStream& stream, AssetCache& asset_cache);

    ///
    /// Constructs a binary decoder given data decode from.
    ///
    /// \param data The data to decode from.
    BinaryDecoder(const ByteVector& data);

    bool is_binary_stream() const override;
    ReadStream& binary_stream() override;
    void begin_array() override;
    void end_array() override;
    bool has_more_elements() const override;
    void begin_object() override;
    void end_object() override;
    bool select_member(const char* name) override;
    std::vector<std::string> member_names() const override;
    std::string decode_string() override;
    int8_t decode_int8() override;
    uint8_t decode_uint8() override;
    int16_t decode_int16() override;
    uint16_t decode_uint16() override;
    int32_t decode_int32() override;
    uint32_t decode_uint32() override;
    int64_t decode_int64() override;
    uint64_t decode_uint64() override;
    float decode_float32() override;
    double decode_float64() override;
    bool decode_bool() override;

private:
    void increment_index();

    enum ValueType
    {
        ValueType_Array,
        ValueType_Object
    };

    std::stack<uint32_t> _index_stack;
    std::stack<uint32_t> _count_stack;
    std::stack<ValueType> _value_type_stack;

    std::unique_ptr<ReadStream> _owned_stream;
    ReadStream& _stream;
};

}