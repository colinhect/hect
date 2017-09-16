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
#include "Decoder.h"

namespace hect
{

Decoder::Decoder()
{
}

Decoder::Decoder(AssetCache& asset_cache) :
    _asset_cache(&asset_cache)
{
}

AssetCache& Decoder::asset_cache()
{
    if (!_asset_cache)
    {
        throw InvalidOperation("Decoder does not have an asset cache");
    }
    return *_asset_cache;
}

Decoder& operator>>(Decoder& decoder, const BeginArray& begin_array)
{
    if (begin_array.name)
    {
        decoder.select_member(begin_array.name);
    }
    decoder.begin_array();
    return decoder;
}

Decoder& operator>>(Decoder& decoder, const EndArray&)
{
    decoder.end_array();
    return decoder;
}

Decoder& operator>>(Decoder& decoder, const BeginObject& begin_object)
{
    if (begin_object.name)
    {
        decoder.select_member(begin_object.name);
    }
    decoder.begin_object();
    return decoder;
}

Decoder& operator>>(Decoder& decoder, const EndObject&)
{
    decoder.end_object();
    return decoder;
}

Decoder& operator>>(Decoder& decoder, std::string& value)
{
    value = decoder.decode_string();
    return decoder;
}

Decoder& operator>>(Decoder& decoder, int8_t& value)
{
    value = decoder.decode_int8();
    return decoder;
}

Decoder& operator>>(Decoder& decoder, uint8_t& value)
{
    value = decoder.decode_uint8();
    return decoder;
}

Decoder& operator>>(Decoder& decoder, int16_t& value)
{
    value = decoder.decode_int16();
    return decoder;
}

Decoder& operator>>(Decoder& decoder, uint16_t& value)
{
    value = decoder.decode_uint16();
    return decoder;
}

Decoder& operator>>(Decoder& decoder, int32_t& value)
{
    value = decoder.decode_int32();
    return decoder;
}

Decoder& operator>>(Decoder& decoder, uint32_t& value)
{
    value = decoder.decode_uint32();
    return decoder;
}

Decoder& operator>>(Decoder& decoder, int64_t& value)
{
    value = decoder.decode_int64();
    return decoder;
}

Decoder& operator>>(Decoder& decoder, uint64_t& value)
{
    value = decoder.decode_uint64();
    return decoder;
}

Decoder& operator>>(Decoder& decoder, float& value)
{
    value = decoder.decode_float32();
    return decoder;
}

Decoder& operator>>(Decoder& decoder, double& value)
{
    value = decoder.decode_float64();
    return decoder;
}

Decoder& operator>>(Decoder& decoder, bool& value)
{
    value = decoder.decode_bool();
    return decoder;
}

}