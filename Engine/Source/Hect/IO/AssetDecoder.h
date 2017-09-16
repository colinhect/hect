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

#include <memory>

#include "Hect/Core/Export.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/DataValue.h"
#include "Hect/IO/MemoryReadStream.h"
#include "Hect/IO/Path.h"

namespace hect
{

///
/// Provides the functionality to decode an Asset directly from an AssetCache,
/// automatically detecting if the data is binary or text-based.
class HECT_EXPORT AssetDecoder :
    public Decoder,
    public Uncopyable
{
public:

    ///
    /// Constructs a decoder.
    ///
    /// \note The correct decoder implementation will be selected based on the
    /// data of the asset.
    ///
    /// \param asset_cache The asset cache to load the asset data from.
    /// \param path The path to the asset data.
    AssetDecoder(AssetCache& asset_cache, const Path& path);

    ~AssetDecoder();

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
    bool is_yaml(ReadStream& stream);

    std::unique_ptr<Decoder> _implementation;

    DataValue _data_value;
    ByteVector _data;
    std::unique_ptr<MemoryReadStream> _stream;
};

}