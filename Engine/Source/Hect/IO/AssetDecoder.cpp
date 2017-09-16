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
#include "AssetDecoder.h"

#include "Hect/IO/AssetCache.h"
#include "Hect/IO/BinaryDecoder.h"
#include "Hect/IO/DataValueDecoder.h"

namespace hect
{

AssetDecoder::AssetDecoder(AssetCache& asset_cache, const Path& path) :
    Decoder(asset_cache)
{
    Path resolved_path = asset_cache.resolve_path(path);

    auto stream = asset_cache.file_system().open_file_for_read(resolved_path);
    if (is_yaml(*stream))
    {
        _data_value.decode_from_yaml(*stream);
        _implementation.reset(new DataValueDecoder(_data_value, asset_cache));
    }
    else
    {
        size_t length = stream->length();
        _data.resize(length);
        stream->read(&_data[0], length);

        _stream.reset(new MemoryReadStream(_data));
        _implementation.reset(new BinaryDecoder(*_stream, asset_cache));
    }

    asset_cache.push_directory(resolved_path.parent_directory());
}

AssetDecoder::~AssetDecoder()
{
    asset_cache().pop_directory();
}

bool AssetDecoder::is_binary_stream() const
{
    assert(_implementation);
    return _implementation->is_binary_stream();
}

ReadStream& AssetDecoder::binary_stream()
{
    assert(_implementation);
    return _implementation->binary_stream();
}

void AssetDecoder::begin_array()
{
    assert(_implementation);
    _implementation->begin_array();
}

void AssetDecoder::end_array()
{
    assert(_implementation);
    _implementation->end_array();
}

bool AssetDecoder::has_more_elements() const
{
    assert(_implementation);
    return _implementation->has_more_elements();
}

void AssetDecoder::begin_object()
{
    assert(_implementation);
    return _implementation->begin_object();
}

void AssetDecoder::end_object()
{
    assert(_implementation);
    return _implementation->end_object();
}

bool AssetDecoder::select_member(const char* name)
{
    assert(_implementation);
    return _implementation->select_member(name);
}

std::vector<std::string> AssetDecoder::member_names() const
{
    assert(_implementation);
    return _implementation->member_names();
}

std::string AssetDecoder::decode_string()
{
    assert(_implementation);
    return _implementation->decode_string();
}

int8_t AssetDecoder::decode_int8()
{
    assert(_implementation);
    return _implementation->decode_int8();
}

uint8_t AssetDecoder::decode_uint8()
{
    assert(_implementation);
    return _implementation->decode_uint8();
}

int16_t AssetDecoder::decode_int16()
{
    assert(_implementation);
    return _implementation->decode_int16();
}

uint16_t AssetDecoder::decode_uint16()
{
    assert(_implementation);
    return _implementation->decode_uint16();
}

int32_t AssetDecoder::decode_int32()
{
    assert(_implementation);
    return _implementation->decode_int32();
}

uint32_t AssetDecoder::decode_uint32()
{
    assert(_implementation);
    return _implementation->decode_uint32();
}

int64_t AssetDecoder::decode_int64()
{
    assert(_implementation);
    return _implementation->decode_int64();
}

uint64_t AssetDecoder::decode_uint64()
{
    assert(_implementation);
    return _implementation->decode_uint64();
}

float AssetDecoder::decode_float32()
{
    assert(_implementation);
    return _implementation->decode_float32();
}

double AssetDecoder::decode_float64()
{
    assert(_implementation);
    return _implementation->decode_float64();
}

bool AssetDecoder::decode_bool()
{
    assert(_implementation);
    return _implementation->decode_bool();
}

bool AssetDecoder::is_yaml(ReadStream& stream)
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
