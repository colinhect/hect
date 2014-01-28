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
#include "TextureEncoder.h"

using namespace hect;

void TextureEncoder::encode(const Texture& texture, ObjectEncoder& encoder)
{
    texture;
    encoder;
    throw Error("Not implemented");
}

void TextureEncoder::decode(Texture& texture, ObjectDecoder& decoder, AssetCache& assetCache)
{
    // Image
    Path imagePath = decoder.decodeString("image");
    AssetHandle<Image> image = assetCache.getHandle<Image>(imagePath);
    texture.setImage(image);

    // Min filter
    if (decoder.hasMember("minFilter"))
    {
        texture.setMinFilter(textureFilterFromString(decoder.decodeString("minFilter")));
    }

    // Mag filter
    if (decoder.hasMember("magFilter"))
    {
        texture.setMagFilter(textureFilterFromString(decoder.decodeString("magFilter")));
    }

    // Wrapped
    if (decoder.hasMember("wrapped"))
    {
        texture.setWrapped(decoder.decodeBool("wrapped"));
    }

    // Mipmapped
    if (decoder.hasMember("mipmapped"))
    {
        texture.setMipmapped(decoder.decodeBool("mipmapped"));
    }
}

TextureFilter TextureEncoder::textureFilterFromString(const std::string& value)
{
    static std::map<std::string, TextureFilter> textureFilters;

    if (textureFilters.empty())
    {
        textureFilters["Nearest"] = TextureFilter::Nearest;
        textureFilters["Linear"] = TextureFilter::Linear;
    }

    auto it = textureFilters.find(value);
    if (it == textureFilters.end())
    {
        throw Error(format("Invalid texture filter '%s'", value.c_str()));
    }

    return (*it).second;
}