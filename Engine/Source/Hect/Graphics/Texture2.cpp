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
    set_image(image);
}

Image& Texture2::image()
{
    if (!_image)
    {
        _image = Image::Handle(new Image(_width, _height, _pixel_format));

        if (is_uploaded())
        {
            renderer().download_texture_image(*this);
        }
    }

    return *_image;
}

void Texture2::set_image(const Image::Handle& image)
{
    destroy_if_uploaded();

    // If the texture is empty
    if (_width == 0 && _height == 0)
    {
        // Use the width/height/pixel format of the image
        _width = image->width();
        _height = image->height();
        _pixel_format = image->pixel_format();
    }
    else if (_width != image->width() || _height != image->height() || _pixel_format != image->pixel_format())
    {
        throw InvalidOperation("Image is incompatible with the texture");
    }

    _image = image;
}

void Texture2::invalidate_local_image()
{
    if (!is_uploaded())
    {
        throw InvalidOperation("Texture is not uploaded");
    }

    _image = Image::Handle();
}

Color Texture2::read_pixel(unsigned x, unsigned y)
{
    return image().read_pixel(x, y);
}

Color Texture2::read_pixel(Vector2 coords)
{
    return image().read_pixel(coords);
}

TextureFilter Texture2::min_filter() const
{
    return _min_filter;
}

void Texture2::set_min_filter(TextureFilter filter)
{
    destroy_if_uploaded();
    _min_filter = filter;
}

TextureFilter Texture2::mag_filter() const
{
    return _mag_filter;
}

void Texture2::set_mag_filter(TextureFilter filter)
{
    destroy_if_uploaded();
    _mag_filter = filter;
}

bool Texture2::is_mipmapped() const
{
    return _mipmapped;
}

void Texture2::set_mipmapped(bool mipmapped)
{
    destroy_if_uploaded();
    _mipmapped = mipmapped;
}

bool Texture2::is_wrapped() const
{
    return _wrapped;
}

void Texture2::set_wrapped(bool wrapped)
{
    destroy_if_uploaded();
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

PixelFormat Texture2::pixel_format() const
{
    return _pixel_format;
}

void Texture2::set_pixel_format(PixelFormat pixel_format)
{
    destroy_if_uploaded();
    _pixel_format = pixel_format;
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
    if (_pixel_format != texture._pixel_format)
    {
        return false;
    }

    // Min/mag filters
    if (_min_filter != texture._min_filter && _mag_filter != texture._mag_filter)
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
    encoder << encode_value("image", _image)
            << encode_enum("min_filter", _min_filter)
            << encode_enum("mag_filter", _mag_filter)
            << encode_value("wrapped", _wrapped)
            << encode_value("mipmapped", _mipmapped);
}

void Texture2::decode(Decoder& decoder)
{
    // Color space
    ColorSpace color_space = ColorSpace::NonLinear;
    if (decoder.select_member("color_space"))
    {
        decoder >> decode_enum(color_space);
    }

    // Images
    if (decoder.select_member("image"))
    {
        Image::Handle image;
        decoder >> decode_value(image);

        // Remove the image from the asset cache because we don't want to
        // store uncompressed image data in main memory
        decoder.asset_cache().remove(image.path());

        image->set_color_space(color_space);
        set_image(image);
    }

    decoder >> decode_enum("min_filter", _min_filter)
            >> decode_enum("mag_filter", _mag_filter)
            >> decode_value("wrapped", _wrapped)
            >> decode_value("mipmapped", _mipmapped);
}

void Texture2::destroy_if_uploaded()
{
    if (is_uploaded())
    {
        renderer().destroy_texture(*this);
    }
}
