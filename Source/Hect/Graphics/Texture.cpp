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
#include "Texture.h"

#include "Hect/Core/Format.h"
#include "Hect/Core/Error.h"
#include "Hect/Graphics/Renderer.h"

using namespace hect;

Texture::Texture() :
    _image(new Image()),
    _width(_image->width()),
    _height(_image->height()),
    _pixelType(_image->pixelType()),
    _pixelFormat(_image->pixelFormat()),
    _minFilter(TextureFilter::Linear),
    _magFilter(TextureFilter::Linear),
    _mipmapped(true),
    _wrapped(false)
{
}

Texture::Texture(const std::string& name) :
    _name(name),
    _image(new Image()),
    _width(_image->width()),
    _height(_image->height()),
    _pixelType(_image->pixelType()),
    _pixelFormat(_image->pixelFormat()),
    _minFilter(TextureFilter::Linear),
    _magFilter(TextureFilter::Linear),
    _mipmapped(true),
    _wrapped(false)
{
}

Texture::Texture(const std::string& name, unsigned width, unsigned height, PixelType pixelType, PixelFormat pixelFormat, TextureFilter minFilter, TextureFilter magFilter, bool mipmapped, bool wrapped) :
    _name(name),
    _image(new Image(width, height, pixelType, pixelFormat)),
    _width(width),
    _height(height),
    _pixelType(pixelType),
    _pixelFormat(pixelFormat),
    _minFilter(minFilter),
    _magFilter(magFilter),
    _mipmapped(mipmapped),
    _wrapped(wrapped)
{
}

Texture::Texture(const std::string& name, const AssetHandle<Image>& image) :
    _name(name),
    _image(image),
    _width(_image->width()),
    _height(_image->height()),
    _pixelType(_image->pixelType()),
    _pixelFormat(_image->pixelFormat()),
    _minFilter(TextureFilter::Linear),
    _magFilter(TextureFilter::Linear),
    _mipmapped(true),
    _wrapped(false)
{
}

Texture::Texture(const Texture& texture) :
    _name(texture._name),
    _image(texture._image),
    _width(texture.width()),
    _height(texture.height()),
    _pixelType(texture.pixelType()),
    _pixelFormat(texture.pixelFormat()),
    _minFilter(texture.minFilter()),
    _magFilter(texture.magFilter()),
    _mipmapped(texture.isMipmapped()),
    _wrapped(texture.isWrapped())
{
    if (texture.isUploaded())
    {
        // Download the image for the source texture and copy it as the source
        // image for this texture
        _image = AssetHandle<Image>(new Image(texture.renderer().downloadTextureImage(texture)));
    }
}

Texture::~Texture()
{
    if (isUploaded())
    {
        renderer().destroyTexture(*this);
    }
}

const std::string& Texture::name() const
{
    return _name;
}

TextureFilter Texture::minFilter() const
{
    return _minFilter;
}

void Texture::setMinFilter(TextureFilter filter)
{
    if (isUploaded())
    {
        throw Error("Attempt to set the min filter of a texture that is uploaded");
    }

    _minFilter = filter;
}

TextureFilter Texture::magFilter() const
{
    return _magFilter;
}

void Texture::setMagFilter(TextureFilter filter)
{
    if (isUploaded())
    {
        throw Error("Attempt to set the mag filter of a texture that is uploaded");
    }

    _magFilter = filter;
}

bool Texture::isMipmapped() const
{
    return _mipmapped;
}

void Texture::setMipmapped(bool mipmapped)
{
    if (isUploaded())
    {
        throw Error("Attempt to set mipmapping of a texture that is uploaded");
    }

    _mipmapped = mipmapped;
}

bool Texture::isWrapped() const
{
    return _wrapped;
}

void Texture::setWrapped(bool wrapped)
{
    if (isUploaded())
    {
        throw Error("Attempt to set wrapping of a texture that is uploaded");
    }

    _wrapped = wrapped;
}

unsigned Texture::width() const
{
    return _width;
}

unsigned Texture::height() const
{
    return _height;
}

PixelFormat Texture::pixelFormat() const
{
    return _pixelFormat;
}

PixelType Texture::pixelType() const
{
    return _pixelType;
}

int Texture::bytesPerPixel() const
{
    int componentCount = 0;

    switch (_pixelFormat)
    {
    case PixelFormat::Rgb:
        componentCount = 3;
        break;
    case PixelFormat::Rgba:
        componentCount = 4;
        break;
    }

    switch (_pixelType)
    {
    case PixelType::Half:
        return componentCount * 2;
        break;
    case PixelType::Float:
        return componentCount * 4;
        break;
    case PixelType::Byte:
        return componentCount * 1;
        break;
    }

    return 0;
}

void Texture::encode(ObjectEncoder& encoder) const
{
    encoder;
    throw Error("Not implemented");
}

void Texture::decode(ObjectDecoder& decoder, AssetCache& assetCache)
{
    // Image
    Path imagePath = decoder.decodeString("image");
    AssetHandle<Image> image = assetCache.getHandle<Image>(imagePath);
    *this = Texture(_name, image);

    // Min filter
    if (decoder.hasMember("minFilter"))
    {
        setMinFilter(_parseTextureFilter(decoder.decodeString("minFilter")));
    }

    // Mag filter
    if (decoder.hasMember("magFilter"))
    {
        setMagFilter(_parseTextureFilter(decoder.decodeString("magFilter")));
    }

    // Wrapped
    if (decoder.hasMember("wrapped"))
    {
        setWrapped(decoder.decodeBool("wrapped"));
    }

    // Mipmapped
    if (decoder.hasMember("mipmapped"))
    {
        setMipmapped(decoder.decodeBool("mipmapped"));
    }
}

TextureFilter Texture::_parseTextureFilter(const std::string& value)
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