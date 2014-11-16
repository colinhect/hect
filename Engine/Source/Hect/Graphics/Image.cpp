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
#include "Image.h"

#include "Hect/IO/Encoders/ImagePngEncoder.h"

using namespace hect;

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
    ImagePngEncoder::encode(image, stream);
    return encoder;
}

Decoder& operator>>(Decoder& decoder, Image& image)
{
    ReadStream& stream = decoder.binaryStream();
    ImagePngEncoder::decode(image, stream);
    return decoder;
}

}