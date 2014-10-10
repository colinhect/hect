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

#include "Hect/IO/Decoder.h"
#include "Hect/IO/JsonValue.h"
#include "Hect/IO/MemoryReadStream.h"
#include "Hect/IO/Path.h"

namespace hect
{

class AssetDecoder :
    public Decoder,
    public Uncopyable
{
public:

    ///
    /// Constructs a decoder given an asset cache and a path to the asset.
    ///
    /// \note The correct decoder implementation will be selected based on the
    /// data of the asset.
    ///
    /// \param assetCache The asset cache to load the asset data from.
    /// \param path The path to the asset data.
    AssetDecoder(AssetCache& assetCache, const Path& path);

    ~AssetDecoder();

    bool isBinaryStream() const override;
    ReadStream& binaryStream() override;

    void beginArray() override;
    void endArray() override;

    bool hasMoreElements() const override;

    void beginObject() override;
    void endObject() override;

    bool selectMember(const char* name) override;
    std::vector<std::string> memberNames() const override;

    std::string decodeString() override;
    int8_t decodeInt8() override;
    uint8_t decodeUInt8() override;
    int16_t decodeInt16() override;
    uint16_t decodeUInt16() override;
    int32_t decodeInt32() override;
    uint32_t decodeUInt32() override;
    int64_t decodeInt64() override;
    uint64_t decodeUInt64() override;
    float decodeFloat32() override;
    double decodeFloat64() override;
    bool decodeBool() override;

private:
    std::unique_ptr<Decoder> _implementation;

    JsonValue _jsonValue;
    std::vector<uint8_t> _data;
    std::unique_ptr<MemoryReadStream> _stream;
};

}