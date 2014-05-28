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

#include "Hect/Core/Export.h"
#include "Hect/Asset/AssetCache.h"
#include "Hect/IO/JsonValue.h"
#include "Hect/IO/Encoder.h"
#include "Hect/IO/Decoder.h"

namespace hect
{

///
/// An object which can be encoded and decoded.
class HECT_API Encodable
{
public:

    ///
    /// Encodes the object.
    ///
    /// \param encoder The encoder to use.
    virtual void encode(ObjectEncoder& encoder) const;

    ///
    /// Encodes the object to a JSON value.
    ///
    /// \returns The resulting JSON value.
    JsonValue encodeToJsonValue() const;

    ///
    /// Encodes the object to binary stream.
    ///
    /// \param stream The stream.
    void encodeToBinary(WriteStream& stream) const;

    ///
    /// Decodes the object.
    ///
    /// \param decoder The decoder to use.
    /// \param assetCache The asset cache to get referenced assets from.
    virtual void decode(ObjectDecoder& decoder, AssetCache& assetCache);

    ///
    /// Decodes the object from a JSON value.
    ///
    /// \param jsonValue The JSON value.
    void decodeFromJsonValue(const JsonValue& jsonValue);

    ///
    /// Decodes the object from a JSON value.
    ///
    /// \param jsonValue The JSON value.
    /// \param assetCache The asset cache to get referenced assets from.
    void decodeFromJsonValue(const JsonValue& jsonValue, AssetCache& assetCache);

    ///
    /// Decodes the object from binary a stream
    ///
    /// \param stream The stream.
    void decodeFromBinary(ReadStream& stream);

    ///
    /// Decodes the object from binary a stream
    ///
    /// \param stream The stream.
    /// \param assetCache The asset cache to get referenced assets from.
    void decodeFromBinary(ReadStream& stream, AssetCache& assetCache);
};

}