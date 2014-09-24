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
    _width(0),
    _height(0),
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
    AssetHandle<Image> sourceImage(new Image());
    sourceImage->setWidth(width);
    sourceImage->setHeight(height);
    sourceImage->setPixelType(pixelType);
    sourceImage->setPixelFormat(pixelFormat);
    addSourceImage(sourceImage);
}

Texture::Texture(const std::string& name, const AssetHandle<Image>& image) :
    _name(name),
    _type(TextureType_2D),
    _width(0),
    _height(0),
    _minFilter(TextureFilter_Linear),
    _magFilter(TextureFilter_Linear),
    _mipmapped(true),
    _wrapped(false)
{
    addSourceImage(image);
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

Sequence<AssetHandle<Image>> Texture::sourceImages()
{
    return Sequence<AssetHandle<Image>>(_sourceImages.begin(), _sourceImages.end());
}

void Texture::addSourceImage(const AssetHandle<Image>& image)
{
    if (_type == TextureType_2D)
    {
        if (!_sourceImages.empty())
        {
            throw Error("A 2-dimensional texture cannot have more than one source image");
        }
    }
    else if (_type == TextureType_CubeMap)
    {
        if (_sourceImages.size() >= 6)
        {
            throw Error("A cube map texture cannot have more than six source images");
        }
    }

    if (isUploaded())
    {
        renderer().destroyTexture(*this);
    }

    if (!_sourceImages.empty())
    {
        if (_width != image->width() || _height != image->height())
        {
            throw Error("The source image does not match the dimensions of the texture");
        }
        else if (_pixelType != image->pixelType())
        {
            throw Error("The source image pixel type does not match the pixel type of the texture");
        }
        else if (_pixelFormat != image->pixelFormat())
        {
            throw Error("The source image pixel format does not match the pixel format of the texture");
        }
    }
    else
    {
        _width = image->width();
        _height = image->height();
        _pixelType = image->pixelType();
        _pixelFormat = image->pixelFormat();
    }

    _sourceImages.push_back(image);
}

void Texture::clearSourceImages()
{

    if (isUploaded())
    {
        renderer().destroyTexture(*this);
    }

    _sourceImages.clear();
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
    case PixelType_Float16:
        return componentCount * 2;
        break;
    case PixelType_Float32:
        return componentCount * 4;
        break;
    case PixelType_UInt8:
        return componentCount * 1;
        break;
    }

    return 0;
}

void Texture::encode(Encoder& encoder) const
{
    TextureEncoder::encode(*this, encoder);
}

void Texture::decode(ObjectDecoder& decoder, AssetCache& assetCache)
{
    TextureEncoder::decode(*this, decoder, assetCache);
}