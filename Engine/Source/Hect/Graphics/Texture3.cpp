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

Texture3::Texture3(Name name, unsigned width, unsigned height, unsigned depth) :
    Asset(name),
    _width(width),
    _height(height),
    _depth(depth)
{
}

Image& Texture3::image(unsigned depth)
{
    bool download_images = false;
    if (_images.empty())
    {
        _images = std::vector<Image::Handle>(_depth);
        download_images = is_uploaded();
    }

    if (depth >= _depth)
    {
        throw InvalidOperation("Depth is greater than the depth of the texture");
    }

    Image::Handle& image = _images[depth];
    if (!image)
    {
        image = Image::Handle(new Image(_width, _height, _pixel_format));
    }

    if (download_images)
    {
        renderer().download_texture_images(*this);
    }

    return *image;
}

void Texture3::set_image(unsigned depth, const Image::Handle& image)
{
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

    // Ensure that the image vector is intialized and the depth is within
    // the bounds of the texture
    this->image(depth);

    _images[depth] = image;
}

void Texture3::invalidate_local_images()
{
    if (!is_uploaded())
    {
        throw InvalidOperation("Texture is not uploaded");
    }

    _images.clear();
}

Color Texture3::read_pixel(unsigned x, unsigned y, unsigned z)
{
    return image(z).read_pixel(x, y);
}

Color Texture3::read_pixel(Vector3 coords)
{
    unsigned z = static_cast<unsigned>(coords.z * _depth) % _depth;
    return image(z).read_pixel(Vector2(coords.x, coords.y));
}

TextureFilter Texture3::min_filter() const
{
    return _min_filter;
}

void Texture3::set_min_filter(TextureFilter filter)
{
    destroy_if_uploaded();
    _min_filter = filter;
}

TextureFilter Texture3::mag_filter() const
{
    return _mag_filter;
}

void Texture3::set_mag_filter(TextureFilter filter)
{
    destroy_if_uploaded();
    _mag_filter = filter;
}

bool Texture3::is_mipmapped() const
{
    return _mipmapped;
}

void Texture3::set_mipmapped(bool mipmapped)
{
    destroy_if_uploaded();
    _mipmapped = mipmapped;
}

bool Texture3::is_wrapped() const
{
    return _wrapped;
}

void Texture3::set_wrapped(bool wrapped)
{
    destroy_if_uploaded();
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

PixelFormat Texture3::pixel_format() const
{
    return _pixel_format;
}

void Texture3::set_pixel_format(PixelFormat pixel_format)
{
    destroy_if_uploaded();
    _pixel_format = pixel_format;
}

bool Texture3::operator==(const Texture3& texture) const
{
    // Dimensions
    if (_width != texture._width && _height != texture._height && _depth != texture._depth)
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

bool Texture3::operator!=(const Texture3& texture) const
{
    return !(*this == texture);
}

void Texture3::encode(Encoder& encoder) const
{
    encoder << encode_vector("image", _images)
            << encode_enum("min_filter", _min_filter)
            << encode_enum("mag_filter", _mag_filter)
            << encode_value("wrapped", _wrapped)
            << encode_value("mipmapped", _mipmapped);
}

void Texture3::decode(Decoder& decoder)
{
    // Color space
    ColorSpace color_space = ColorSpace::NonLinear;
    if (decoder.select_member("color_space"))
    {
        decoder >> decode_enum(color_space);
    }

    // Images
    if (decoder.select_member("images"))
    {
        std::vector<Image::Handle> images;
        decoder >> decode_vector(images);

        _depth = static_cast<unsigned>(images.size());

        unsigned depth = 0;
        for (Image::Handle& image : images)
        {
            // Remove the image from the asset cache because we don't want to
            // store uncompressed image data in main memory
            decoder.asset_cache().remove(image.path());

            image->set_color_space(color_space);
            set_image(depth, image);

            ++depth;
        }
    }

    decoder >> decode_enum("min_filter", _min_filter)
            >> decode_enum("mag_filter", _mag_filter)
            >> decode_value("wrapped", _wrapped)
            >> decode_value("mipmapped", _mipmapped);
}

void Texture3::destroy_if_uploaded()
{
    if (is_uploaded())
    {
        renderer().destroy_texture(*this);
    }
}
