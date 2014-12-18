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
#include "AssetDecoder.h"

#include "Hect/IO/AssetCache.h"
#include "Hect/IO/BinaryDecoder.h"
#include "Hect/IO/DataValueDecoder.h"

namespace hect
{

AssetDecoder::AssetDecoder(AssetCache& assetCache, const Path& path) :
    Decoder(assetCache)
{
    Path resolvedPath = assetCache.resolvePath(path);

    auto stream = assetCache.fileSystem().openFileForRead(resolvedPath);
    if (isYaml(*stream))
    {
        _dataValue.decodeFromYaml(*stream);
        _implementation.reset(new DataValueDecoder(_dataValue, assetCache));
    }
    else
    {
        size_t length = stream->length();
        _data.resize(length);
        stream->read(&_data[0], length);

        _stream.reset(new MemoryReadStream(_data));
        _implementation.reset(new BinaryDecoder(*_stream, assetCache));
    }

    assetCache.selectDirectory(path.parentDirectory());
}

AssetDecoder::~AssetDecoder()
{
    assetCache().restoreDirectory();
}

bool AssetDecoder::isBinaryStream() const
{
    assert(_implementation);
    return _implementation->isBinaryStream();
}

ReadStream& AssetDecoder::binaryStream()
{
    assert(_implementation);
    return _implementation->binaryStream();
}

void AssetDecoder::beginArray()
{
    assert(_implementation);
    _implementation->beginArray();
}

void AssetDecoder::endArray()
{
    assert(_implementation);
    _implementation->endArray();
}

bool AssetDecoder::hasMoreElements() const
{
    assert(_implementation);
    return _implementation->hasMoreElements();
}

void AssetDecoder::beginObject()
{
    assert(_implementation);
    return _implementation->beginObject();
}

void AssetDecoder::endObject()
{
    assert(_implementation);
    return _implementation->endObject();
}

bool AssetDecoder::selectMember(const char* name)
{
    assert(_implementation);
    return _implementation->selectMember(name);
}

std::vector<std::string> AssetDecoder::memberNames() const
{
    assert(_implementation);
    return _implementation->memberNames();
}

std::string AssetDecoder::decodeString()
{
    assert(_implementation);
    return _implementation->decodeString();
}

int8_t AssetDecoder::decodeInt8()
{
    assert(_implementation);
    return _implementation->decodeInt8();
}

uint8_t AssetDecoder::decodeUInt8()
{
    assert(_implementation);
    return _implementation->decodeUInt8();
}

int16_t AssetDecoder::decodeInt16()
{
    assert(_implementation);
    return _implementation->decodeInt16();
}

uint16_t AssetDecoder::decodeUInt16()
{
    assert(_implementation);
    return _implementation->decodeUInt16();
}

int32_t AssetDecoder::decodeInt32()
{
    assert(_implementation);
    return _implementation->decodeInt32();
}

uint32_t AssetDecoder::decodeUInt32()
{
    assert(_implementation);
    return _implementation->decodeUInt32();
}

int64_t AssetDecoder::decodeInt64()
{
    assert(_implementation);
    return _implementation->decodeInt64();
}

uint64_t AssetDecoder::decodeUInt64()
{
    assert(_implementation);
    return _implementation->decodeUInt64();
}

float AssetDecoder::decodeFloat32()
{
    assert(_implementation);
    return _implementation->decodeFloat32();
}

double AssetDecoder::decodeFloat64()
{
    assert(_implementation);
    return _implementation->decodeFloat64();
}

bool AssetDecoder::decodeBool()
{
    assert(_implementation);
    return _implementation->decodeBool();
}

bool AssetDecoder::isYaml(ReadStream& stream)
{
    if (stream.length() >= 3)
    {
        uint8_t beginning[3];
        stream.read(&beginning[0], 3);
        stream.seek(0);
        if (beginning[0] == '-' && beginning[1] == '-' && beginning[2] == '-')
        {
            return true;
        }
    }

    return false;
}

}
