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
#include "Texture2.h"

#include "Hect/Core/Format.h"
#include "Hect/Core/Exception.h"

using namespace hect;

Texture2::Texture2()
{
}

Texture2::Texture2(Name name, unsigned width, unsigned height) :
    Asset(name),
    _width(width),
    _height(height)
{
}

Texture2::Texture2(Name name, const Image::Handle& image) :
    Asset(name)
{
    setImage(image);
}

Image& Texture2::image()
{
    if (!_image)
    {
        _image = Image::Handle(new Image(_width, _height, _pixelFormat));

        if (isUploaded())
        {
            renderer().downloadTextureImage(*this);
        }
    }

    return *_image;
}

void Texture2::setImage(const Image::Handle& image)
{
    destroyIfUploaded();

    // If the texture is empty
    if (_width == 0 && _height == 0)
    {
        // Use the width/height/pixel format of the image
        _width = image->width();
        _height = image->height();
        _pixelFormat = image->pixelFormat();
    }
    else if (_width != image->width() || _height != image->height() || _pixelFormat != image->pixelFormat())
    {
        throw InvalidOperation("Image is incompatible with the texture");
    }

    _image = image;
}

void Texture2::invalidateLocalImage()
{
    if (!isUploaded())
    {
        throw InvalidOperation("Texture is not uploaded");
    }

    _image = Image::Handle();
}

Color Texture2::readPixel(unsigned x, unsigned y)
{
    return image().readPixel(x, y);
}

Color Texture2::readPixel(Vector2 coords)
{
    return image().readPixel(coords);
}

TextureFilter Texture2::minFilter() const
{
    return _minFilter;
}

void Texture2::setMinFilter(TextureFilter filter)
{
    destroyIfUploaded();
    _minFilter = filter;
}

TextureFilter Texture2::magFilter() const
{
    return _magFilter;
}

void Texture2::setMagFilter(TextureFilter filter)
{
    destroyIfUploaded();
    _magFilter = filter;
}

bool Texture2::isMipmapped() const
{
    return _mipmapped;
}

void Texture2::setMipmapped(bool mipmapped)
{
    destroyIfUploaded();
    _mipmapped = mipmapped;
}

bool Texture2::isWrapped() const
{
    return _wrapped;
}

void Texture2::setWrapped(bool wrapped)
{
    destroyIfUploaded();
    _wrapped = wrapped;
}

unsigned Texture2::width() const
{
    return _width;
}

unsigned Texture2::height() const
{
    return _height;
}

PixelFormat Texture2::pixelFormat() const
{
    return _pixelFormat;
}

void Texture2::setPixelFormat(PixelFormat pixelFormat)
{
    destroyIfUploaded();
    _pixelFormat = pixelFormat;
}
    
bool Texture2::operator==(const Texture2& texture) const
{
    // Image
    if (_image != texture._image)
    {
        return false;
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

bool Texture2::operator!=(const Texture2& texture) const
{
    return !(*this == texture);
}

void Texture2::encode(Encoder& encoder) const
{
    encoder << encodeValue("image", _image)
            << encodeEnum("minFilter", _minFilter)
            << encodeEnum("magFilter", _magFilter)
            << encodeValue("wrapped", _wrapped)
            << encodeValue("mipmapped", _mipmapped);
}

void Texture2::decode(Decoder& decoder)
{
    // Color space
    ColorSpace colorSpace = ColorSpace::NonLinear;
    if (decoder.selectMember("colorSpace"))
    {
        decoder >> decodeEnum(colorSpace);
    }

    // Images
    if (decoder.selectMember("image"))
    {
        Image::Handle image;
        decoder >> decodeValue(image);

        // Remove the image from the asset cache because we don't want to
        // store uncompressed image data in main memory
        decoder.assetCache().remove(image.path());

        image->setColorSpace(colorSpace);
        setImage(image);
    }

    decoder >> decodeEnum("minFilter", _minFilter)
            >> decodeEnum("magFilter", _magFilter)
            >> decodeValue("wrapped", _wrapped)
            >> decodeValue("mipmapped", _mipmapped);
}

void Texture2::destroyIfUploaded()
{
    if (isUploaded())
    {
        renderer().destroyTexture(*this);
    }
}
