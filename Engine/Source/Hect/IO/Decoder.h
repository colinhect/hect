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

    ///
    /// Constructs a decoder.
    Decoder();

    ///
    /// Constructs a decoder.
    ///
    /// \param assetCache The asset cache to load further assets from.
    Decoder(AssetCache& assetCache);

    virtual ~Decoder() { }

    ///
    /// Returns the asset cache to use to get needed assets from while
    /// decoding.
    ///
    /// \throws Error If the decoder has no asset cache.
    AssetCache& assetCache();

    ///
    /// Returns whether the decoder is reading from a binary stream.
    virtual bool isBinaryStream() const = 0;

    ///
    /// Returns the raw binary stream.
    ///
    /// \throws Error If the decoder is not reading from a binary stream.
    virtual ReadStream& binaryStream() = 0;

    ///
    /// Begins an array.
    ///
    /// \note Values decoded following this call will be decoded as elements
    /// from the array.  Attempting to decode values past the end of the array
    /// will result in an error being thrown.  Whether there are any more
    /// elements left in the array can be queried using hasMoreElements().
    ///
    /// \throws Error If the next value to decode is not an array.
    virtual void beginArray() = 0;

    ///
    /// Ends an array.
    ///
    /// \note An array cannot be ended before all elements are decoded from it.
    ///
    /// \throws Error If an array was not started using beginArray().
    virtual void endArray() = 0;

    ///
    /// Returns whether there are more elements available to be decoded in the
    /// current array.
    ///
    /// \throws Error If an array was not started using beginArray().
    virtual bool hasMoreElements() const = 0;

    ///
    /// Begins an object.
    ///
    /// \note The specific members of an object can be targeted using
    /// selectMember().
    ///
    /// \throws Error If the next value to decode is not an object.
    virtual void beginObject() = 0;

    ///
    /// Ends an object.
    ///
    /// \throws Error If an object was not started using beginObject().
    virtual void endObject() = 0;

    ///
    /// Selects a specific member of the object.
    ///
    /// \note The following call to decode a value (included beginning an
    /// array or object) will decode from the selected member of the object.
    ///
    /// \param name The name of the member to select.
    ///
    /// \returns True if the member exists and was selected; false otherwise.
    ///
    /// \throws Error If an object was not started using beginObject().
    virtual bool selectMember(const char* name) = 0;

    ///
    /// Returns the names of the members of the object.
    ///
    /// \throws Error If an object was not started using beginObject().
    virtual std::vector<std::string> memberNames() const = 0;

    ///
    /// Decodes a string.
    ///
    /// \returns The decoded value.
    virtual std::string decodeString() = 0;

    ///
    /// Decodes an 8-bit signed integer.
    ///
    /// \returns The decoded value.
    virtual int8_t decodeInt8() = 0;

    ///
    /// Decodes an 8-bit unsigned integer.
    ///
    /// \returns The decoded value.
    virtual uint8_t decodeUInt8() = 0;

    ///
    /// Decodes a 16-bit signed integer.
    ///
    /// \returns The decoded value.
    virtual int16_t decodeInt16() = 0;

    ///
    /// Decodes a 16-bit unsigned integer.
    ///
    /// \returns The decoded value.
    virtual uint16_t decodeUInt16() = 0;

    ///
    /// Decodes a 32-bit signed integer.
    ///
    /// \returns The decoded value.
    virtual int32_t decodeInt32() = 0;

    ///
    /// Decodes a 32-bit unsigned integer.
    ///
    /// \returns The decoded value.
    virtual uint32_t decodeUInt32() = 0;

    ///
    /// Decodes a 64-bit signed integer.
    ///
    /// \returns The decoded value.
    virtual int64_t decodeInt64() = 0;

    ///
    /// Decodes a 64-bit unsigned integer.
    ///
    /// \returns The decoded value.
    virtual uint64_t decodeUInt64() = 0;

    ///
    /// Decodes a 32-bit floating point number.
    ///
    /// \returns The decoded value.
    virtual float decodeFloat32() = 0;

    ///
    /// Decodes a 64-bit floating point number.
    ///
    /// \returns The decoded value.
    virtual double decodeFloat64() = 0;

    ///
    /// Decodes a boolean.
    ///
    /// \returns The decoded value.
    virtual bool decodeBool() = 0;

private:
    AssetCache* _assetCache { nullptr };
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