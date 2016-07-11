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
#include "Texture3.h"

#include "Hect/Core/Format.h"
#include "Hect/Core/Exception.h"

using namespace hect;

Texture3::Texture3()
{
}

Texture3::Texture3(Name name, unsigned width, unsigned height, unsigned depth, const PixelFormat& pixelFormat, TextureFilter minFilter, TextureFilter magFilter, bool mipmapped, bool wrapped) :
    Asset(name),
    _width(width),
    _height(height),
    _depth(depth),
    _pixelFormat(pixelFormat),
    _minFilter(minFilter),
    _magFilter(magFilter),
    _mipmapped(mipmapped),
    _wrapped(wrapped)
{
}

Image& Texture3::image(unsigned depth)
{
    bool downloadImages = false;
    if (_images.empty())
    {
        _images = std::vector<Image::Handle>(_depth);
        downloadImages = isUploaded();
    }

    if (depth >= _depth)
    {
        throw InvalidOperation("Depth is greater than the depth of the texture");
    }

    Image::Handle& image = _images[depth];
    if (!image)
    {
        image = Image::Handle(new Image(_width, _height, _pixelFormat));
    }

    if (downloadImages)
    {
        renderer().downloadTextureImages(*this);
    }

    return *image;
}

void Texture3::setImage(unsigned depth, const Image::Handle& image)
{
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

    // Ensure that the image vector is intialized and the depth is within
    // the bounds of the texture
    this->image(depth);

    _images[depth] = image;
}

void Texture3::invalidateLocalImages()
{
    if (!isUploaded())
    {
        throw InvalidOperation("Texture is not uploaded");
    }

    _images.clear();
}

Color Texture3::readPixel(unsigned x, unsigned y, unsigned z)
{
    return image(z).readPixel(x, y);
}

Color Texture3::readPixel(Vector3 coords)
{
    unsigned z = static_cast<unsigned>(coords.z * _depth) % _depth;
    return image(z).readPixel(Vector2(coords.x, coords.y));
}

TextureFilter Texture3::minFilter() const
{
    return _minFilter;
}

void Texture3::setMinFilter(TextureFilter filter)
{
    if (isUploaded())
    {
        renderer().destroyTexture(*this);
    }

    _minFilter = filter;
}

TextureFilter Texture3::magFilter() const
{
    return _magFilter;
}

void Texture3::setMagFilter(TextureFilter filter)
{
    if (isUploaded())
    {
        renderer().destroyTexture(*this);
    }

    _magFilter = filter;
}

bool Texture3::isMipmapped() const
{
    return _mipmapped;
}

void Texture3::setMipmapped(bool mipmapped)
{
    if (isUploaded())
    {
        renderer().destroyTexture(*this);
    }

    _mipmapped = mipmapped;
}

bool Texture3::isWrapped() const
{
    return _wrapped;
}

void Texture3::setWrapped(bool wrapped)
{
    if (isUploaded())
    {
        renderer().destroyTexture(*this);
    }

    _wrapped = wrapped;
}

unsigned Texture3::width() const
{
    return _width;
}

unsigned Texture3::height() const
{
    return _height;
}

unsigned Texture3::depth() const
{
    return _depth;
}

const PixelFormat& Texture3::pixelFormat() const
{
    return _pixelFormat;
}

bool Texture3::operator==(const Texture3& texture) const
{
    // Dimensions
    if (_width != texture._width && _height != texture._height && _depth != texture._depth)
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

bool Texture3::operator!=(const Texture3& texture) const
{
    return !(*this == texture);
}

void Texture3::encode(Encoder& encoder) const
{
    encoder << encodeVector("image", _images)
            << encodeEnum("minFilter", _minFilter)
            << encodeEnum("magFilter", _magFilter)
            << encodeValue("wrapped", _wrapped)
            << encodeValue("mipmapped", _mipmapped);
}

void Texture3::decode(Decoder& decoder)
{
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

        _depth = static_cast<unsigned>(images.size());

        unsigned depth = 0;
        for (Image::Handle& image : images)
        {
            // Remove the image from the asset cache because we don't want to
            // store uncompressed image data in main memory
            decoder.assetCache().remove(image.path());

            image->setColorSpace(colorSpace);
            setImage(depth, image);

            ++depth;
        }
    }

    decoder >> decodeEnum("minFilter", _minFilter)
            >> decodeEnum("magFilter", _magFilter)
            >> decodeValue("wrapped", _wrapped)
            >> decodeValue("mipmapped", _mipmapped);
}
