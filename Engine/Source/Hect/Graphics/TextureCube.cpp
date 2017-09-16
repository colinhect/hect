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
#include "TextureCube.h"

#include "Hect/Core/Format.h"
#include "Hect/Core/Exception.h"

using namespace hect;

TextureCube::TextureCube()
{
}

TextureCube::TextureCube(Name name, unsigned width, unsigned height) :
    Asset(name),
    _width(width),
    _height(height)
{
}

Image& TextureCube::image(CubeSide side)
{
    Image::Handle& image = _images[static_cast<int>(side)];
    if (!image)
    {
        image = Image::Handle(new Image(_width, _height, _pixel_format));
    }
    return *image;
}

void TextureCube::set_image(CubeSide side, const Image::Handle& image)
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

    _images[static_cast<int>(side)] = image;
}

void TextureCube::invalidate_local_images()
{
    if (!is_uploaded())
    {
        throw InvalidOperation("Texture is not uploaded");
    }

    for (Image::Handle& image : _images)
    {
        image = Image::Handle();
    }
}

TextureFilter TextureCube::min_filter() const
{
    return _min_filter;
}

void TextureCube::set_min_filter(TextureFilter filter)
{
    destroy_if_uploaded();
    _min_filter = filter;
}

TextureFilter TextureCube::mag_filter() const
{
    return _mag_filter;
}

void TextureCube::set_mag_filter(TextureFilter filter)
{
    destroy_if_uploaded();
    _mag_filter = filter;
}

bool TextureCube::is_mipmapped() const
{
    return _mipmapped;
}

void TextureCube::set_mipmapped(bool mipmapped)
{
    destroy_if_uploaded();
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

PixelFormat TextureCube::pixel_format() const
{
    return _pixel_format;
}

void TextureCube::set_pixel_format(PixelFormat pixel_format)
{
    destroy_if_uploaded();
    _pixel_format = pixel_format;
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
    if (_pixel_format != texture._pixel_format)
    {
        return false;
    }

    // Min/mag filters
    if (_min_filter != texture._min_filter && _mag_filter != texture._mag_filter)
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
    encoder << begin_object("images")
            << encode_value("positive_x", _images[0])
            << encode_value("negative_x", _images[1])
            << encode_value("positive_y", _images[2])
            << encode_value("negative_y", _images[3])
            << encode_value("positive_z", _images[4])
            << encode_value("negative_z", _images[5])
            << end_object()
            << encode_enum("min_filter", _min_filter)
            << encode_enum("mag_filter", _mag_filter)
            << encode_value("mipmapped", _mipmapped);
}

void TextureCube::decode(Decoder& decoder)
{
    // Color space
    ColorSpace color_space = ColorSpace::NonLinear;
    if (decoder.select_member("color_space"))
    {
        decoder >> decode_enum(color_space);
    }

    // Images
    decoder >> begin_object("images")
            >> decode_value("positive_x", _images[0], true)
            >> decode_value("negative_x", _images[1])
            >> decode_value("positive_y", _images[2])
            >> decode_value("negative_y", _images[3])
            >> decode_value("positive_z", _images[4])
            >> decode_value("negative_z", _images[5])
            >> end_object();

    for (Image::Handle& image : _images)
    {
        // Remove the image from the asset cache because we don't want to
        // store uncompressed image data in main memory
        decoder.asset_cache().remove(image.path());

        image->set_color_space(color_space);
    }

    decoder >> decode_enum("min_filter", _min_filter)
            >> decode_enum("mag_filter", _mag_filter)
            >> decode_value("mipmapped", _mipmapped);
}

void TextureCube::destroy_if_uploaded()
{
    if (is_uploaded())
    {
        renderer().destroy_texture(*this);
    }
}
