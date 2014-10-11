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

#include <cstdint>
#include <vector>

#include "Hect/IO/Asset.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"

namespace hect
{

///
/// The color space a pixel is in.
enum ColorSpace
{
    ///
    /// Non-linear color space (sRGB).
    ///
    /// \note Only an image with a pixel type of PixelType_Byte can
    /// be non-linear.
    ColorSpace_NonLinear,

    ///
    /// Linear color space.
    ColorSpace_Linear
};

///
/// The type of each component in a pixel.
enum PixelType
{
    ///
    /// A byte.
    PixelType_Byte,

    ///
    /// 16-bit floating point.
    PixelType_Float16,

    ///
    /// 32-bit floating point.
    PixelType_Float32
};

///
/// The layout of the component(s) in a pixel.
enum PixelFormat
{
    ///
    /// Red, green, and blue channels.
    PixelFormat_Rgb,

    ///
    /// Red, green, blue, and alpha channels.
    PixelFormat_Rgba
};

///
/// A 2-dimensional image.
class Image :
    public Asset
{
public:

    ///
    /// Raw pixel data.
    typedef std::vector<uint8_t> PixelData;

    ///
    /// Flips the image vertically.
    void flipVertical();

    ///
    /// Returns whether the image has pixel data.
    bool hasPixelData() const;

    ///
    /// Returns the pixel data.
    PixelData& pixelData();

    ///
    /// Returns the pixel data.
    const PixelData& pixelData() const;

    ///
    /// Sets the pixel data for the image.
    ///
    /// \param pixelData The raw pixel data to move into the image's pixel
    /// data.
    void setPixelData(PixelData&& pixelData);

    ///
    /// Returns the width.
    unsigned width() const;

    ///
    /// Sets the width.
    ///
    /// \param width The new width.
    void setWidth(unsigned width);

    ///
    /// Returns the height.
    unsigned height() const;

    ///
    /// Sets the height.
    ///
    /// \param height The new height.
    void setHeight(unsigned height);

    ///
    /// Returns the pixel type.
    PixelType pixelType() const;

    ///
    /// Sets the pixel type.
    ///
    /// \param pixelType The new pixel type.
    void setPixelType(PixelType pixelType);

    ///
    /// Returns the pixel format.
    PixelFormat pixelFormat() const;

    ///
    /// Sets the pixel format.
    ///
    /// \param pixelFormat The new pixel format.
    void setPixelFormat(PixelFormat pixelFormat);

    ///
    /// Returns the color space.
    ColorSpace colorSpace() const;

    ///
    /// Sets the color space.
    ///
    /// \param colorSpace The new color space.
    void setColorSpace(ColorSpace colorSpace);

    ///
    /// Returns the number of bytes in a pixel of this image.
    unsigned bytesPerPixel() const;

    friend Encoder& operator<<(Encoder& encoder, const Image& image);
    friend Decoder& operator>>(Decoder& decoder, Image& image);

private:
    unsigned _width { 0 };
    unsigned _height { 0 };

    PixelType _pixelType { PixelType_Byte };
    PixelFormat _pixelFormat { PixelFormat_Rgba };

    ColorSpace _colorSpace { ColorSpace_Linear };

    PixelData _pixelData;
};

}