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
#include "ImagePngEncoder.h"

#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"

using namespace hect;

#include <lodepng.h>

void ImagePngEncoder::encode(const Image& image, WriteStream& stream)
{
    // Verify pixel format and type.
    if (image.pixelType() != PixelType_UInt8 || image.pixelFormat() != PixelFormat_Rgba)
    {
        throw Error("Cannot encode an image to PNG which does not conform to the 32-bit RGBA format");
    }

    // Flip the image from OpenGL ordering
    Image flippedImage = image;
    flippedImage.flipVertical();

    // Encode to PNG data
    Image::PixelData encodedPixelData;
    unsigned error = lodepng::encode(encodedPixelData, flippedImage.pixelData(), image.width(), image.height());
    if (error)
    {
        throw Error(format("Failed to encode PNG data: %s", lodepng_error_text(error)));
    }

    // Write the encoded data
    stream.write(&encodedPixelData[0], encodedPixelData.size());
}

void ImagePngEncoder::decode(Image& image, ReadStream& stream)
{
    // Read the encoded data
    size_t length = stream.length();
    Image::PixelData encodedPixelData(length, 0);
    stream.read(&encodedPixelData[0], length);

    Image::PixelData decodedPixelData;

    // Decode the PNG pixel data
    unsigned width = 0;
    unsigned height = 0;
    unsigned error = lodepng::decode(decodedPixelData, width, height, encodedPixelData);
    if (error)
    {
        throw Error(format("Failed to decode PNG data: %s", lodepng_error_text(error)));
    }

    image.setWidth(width);
    image.setHeight(height);
    image.setPixelType(PixelType_UInt8);
    image.setPixelFormat(PixelFormat_Rgba);
    image.setColorSpace(ColorSpace_NonLinear);
    image.setPixelData(std::move(decodedPixelData));

    // Flip the image to OpenGL ordering
    image.flipVertical();
}