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

#include "Hect/Asset/AssetCache.h"
#include "Hect/IO/DataWriter.h"
#include "Hect/IO/DataReader.h"

namespace hect
{

///
/// An object which can be serialized and deserialized.
class Serializable
{
public:

    ///
    /// Serializes the object using a writer.
    ///
    /// \param writer The writer.
    virtual void save(ObjectWriter& writer) const;

    ///
    /// Serializes the object to a data value.
    ///
    /// \returns The resulting data value.
    DataValue saveToDataValue() const;

    ///
    /// Serializes the object to a stream.
    ///
    /// \param stream The stream.
    void saveToBinaryStream(WriteStream& stream) const;

    ///
    /// Deserializes the object using a reader.
    ///
    /// \param reader The reader.
    /// \param assetCache The asset cache to load referenced assets from.
    virtual void load(ObjectReader& reader, AssetCache& assetCache);

    ///
    /// Deserializes the object from a data value.
    ///
    /// \param dataValue The data value.
    /// \param assetCache The asset cache to load referenced assets from.
    void loadFromDataValue(const DataValue& dataValue, AssetCache& assetCache);

    ///
    /// Deserializes the object from a stream
    ///
    /// \param stream The stream.
    /// \param assetCache The asset cache to load referenced assets from.
    void loadFromBinaryStream(ReadStream& stream, AssetCache& assetCache);
};

}