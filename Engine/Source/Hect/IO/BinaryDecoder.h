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
    /// \param assetCache The asset cache to load further assets from.
    BinaryDecoder(ReadStream& stream, AssetCache& assetCache);

    ///
    /// Constructs a binary decoder given data decode from.
    ///
    /// \param data The data to decode from.
    BinaryDecoder(const ByteVector& data);

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
    void incrementIndex();

    enum ValueType
    {
        ValueType_Array,
        ValueType_Object
    };

    std::stack<uint32_t> _indexStack;
    std::stack<uint32_t> _countStack;
    std::stack<ValueType> _valueTypeStack;

    std::unique_ptr<ReadStream> _ownedStream;
    ReadStream& _stream;
};

}