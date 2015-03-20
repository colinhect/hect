///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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
#include "Hect/Core/Exception.h"

using namespace hect;

Texture::Texture()
{
}

Texture::Texture(const std::string& name, unsigned width, unsigned height, const PixelFormat& pixelFormat, TextureFilter minFilter, TextureFilter magFilter, bool mipmapped, bool wrapped) :
    Asset(name),
    _width(width),
    _height(height),
    _pixelFormat(pixelFormat),
    _minFilter(minFilter),
    _magFilter(magFilter),
    _mipmapped(mipmapped),
    _wrapped(wrapped)
{
    Image::Handle sourceImage(new Image());
    sourceImage->setWidth(width);
    sourceImage->setHeight(height);
    sourceImage->setPixelFormat(pixelFormat);
    addSourceImage(sourceImage);
}

Texture::Texture(const std::string& name, const Image::Handle& image) :
    Asset(name)
{
    addSourceImage(image);
}

TextureType Texture::type()
{
    return _type;
}

void Texture::setType(TextureType type)
{
    _type = type;
    if (type == TextureType::CubeMap)
    {
        _wrapped = false;
    }
}

Texture::ImageSequence Texture::sourceImages()
{
    return _sourceImages;
}

void Texture::addSourceImage(const Image::Handle& image)
{
    if (_type == TextureType::TwoDimensional)
    {
        if (!_sourceImages.empty())
        {
            throw InvalidOperation("A 2-dimensional texture cannot have more than one source image");
        }
    }
    else if (_type == TextureType::CubeMap)
    {
        if (_sourceImages.size() >= 6)
        {
            throw InvalidOperation("A cube map texture cannot have more than six source images");
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
            throw InvalidOperation("The source image does not match the dimensions of the texture");
        }
        else if (_pixelFormat != image->pixelFormat())
        {
            throw InvalidOperation("The source image pixel format does not match the pixel format of the texture");
        }
    }
    else
    {
        _width = image->width();
        _height = image->height();
        _pixelFormat = image->pixelFormat();
    }

    _sourceImages.push_back(image);
}

void Texture::clearSourceImages()
{
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

const PixelFormat& Texture::pixelFormat() const
{
    return _pixelFormat;
}

bool Texture::operator==(const Texture& texture) const
{
    // Type
    if (_type != texture._type)
    {
        return false;
    }

    // Source image count
    if (_sourceImages.size() != texture._sourceImages.size())
    {
        return false;
    }

    // Source images
    for (size_t i = 0; i < _sourceImages.size(); ++i)
    {
        if (_sourceImages[i] != texture._sourceImages[i])
        {
            return false;
        }
    }

    // Dimensions
    if (_width != texture._width && _height != texture._height)
    {
        return false;
    }

    // Pixel format
    if (_pixelFormat != texture._pixelFormat)
    {
        return false;
    }

    // Min/mag filters
    if (_minFilter != texture._minFilter && _magFilter != texture._magFilter)
    {
        return false;
    }

    // Mipmapped/wrapped flags
    if (_mipmapped != texture._mipmapped && _wrapped != texture._wrapped)
    {
        return false;
    }

    return true;
}

bool Texture::operator!=(const Texture& texture) const
{
    return !(*this == texture);
}

void Texture::encode(Encoder& encoder) const
{
    encoder << encodeEnum("type", _type)
            << encodeVector("images", _sourceImages)
            << encodeEnum("minFilter", _minFilter)
            << encodeEnum("magFilter", _magFilter)
            << encodeValue("wrapped", _wrapped)
            << encodeValue("mipmapped", _mipmapped);
}

void Texture::decode(Decoder& decoder)
{
    // Type
    if (decoder.selectMember("type"))
    {
        TextureType type;
        decoder >> decodeEnum(type);
        setType(type);
    }

    // Color space
    ColorSpace colorSpace = ColorSpace::NonLinear;
    if (decoder.selectMember("colorSpace"))
    {
        decoder >> decodeEnum(colorSpace);
    }

    // Images
    if (decoder.selectMember("images"))
    {
        std::vector<Image::Handle> images;
        decoder >> decodeVector(images);

        for (Image::Handle& image : images)
        {
            // Remove the image from the asset cache because we don't want to
            // store uncompressed image data in main memory
            decoder.assetCache().remove(image.path());

            image->setColorSpace(colorSpace);
            addSourceImage(image);
        }
    }

    decoder >> decodeEnum("minFilter", _minFilter)
            >> decodeEnum("magFilter", _magFilter)
            >> decodeValue("wrapped", _wrapped)
            >> decodeValue("mipmapped", _mipmapped);
}
