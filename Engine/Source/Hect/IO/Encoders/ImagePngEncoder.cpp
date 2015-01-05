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
#include "ImagePngEncoder.h"

#include <lodepng.h>

#include "Hect/Core/Exception.h"
#include "Hect/Core/Format.h"

using namespace hect;

void ImagePngEncoder::encode(const Image& image, WriteStream& stream)
{
    // Verify pixel format and type.
    if (image.pixelType() != PixelType_Byte || image.pixelFormat() != PixelFormat_Rgba)
    {
        throw InvalidOperation("Cannot encode an image to PNG which does not conform to the 32-bit RGBA format");
    }

    // Flip the image from OpenGL ordering
    Image flippedImage = image;
    flippedImage.flipVertical();

    // Encode to PNG data
    ByteVector encodedPixelData;
    unsigned error = lodepng::encode(encodedPixelData, flippedImage.pixelData(), image.width(), image.height());
    if (error)
    {
        throw EncodeError(format("Failed to encode PNG data: %s", lodepng_error_text(error)));
    }

    // Write the encoded data to the stream
    stream.write(&encodedPixelData[0], encodedPixelData.size());
}

void ImagePngEncoder::decode(Image& image, ReadStream& stream)
{
    // Read all of the encoded data from the stream
    size_t length = stream.length();
    ByteVector encodedPixelData(length, 0);
    stream.read(&encodedPixelData[0], length);

    ByteVector decodedPixelData;

    // Decode the PNG pixel data
    unsigned width = 0;
    unsigned height = 0;
    unsigned error = lodepng::decode(decodedPixelData, width, height, encodedPixelData);
    if (error)
    {
        throw DecodeError(format("Failed to decode PNG data: %s", lodepng_error_text(error)));
    }

    // Set various properties for the image
    image.setWidth(width);
    image.setHeight(height);
    image.setPixelType(PixelType_Byte);
    image.setPixelFormat(PixelFormat_Rgba);
    image.setColorSpace(ColorSpace_NonLinear); // Assume the image is sRGB
    image.setPixelData(std::move(decodedPixelData));

    // Flip the image to OpenGL ordering
    image.flipVertical();
}