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
#include "Hect.h"

using namespace hect;

#include <lodepng.h>

void ImagePngFormat::load(Image& image, ReadStream& stream)
{
    image._pixelData.clear();

    // Load the PNG pixel data
    size_t length = stream.length();
    Image::RawPixelData encodedPixelData(length, 0);
    stream.readBytes(&encodedPixelData[0], length);

    // Decode the PNG pixel data
    unsigned width = 0;
    unsigned height = 0;
    unsigned error = lodepng::decode(image._pixelData, width, height, encodedPixelData);
    if (error)
    {
        throw Error(format("Failed to decode PNG data: %s", lodepng_error_text(error)));
    }

    image._width = width;
    image._height = height;
    image._pixelType = PixelType::Byte;
    image._pixelFormat = PixelFormat::Rgba;
    image._colorSpace = ColorSpace::NonLinear;

    // Flip the image to OpenGL ordering
    image.flipVertical();
}

void ImagePngFormat::save(const Image& image, WriteStream& stream)
{
    // Verify pixel format and type.
    if (image.pixelType() != PixelType::Byte || image.pixelFormat() != PixelFormat::Rgba)
    {
        throw Error("Attempt to save an image to PNG which does not conform to the 32-bit RGBA format");
    }

    // Flip the image from OpenGL ordering
    Image flippedImage = image;
    flippedImage.flipVertical();

    // Encode to PNG data
    Image::RawPixelData encodedPixelData;
    unsigned error = lodepng::encode(encodedPixelData, flippedImage.pixelData(), image.width(), image.height());
    if (error)
    {
        throw Error(format("Failed to encode PNG data: %s", lodepng_error_text(error)));
    }

    // Write the encoded data to disk
    stream.writeBytes(&encodedPixelData[0], encodedPixelData.size());
}