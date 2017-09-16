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
#include "Image.h"

#include <lodepng.h>

#include "Hect/Core/Exception.h"
#include "Hect/Core/Format.h"

using namespace hect;

Image::Image()
{
}

Image::Image(unsigned width, unsigned height, const PixelFormat& pixel_format) :
    _width(width),
    _height(height),
    _pixel_format(pixel_format)
{
}

void Image::flip_vertical()
{
    size_t bytes_per_row = _pixel_format.size() * _width;
    ByteVector new_pixel_data(_pixel_data.size(), 0);
    for (unsigned i = 0; i < _height; ++i)
    {
        size_t source_row_offset = bytes_per_row * i;
        size_t dest_row_offset = bytes_per_row * (_height - i - 1);
        std::memcpy(&new_pixel_data[source_row_offset], &_pixel_data[dest_row_offset], bytes_per_row);
    }
    _pixel_data = std::move(new_pixel_data);
}

bool Image::has_pixel_data() const
{
    return !_pixel_data.empty();
}

ByteVector& Image::pixel_data()
{
    return _pixel_data;
}

const ByteVector& Image::pixel_data() const
{
    return _pixel_data;
}

void Image::set_pixel_data(ByteVector&& pixel_data)
{
    _pixel_data = pixel_data;
}

void Image::write_pixel(unsigned x, unsigned y, Color color)
{
    ensure_pixel_data();

    size_t offset = compute_pixel_offset(x, y);

    for (unsigned component_index = 0; component_index < _pixel_format.cardinality(); ++component_index)
    {
        double value = color[component_index];

        switch (_pixel_format.type())
        {
        case PixelType::Byte:
            _pixel_data[offset + component_index] = static_cast<uint8_t>(color[component_index] * 255);
            break;
        case PixelType::Float16:
            throw InvalidOperation("16-bit floats are not implemented");
        case PixelType::Float32:
            *reinterpret_cast<float*>(&_pixel_data[offset + (component_index * 4)]) = static_cast<float>(value);
            break;
        }
    }
}

void Image::write_pixel(Vector2 coords, Color color)
{
    unsigned x = static_cast<unsigned>(coords.x * _width) % _width;
    unsigned y = static_cast<unsigned>(coords.y * _height) % _height;
    write_pixel(x, y, color);
}

Color Image::read_pixel(unsigned x, unsigned y) const
{
    Color color;

    if (has_pixel_data())
    {
        size_t offset = compute_pixel_offset(x, y);

        for (unsigned component_index = 0; component_index < _pixel_format.cardinality(); ++component_index)
        {
            switch (_pixel_format.type())
            {
            case PixelType::Byte:
                color[component_index] = static_cast<double>(_pixel_data[offset + component_index]) / 255.0;
                break;
            case PixelType::Float16:
                throw InvalidOperation("16-bit floats are not implemented");
            case PixelType::Float32:
                color[component_index] = *reinterpret_cast<const float*>(&_pixel_data[offset + (component_index * 4)]);
                break;
            }
        }
    }

    return color;
}

Color Image::read_pixel(Vector2 coords) const
{
    unsigned x = static_cast<unsigned>(coords.x * _width) % _width;
    unsigned y = static_cast<unsigned>(coords.y * _height) % _height;
    return read_pixel(x, y);
}

unsigned Image::width() const
{
    return _width;
}

void Image::set_width(unsigned width)
{
    _width = width;
}

unsigned Image::height() const
{
    return _height;
}

void Image::set_height(unsigned height)
{
    _height = height;
}

const PixelFormat& Image::pixel_format() const
{
    return _pixel_format;
}

void Image::set_pixel_format(PixelFormat pixel_format)
{
    ensure_compatible(pixel_format, _color_space);
    _pixel_format = pixel_format;
}

ColorSpace Image::color_space() const
{
    return _color_space;
}

void Image::set_color_space(ColorSpace color_space)
{
    ensure_compatible(_pixel_format, color_space);
    _color_space = color_space;
}

void Image::ensure_pixel_data()
{
    if (_pixel_data.empty())
    {
        _pixel_data = ByteVector(_width * _height * _pixel_format.size());
    }
}

size_t Image::compute_pixel_offset(unsigned x, unsigned y) const
{
    unsigned pixel_size = _pixel_format.size();
    size_t offset = _width * y * pixel_size + x * pixel_size;
    return offset;
}

void Image::ensure_compatible(const PixelFormat& pixel_format, ColorSpace color_space)
{
    if (color_space == ColorSpace::NonLinear)
    {
        if (pixel_format.type() != PixelType::Byte ||
                (pixel_format.cardinality() != 3 &&
                 pixel_format.cardinality() != 4))
        {
            throw InvalidOperation("Color space is incompatible with pixel format");
        }
    }
}

void Image::encode(Encoder& encoder) const
{
    WriteStream& stream = encoder.binary_stream();

    // Verify pixel format and type.
    if (_pixel_format.type() != PixelType::Byte || _pixel_format.cardinality() != 4)
    {
        throw InvalidOperation("Cannot encode an image to PNG which does not conform to the 32-bit RGBA format");
    }

    // Flip the image from OpenGL ordering
    Image flipped_image = *this;
    flipped_image.flip_vertical();

    // Encode to PNG data
    ByteVector encoded_pixel_data;
    unsigned error = lodepng::encode(encoded_pixel_data, flipped_image.pixel_data(), _width, _height);
    if (error)
    {
        throw EncodeError(format("Failed to encode PNG data: %s", lodepng_error_text(error)));
    }

    // Write the encoded data to the stream
    stream.write(&encoded_pixel_data[0], encoded_pixel_data.size());
}

void Image::decode(Decoder& decoder)
{
    ReadStream& stream = decoder.binary_stream();

    // Read all of the encoded data from the stream
    size_t length = stream.length();
    ByteVector encoded_pixel_data(length, 0);
    stream.read(&encoded_pixel_data[0], length);

    ByteVector decoded_pixel_data;

    // Decode the PNG pixel data
    unsigned width = 0;
    unsigned height = 0;
    unsigned error = lodepng::decode(decoded_pixel_data, width, height, encoded_pixel_data);
    if (error)
    {
        throw DecodeError(format("Failed to decode PNG data: %s", lodepng_error_text(error)));
    }

    // Set various properties for the image
    set_width(width);
    set_height(height);
    set_pixel_format(PixelFormat::Rgba8);
    set_pixel_data(std::move(decoded_pixel_data));

    // Flip the image to OpenGL ordering
    flip_vertical();
}
