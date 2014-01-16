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

Image::Image(unsigned width, unsigned height, PixelType pixelType, PixelFormat pixelFormat, ColorSpace colorSpace) :
    _width(width),
    _height(height),
    _pixelType(pixelType),
    _pixelFormat(pixelFormat),
    _colorSpace(colorSpace)
{
    size_t totalSize = bytesPerPixel() * _width * _height;
    _pixelData = RawPixelData(totalSize, 0);
}

Image::Image(Image&& image) :
    _width(image._width),
    _height(image._height),
    _pixelType(image._pixelType),
    _pixelFormat(image._pixelFormat),
    _colorSpace(image._colorSpace),
    _pixelData(std::move(image._pixelData))
{
}

void Image::flipVertical()
{
    size_t bytesPerRow = bytesPerPixel() * _width;
    RawPixelData newPixelData(_pixelData.size(), 0);
    for (unsigned i = 0; i < _height; ++i)
    {
        size_t sourceRowOffset = bytesPerRow * i;
        size_t destRowOffset = bytesPerRow * (_height - i - 1);
        std::memcpy(&newPixelData[sourceRowOffset], &_pixelData[destRowOffset], bytesPerRow);
    }
    _pixelData = std::move(newPixelData);
}

Image::RawPixelData& Image::pixelData()
{
    return _pixelData;
}

const Image::RawPixelData& Image::pixelData() const
{
    return _pixelData;
}

unsigned Image::width() const
{
    return _width;
}

unsigned Image::height() const
{
    return _height;
}

PixelFormat Image::pixelFormat() const
{
    return _pixelFormat;
}

PixelType Image::pixelType() const
{
    return _pixelType;
}

ColorSpace Image::colorSpace() const
{
    return _colorSpace;
}

int Image::bytesPerPixel() const
{
    int componentCount = 0;

    switch (_pixelFormat)
    {
    case PixelFormat::Rgb:
        componentCount = 3;
        break;
    case PixelFormat::Rgba:
        componentCount = 4;
        break;
    }

    switch (_pixelType)
    {
    case PixelType::Half:
        return componentCount * 2;
        break;
    case PixelType::Float:
        return componentCount * 4;
        break;
    case PixelType::Byte:
        return componentCount * 1;
        break;
    }

    return 0;
}