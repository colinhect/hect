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
#include "Hect.h"

using namespace hect;

void TextureDataValueFormat::load(Texture& texture, const std::string& name, const DataValue& dataValue, AssetCache& assetCache)
{
    // Image
    if (dataValue["image"].isString())
    {
        AssetHandle<Image> handle = assetCache.getHandle<Image>(dataValue["image"].asString());
        texture = Texture(name, handle.getShared());
    }

    // Min filter
    if (dataValue["minFilter"].isString())
    {
        texture.setMinFilter(_parseTextureFilter(dataValue["minFilter"]));
    }

    // Mag filter
    if (dataValue["magFilter"].isString())
    {
        texture.setMagFilter(_parseTextureFilter(dataValue["magFilter"]));
    }

    // Wrapped
    if (dataValue["wrapped"].isBool())
    {
        texture.setWrapped(dataValue["wrapped"].asBool());
    }

    // Mipmapped
    if (dataValue["mipmapped"].isBool())
    {
        texture.setMipmapped(dataValue["mipmapped"].asBool());
    }
}

TextureFilter TextureDataValueFormat::_parseTextureFilter(const DataValue& dataValue)
{
    static std::map<std::string, TextureFilter> textureFilters;

    if (textureFilters.empty())
    {
        textureFilters["Nearest"] = TextureFilter::Nearest;
        textureFilters["Linear"] = TextureFilter::Linear;
    }

    auto it = textureFilters.find(dataValue.asString());
    if (it == textureFilters.end())
    {
        throw Error(format("Invalid texture filter '%s'", dataValue.asString().c_str()));
    }

    return (*it).second;
}