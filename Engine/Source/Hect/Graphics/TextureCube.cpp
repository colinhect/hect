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
#include "TextureCube.h"

#include "Hect/Core/Format.h"
#include "Hect/Core/Exception.h"

using namespace hect;

TextureCube::TextureCube()
{
}

TextureCube::TextureCube(const std::string& name, unsigned width, unsigned height, const PixelFormat& pixelFormat, TextureFilter minFilter, TextureFilter magFilter, bool mipmapped) :
    Asset(name),
    _width(width),
    _height(height),
    _pixelFormat(pixelFormat),
    _minFilter(minFilter),
    _magFilter(magFilter),
    _mipmapped(mipmapped)
{
}

Image& TextureCube::image(CubeSide side)
{
    Image::Handle& image = _images[static_cast<int>(side)];
    if (!image)
    {
        image = Image::Handle(new Image(_width, _height, _pixelFormat));
    }
    return *image;
}

void TextureCube::setImage(CubeSide side, const Image::Handle& image)
{
    if (isUploaded())
    {
        renderer().destroyTexture(*this);
    }

    if (_width != image->width() || _height != image->height())
    {
        throw InvalidOperation("The source image does not match the dimensions of the texture");
    }
    else if (_pixelFormat != image->pixelFormat())
    {
        throw InvalidOperation("The source image pixel format does not match the pixel format of the texture");
    }

    _images[static_cast<int>(side)] = image;
}

void TextureCube::markAsDirty()
{
    for (Image::Handle& image : _images)
    {
        image = Image::Handle();
    }
}

TextureFilter TextureCube::minFilter() const
{
    return _minFilter;
}

void TextureCube::setMinFilter(TextureFilter filter)
{
    if (isUploaded())
    {
        renderer().destroyTexture(*this);
    }

    _minFilter = filter;
}

TextureFilter TextureCube::magFilter() const
{
    return _magFilter;
}

void TextureCube::setMagFilter(TextureFilter filter)
{
    if (isUploaded())
    {
        renderer().destroyTexture(*this);
    }

    _magFilter = filter;
}

bool TextureCube::isMipmapped() const
{
    return _mipmapped;
}

void TextureCube::setMipmapped(bool mipmapped)
{
    if (isUploaded())
    {
        renderer().destroyTexture(*this);
    }

    _mipmapped = mipmapped;
}

unsigned TextureCube::width() const
{
    return _width;
}

unsigned TextureCube::height() const
{
    return _height;
}

const PixelFormat& TextureCube::pixelFormat() const
{
    return _pixelFormat;
}

bool TextureCube::operator==(const TextureCube& texture) const
{
    // Source images
    for (size_t i = 0; i < 6; ++i)
    {
        if (_images[i] != texture._images[i])
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

    return true;
}

bool TextureCube::operator!=(const TextureCube& texture) const
{
    return !(*this == texture);
}

void TextureCube::encode(Encoder& encoder) const
{
    encoder << beginObject("images")
            << encodeValue("positiveX", _images[0])
            << encodeValue("negativeX", _images[1])
            << encodeValue("positiveY", _images[2])
            << encodeValue("negativeY", _images[3])
            << encodeValue("positiveZ", _images[4])
            << encodeValue("negativeZ", _images[5])
            << endObject()
            << encodeEnum("minFilter", _minFilter)
            << encodeEnum("magFilter", _magFilter)
            << encodeValue("mipmapped", _mipmapped);
}

void TextureCube::decode(Decoder& decoder)
{
    // Color space
    ColorSpace colorSpace = ColorSpace::NonLinear;
    if (decoder.selectMember("colorSpace"))
    {
        decoder >> decodeEnum(colorSpace);
    }

    // Images
    decoder >> beginObject("images")
            >> decodeValue("positiveX", _images[0], true)
            >> decodeValue("negativeX", _images[1])
            >> decodeValue("positiveY", _images[2])
            >> decodeValue("negativeY", _images[3])
            >> decodeValue("positiveZ", _images[4])
            >> decodeValue("negativeZ", _images[5])
            >> endObject();

    for (Image::Handle& image : _images)
    {
        // Remove the image from the asset cache because we don't want to
        // store uncompressed image data in main memory
        decoder.assetCache().remove(image.path());

        image->setColorSpace(colorSpace);
    }

    decoder >> decodeEnum("minFilter", _minFilter)
            >> decodeEnum("magFilter", _magFilter)
            >> decodeValue("mipmapped", _mipmapped);
}
