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

VideoMode::VideoMode() :
    _width(800),
    _height(600),
    _bitsPerPixel(32),
    _fullscreen(false)
{
}

VideoMode::VideoMode(unsigned width, unsigned height, unsigned bitsPerPixel, bool fullscreen) :
    _width(width),
    _height(height),
    _bitsPerPixel(bitsPerPixel),
    _fullscreen(fullscreen)
{
}

unsigned VideoMode::width() const
{
    return _width;
}

unsigned VideoMode::height() const
{
    return _height;
}

unsigned VideoMode::bitsPerPixel() const
{
    return _bitsPerPixel;
}

bool VideoMode::isFullscreen() const
{
    return _fullscreen;
}