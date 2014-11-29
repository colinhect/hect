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

using namespace hect;

Texture::Texture()
{
}

Texture::Texture(const std::string& name, unsigned width, unsigned height, PixelType pixelType, PixelFormat pixelFormat, TextureFilter minFilter, TextureFilter magFilter, bool mipmapped, bool wrapped) :
    Asset(name),
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
    if (type == TextureType_CubeMap)
    {
        _wrapped = false;
    }
}

Texture::ImageSequence Texture::sourceImages()
{
    return _sourceImages;
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

unsigned Texture::bytesPerPixel() const
{
    static const unsigned _bytesPerPixelLookUp[2][3] =
    {
        { 3, 6, 12 },
        { 4, 8, 16 }
    };

    return _bytesPerPixelLookUp[_pixelFormat][_pixelType];
}

size_t Texture::memoryUsage() const
{
    return _width * _height * bytesPerPixel();
}

bool Texture::operator==(const Texture& texture) const
{
    if (_type != texture._type)
    {
        return false;
    }

    if (_sourceImages.size() != texture._sourceImages.size())
    {
        return false;
    }

    for (size_t i = 0; i < _sourceImages.size(); ++i)
    {
        if (_sourceImages[i] != texture._sourceImages[i])
        {
            return false;
        }
    }

    if (_width != texture._width && _height != texture._height)
    {
        return false;
    }

    if (_pixelType != texture._pixelType && _pixelFormat != texture._pixelFormat)
    {
        return false;
    }

    if (_minFilter != texture._minFilter && _magFilter != texture._magFilter)
    {
        return false;
    }

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

namespace hect
{

Encoder& operator<<(Encoder& encoder, const Texture& texture)
{
    return encoder << beginObject()
           << encodeEnum("type", texture._type)
           << encodeVector("images", texture._sourceImages)
           << encodeEnum("minFilter", texture._minFilter)
           << encodeEnum("magFilter", texture._magFilter)
           << encodeValue("wrapped", texture._wrapped)
           << encodeValue("mipmapped", texture._mipmapped)
           << endObject();
}

Decoder& operator>>(Decoder& decoder, Texture& texture)
{
    decoder >> beginObject();

    // Type
    if (decoder.selectMember("type"))
    {
        TextureType type;
        decoder >> decodeEnum(type);
        texture.setType(type);
    }

    // Images
    if (decoder.selectMember("images"))
    {
        std::vector<AssetHandle<Image>> images;
        decoder >> decodeVector(images);

        for (AssetHandle<Image>& image : images)
        {
            texture.addSourceImage(image);
        }
    }

    decoder >> decodeEnum("minFilter", texture._minFilter)
            >> decodeEnum("magFilter", texture._magFilter)
            >> decodeValue("wrapped", texture._wrapped)
            >> decodeValue("mipmapped", texture._mipmapped);

    decoder >> endObject();

    return decoder;
}

}