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

using namespace hect;

Image::Image() :
    _width(0),
    _height(0),
    _pixelType(PixelType_Byte),
    _pixelFormat(PixelFormat_Rgba),
    _colorSpace(ColorSpace_Linear)
{
}

void Image::flipVertical()
{
    size_t bytesPerRow = bytesPerPixel() * _width;
    PixelData newPixelData(_pixelData.size(), 0);
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

Image::PixelData& Image::pixelData()
{
    return _pixelData;
}

const Image::PixelData& Image::pixelData() const
{
    return _pixelData;
}

void Image::setPixelData(PixelData&& pixelData)
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

int Image::bytesPerPixel() const
{
    int componentCount = 0;

    switch (_pixelFormat)
    {
    case PixelFormat_Rgb:
        componentCount = 3;
        break;
    case PixelFormat_Rgba:
        componentCount = 4;
        break;
    }

    switch (_pixelType)
    {
    case PixelType_Half:
        return componentCount * 2;
        break;
    case PixelType_Float:
        return componentCount * 4;
        break;
    case PixelType_Byte:
        return componentCount * 1;
        break;
    }

    return 0;
}