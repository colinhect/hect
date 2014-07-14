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
#include "Hect/IO/Encoders/TextureEncoder.h"

using namespace hect;

Texture::Texture() :
    _type(TextureType_2D),
    _minFilter(TextureFilter_Linear),
    _magFilter(TextureFilter_Linear),
    _mipmapped(true),
    _wrapped(false)
{
}

Texture::Texture(const std::string& name, TextureType type) :
    _name(name),
    _type(type),
    _minFilter(TextureFilter_Linear),
    _magFilter(TextureFilter_Linear),
    _mipmapped(true),
    _wrapped(false)
{
}

Texture::Texture(const std::string& name, unsigned width, unsigned height, PixelType pixelType, PixelFormat pixelFormat, TextureFilter minFilter, TextureFilter magFilter, bool mipmapped, bool wrapped) :
    _name(name),
    _type(TextureType_2D),
    _width(width),
    _height(height),
    _pixelType(pixelType),
    _pixelFormat(pixelFormat),
    _minFilter(minFilter),
    _magFilter(magFilter),
    _mipmapped(mipmapped),
    _wrapped(wrapped)
{
    addImage(new Image(width, height, pixelType, pixelFormat));
}

Texture::Texture(const std::string& name, const AssetHandle<Image>& image) :
    _name(name),
    _type(TextureType_2D),
    _minFilter(TextureFilter_Linear),
    _magFilter(TextureFilter_Linear),
    _mipmapped(true),
    _wrapped(false)
{
    addImage(image);
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

void Texture::setName(const std::string& name)
{
    _name = name;
}

TextureType Texture::type()
{
    return _type;
}

void Texture::setType(TextureType type)
{
    _type = type;
    if (type == TextureType_CubeMap)
    {
        _wrapped = false;
    }
}

void Texture::addImage(const AssetHandle<Image>& image)
{
    if (isUploaded())
    {
        renderer().destroyTexture(*this);
    }

    _images.push_back(image);

    _width = image->width();
    _height = image->height();
    _pixelType = image->pixelType();
    _pixelFormat = image->pixelFormat();
}

AssetHandle<Image>::Array& Texture::images()
{
    return _images;
}

TextureFilter Texture::minFilter() const
{
    return _minFilter;
}

void Texture::setMinFilter(TextureFilter filter)
{
    if (isUploaded())
    {
        renderer().destroyTexture(*this);
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
        renderer().destroyTexture(*this);
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
        renderer().destroyTexture(*this);
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
        renderer().destroyTexture(*this);
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
    case PixelFormat_Rgb:
        componentCount = 3;
        break;
    case PixelFormat_Rgba:
        componentCount = 4;
        break;
    }

    switch (_pixelType)
    {
    case PixelType_Half:
        return componentCount * 2;
        break;
    case PixelType_Float:
        return componentCount * 4;
        break;
    case PixelType_Byte:
        return componentCount * 1;
        break;
    }

    return 0;
}

void Texture::encode(ObjectEncoder& encoder) const
{
    TextureEncoder::encode(*this, encoder);
}

void Texture::decode(ObjectDecoder& decoder, AssetCache& assetCache)
{
    TextureEncoder::decode(*this, decoder, assetCache);
}