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
#include "Encodable.h"

#include "Hect/IO/BinaryDecoder.h"
#include "Hect/IO/BinaryEncoder.h"
#include "Hect/IO/JsonDecoder.h"
#include "Hect/IO/JsonEncoder.h"
#include "Hect/IO/MemoryReadStream.h"

using namespace hect;

void Encodable::encode(ObjectEncoder& encoder) const
{
    encoder;
}

JsonValue Encodable::encodeToJsonValue() const
{
    JsonEncoder encoder;
    {
        ObjectEncoder objectEncoder = encoder.encodeObject();
        encode(objectEncoder);
    }
    return *encoder.jsonValues().begin();
}

void Encodable::encodeToBinary(WriteStream& stream) const
{
    BinaryEncoder encoder(stream);
    ObjectEncoder objectEncode = encoder.encodeObject();
    encode(objectEncode);
}

void Encodable::decode(ObjectDecoder& decoder, AssetCache& assetCache)
{
    decoder;
    assetCache;
}

void Encodable::decodeFromJsonValue(const JsonValue& jsonValue)
{
    AssetCache assetCache;
    decodeFromJsonValue(jsonValue, assetCache);
}

void Encodable::decodeFromJsonValue(const JsonValue& jsonValue, AssetCache& assetCache)
{
    JsonDecoder decoder(jsonValue);
    ObjectDecoder objectDecoder = decoder.decodeObject();
    decode(objectDecoder, assetCache);
}

void Encodable::decodeFromBinary(ReadStream& stream)
{
    AssetCache assetCache;
    decodeFromBinary(stream, assetCache);
}

void Encodable::decodeFromBinary(ReadStream& stream, AssetCache& assetCache)
{
    BinaryDecoder decoder(stream);
    ObjectDecoder objectDecoder = decoder.decodeObject();
    decode(objectDecoder, assetCache);
}

void Encodable::decodeFromData(const Data& data)
{
    if (!data.jsonValue.isNull())
    {
        decodeFromJsonValue(data.jsonValue);
    }
    else if (!data.binaryData.empty())
    {
        MemoryReadStream stream(data.binaryData);
        decodeFromBinary(stream);
    }
}

void Encodable::decodeFromData(const Data& data, AssetCache& assetCache)
{
    if (!data.jsonValue.isNull())
    {
        AssetCache::SelectDirectoryScope scope(assetCache, data.assetPath.parentDirectory());
        decodeFromJsonValue(data.jsonValue, assetCache);
    }
    else if (!data.binaryData.empty())
    {
        MemoryReadStream stream(data.binaryData);
        decodeFromBinary(stream, assetCache);
    }
}