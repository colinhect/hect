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
#pragma once

#include "Hect/Core/Export.h"
#include "Hect/Graphics/Color.h"
#include "Hect/Graphics/ColorSpace.h"
#include "Hect/Graphics/PixelFormat.h"
#include "Hect/IO/Asset.h"
#include "Hect/IO/ByteVector.h"

namespace hect
{

///
/// A 2-dimensional image.
class HECT_EXPORT Image :
    public Asset<Image>
{
public:

    ///
    /// Constructs an empty image.
    Image();

    ///
    /// Constructs a blank image.
    ///
    /// \param width The width.
    /// \param height The height.
    /// \param pixel_format The pixel format.
    Image(unsigned width, unsigned height, const PixelFormat& pixel_format);

    ///
    /// Flips the image vertically.
    void flip_vertical();

    ///
    /// Returns whether the image has pixel data.
    bool has_pixel_data() const;

    ///
    /// Returns the raw pixel data.
    ByteVector& pixel_data();

    ///
    /// Returns the raw pixel data.
    const ByteVector& pixel_data() const;

    ///
    /// Sets the pixel data for the image.
    ///
    /// \param pixel_data The raw pixel data to move into the image's pixel
    /// data.
    void set_pixel_data(ByteVector&& pixel_data);

    ///
    /// Writes a color value to a pixel.
    ///
    /// \param x The x coordinate.
    /// \param y The y coordinate.
    /// \param color The color to write to the pixel.
    void write_pixel(unsigned x, unsigned y, Color color);

    ///
    /// Writes a color value to a pixel.
    ///
    /// \param coords The UV coordinates.
    /// \param color The color to write to the pixel.
    void write_pixel(Vector2 coords, Color color);

    ///
    /// Reads a color value of a pixel.
    ///
    /// \param x The x coordinate.
    /// \param y The y coordinate.
    Color read_pixel(unsigned x, unsigned y) const;

    ///
    /// Reads a color value of a pixel.
    ///
    /// \param coords The UV coordinates.
    Color read_pixel(Vector2 coords) const;

    ///
    /// Returns the width.
    unsigned width() const;

    ///
    /// Sets the width.
    ///
    /// \param width The new width.
    void set_width(unsigned width);

    ///
    /// Returns the height.
    unsigned height() const;

    ///
    /// Sets the height.
    ///
    /// \param height The new height.
    void set_height(unsigned height);

    ///
    /// Returns the pixel format.
    const PixelFormat& pixel_format() const;

    ///
    /// Sets the pixel format.
    ///
    /// \note Does not affect the raw pixel data.
    ///
    /// \param pixel_format The new pixel format.
    ///
    /// \throws InvalidOperation If the pixel format is incompatible with the
    /// color space.
    void set_pixel_format(PixelFormat pixel_format);

    ///
    /// Returns the color space that the image's pixel data is in.
    ColorSpace color_space() const;

    ///
    /// Set the color space that the image's pixel data is in.
    ///
    /// \note Does not affect the raw pixel data.
    ///
    /// \param color_space The new color space.
    ///
    /// \throws InvalidOperation If the color space is incompatible with the
    /// pixel format.
    void set_color_space(ColorSpace color_space);

    void encode(Encoder& encoder) const override;
    void decode(Decoder& decoder) override;

private:
    void ensure_pixel_data();
    void ensure_compatible(const PixelFormat& pixel_format, ColorSpace color_space);
    size_t compute_pixel_offset(unsigned x, unsigned y) const;

    unsigned _width { 0 };
    unsigned _height { 0 };

    PixelFormat _pixel_format;

    ColorSpace _color_space { ColorSpace::Linear };

    ByteVector _pixel_data;
};

}