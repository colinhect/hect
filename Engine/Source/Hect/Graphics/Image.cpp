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
#include "Image.h"

#include <lodepng.h>

#include "Hect/Core/Exception.h"
#include "Hect/Core/Format.h"

using namespace hect;

Image::Image()
{
}

Image::Image(unsigned width, unsigned height, PixelType pixelType, PixelFormat pixelFormat) :
    _width(width),
    _height(height),
    _pixelType(pixelType),
    _pixelFormat(pixelFormat)
{
}

void Image::flipVertical()
{
    size_t bytesPerRow = bytesPerPixel() * _width;
    ByteVector newPixelData(_pixelData.size(), 0);
    for (unsigned i = 0; i < _height; ++i)
    {
        size_t sourceRowOffset = bytesPerRow * i;
        size_t destRowOffset = bytesPerRow * (_height - i - 1);
        std::memcpy(&newPixelData[sourceRowOffset], &_pixelData[destRowOffset], bytesPerRow);
    }
    _pixelData = std::move(newPixelData);
}

bool Image::hasPixelData() const
{
    return !_pixelData.empty();
}

ByteVector& Image::pixelData()
{
    return _pixelData;
}

const ByteVector& Image::pixelData() const
{
    return _pixelData;
}

void Image::setPixelData(ByteVector&& pixelData)
{
    _pixelData = pixelData;
}

void Image::setPixel(unsigned x, unsigned y, const Color& color)
{
    unsigned pixelSize = bytesPerPixel();

    if (_pixelData.empty())
    {
        _pixelData = ByteVector(_width * _height * pixelSize);
    }

    unsigned index = _width * y * pixelSize + x * pixelSize;
    for (unsigned pixelOffset = 0; pixelOffset < pixelSize; ++pixelOffset)
    {
        _pixelData[index + pixelOffset] = static_cast<uint8_t>(color[pixelOffset] * 255);
    }
}

unsigned Image::width() const
{
    return _width;
}

void Image::setWidth(unsigned width)
{
    _width = width;
}

unsigned Image::height() const
{
    return _height;
}

void Image::setHeight(unsigned height)
{
    _height = height;
}

PixelType Image::pixelType() const
{
    return _pixelType;
}

void Image::setPixelType(PixelType pixelType)
{
    _pixelType = pixelType;
}

PixelFormat Image::pixelFormat() const
{
    return _pixelFormat;
}

void Image::setPixelFormat(PixelFormat pixelFormat)
{
    _pixelFormat = pixelFormat;
}

ColorSpace Image::colorSpace() const
{
    return _colorSpace;
}

void Image::setColorSpace(ColorSpace colorSpace)
{
    _colorSpace = colorSpace;
}

unsigned Image::bytesPerPixel() const
{
    static const unsigned _bytesPerPixelLookUp[2][3] =
    {
        { 3, 6, 12 },
        { 4, 8, 16 }
    };

    return _bytesPerPixelLookUp[_pixelFormat][_pixelType];
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const Image& image)
{
    WriteStream& stream = encoder.binaryStream();

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

    return encoder;
}

Decoder& operator>>(Decoder& decoder, Image& image)
{
    ReadStream& stream = decoder.binaryStream();

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
    image.setPixelData(std::move(decodedPixelData));

    // Flip the image to OpenGL ordering
    image.flipVertical();

    return decoder;
}

}
