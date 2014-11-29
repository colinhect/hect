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
#include "RenderBuffer.h"

using namespace hect;

RenderBuffer::RenderBuffer()
{
}

RenderBuffer::RenderBuffer(RenderBufferFormat format, unsigned width, unsigned height) :
    _format(format),
    _width(width),
    _height(height)
{
}

RenderBufferFormat RenderBuffer::format() const
{
    return _format;
}

unsigned RenderBuffer::width() const
{
    return _width;
}

unsigned RenderBuffer::height() const
{
    return _height;
}

unsigned RenderBuffer::bytesPerPixel() const
{
    unsigned byteCount = 0;
    switch (_format)
    {
    case RenderBufferFormat_DepthComponent:
        byteCount = 4;
        break;
    }
    return byteCount;
}

size_t RenderBuffer::memoryUsage() const
{
    return _width * _height * bytesPerPixel();
}