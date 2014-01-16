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
#pragma once

namespace hect
{

///
/// The type of each component in a pixel.
enum class PixelType
{
    ///
    /// 16-bit floating point.
    Half,

    ///
    /// 32-bit floating point.
    Float,

    ///
    /// A byte.
    Byte
};

///
/// The layout of the component(s) in a pixel.
enum class PixelFormat
{
    ///
    /// Red, green, and blue channels.
    Rgb,

    ///
    /// Red, green, blue, and alpha channels.
    Rgba
};

///
/// The color space a pixel is in.
enum class ColorSpace
{
    ///
    /// Non-linear color space (sRGB).
    ///
    /// \remarks Only an image with a pixel type of PixelType::Byte can
    /// be non-linear.
    NonLinear,

    ///
    /// Linear color space.
    Linear
};

///
/// A 2-dimensional image.
class Image
{
    friend class ImagePngFormat;
public:

    ///
    /// A shared reference to an image.
    typedef std::shared_ptr<Image> Ref;

    ///
    /// Raw pixel data.
    typedef std::vector<uint8_t> RawPixelData;

    ///
    /// Constructs a solid black image.
    ///
    /// \param width The width.
    /// \param height The height.
    /// \param pixelType The pixel type.
    /// \param pixelFormat The pixel format.
    /// \param colorSpace The color space.
    Image(unsigned width = 1, unsigned height = 1, PixelType pixelType = PixelType::Byte, PixelFormat pixelFormat = PixelFormat::Rgba, ColorSpace colorSpace = ColorSpace::Linear);

    ///
    /// Constructs an image moved from another.
    ///
    /// \param image The image to move.
    Image(Image&& image);

    ///
    /// Flips the image vertically.
    void flipVertical();

    ///
    /// Returns the raw pixel data.
    RawPixelData& pixelData();

    ///
    /// Returns the raw pixel data.
    const RawPixelData& pixelData() const;

    ///
    /// Returns the width.
    unsigned width() const;

    ///
    /// Returns the height.
    unsigned height() const;

    ///
    /// Returns the pixel type.
    PixelType pixelType() const;

    ///
    /// Returns the pixel format.
    PixelFormat pixelFormat() const;

    ///
    /// Returns the color space.
    ColorSpace colorSpace() const;

    ///
    /// Returns the number of bytes in a pixel of this image.
    int bytesPerPixel() const;

private:
    unsigned _width;
    unsigned _height;

    PixelType _pixelType;
    PixelFormat _pixelFormat;

    ColorSpace _colorSpace;

    RawPixelData _pixelData;
};

}