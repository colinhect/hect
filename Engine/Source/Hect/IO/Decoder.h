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

#include "Hect/IO/EncodeOperations.h"
#include "Hect/IO/ReadStream.h"
#include "Hect/Reflection/Enum.h"

namespace hect
{

class AssetCache;

///
/// Provides access for decoding structured data from a persistent format.
class Decoder
{
public:
    Decoder();
    Decoder(AssetCache& assetCache);
    virtual ~Decoder();

    AssetCache& assetCache();

    ///
    /// Returns whether the decoder is reading from a binary stream.
    virtual bool isBinaryStream() const = 0;

    ///
    /// Returns the raw binary stream.
    ///
    /// \throws Error If the decoder is not reading from a binary stream.
    virtual ReadStream& binaryStream() = 0;

    virtual void beginArray() = 0;
    virtual void endArray() = 0;

    virtual bool hasMoreElements() const = 0;

    virtual void beginObject() = 0;
    virtual void endObject() = 0;

    virtual bool selectMember(const char* name) = 0;
    virtual std::vector<std::string> memberNames() const = 0;

    virtual std::string decodeString() = 0;
    virtual int8_t decodeInt8() = 0;
    virtual uint8_t decodeUInt8() = 0;
    virtual int16_t decodeInt16() = 0;
    virtual uint16_t decodeUInt16() = 0;
    virtual int32_t decodeInt32() = 0;
    virtual uint32_t decodeUInt32() = 0;
    virtual int64_t decodeInt64() = 0;
    virtual uint64_t decodeUInt64() = 0;
    virtual float decodeFloat32() = 0;
    virtual double decodeFloat64() = 0;
    virtual bool decodeBool() = 0;

private:
    AssetCache* _assetCache;
};

Decoder& operator>>(Decoder& decoder, const BeginArray& beginArray);
Decoder& operator>>(Decoder& decoder, const EndArray&);

Decoder& operator>>(Decoder& decoder, const BeginObject& beginObject);
Decoder& operator>>(Decoder& decoder, const EndObject&);

template <typename T>
Decoder& operator>>(Decoder& decoder, const DecodeValue<T>& decodeValue);

template <typename T>
Decoder& operator>>(Decoder& decoder, const DecodeEnum<T>& decodeEnum);

Decoder& operator>>(Decoder& decoder, std::string& value);
Decoder& operator>>(Decoder& decoder, int8_t& value);
Decoder& operator>>(Decoder& decoder, uint8_t& value);
Decoder& operator>>(Decoder& decoder, int16_t& value);
Decoder& operator>>(Decoder& decoder, uint16_t& value);
Decoder& operator>>(Decoder& decoder, int32_t& value);
Decoder& operator>>(Decoder& decoder, uint32_t& value);
Decoder& operator>>(Decoder& decoder, int64_t& value);
Decoder& operator>>(Decoder& decoder, uint64_t& value);
Decoder& operator>>(Decoder& decoder, float& value);
Decoder& operator>>(Decoder& decoder, double& value);
Decoder& operator>>(Decoder& decoder, bool& value);

}

#include "Decoder.inl"