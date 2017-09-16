///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2016 Colin Hill
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
#include "RenderTarget.h"

#include "Hect/Core/Exception.h"

using namespace hect;

RenderTarget::RenderTarget()
{
}

RenderTarget::RenderTarget(unsigned width, unsigned height) :
    _width(width),
    _height(height)
{
}

void RenderTarget::swap_buffers()
{
    throw InvalidOperation("Render target does not have a back buffer");
}

unsigned RenderTarget::width() const
{
    return _width;
}

unsigned RenderTarget::height() const
{
    return _height;
}

Vector2 RenderTarget::dimensions() const
{
    return Vector2(_width, _height);
}

float RenderTarget::aspect_ratio() const
{
    return static_cast<float>(_width) / static_cast<float>(_height);
}

void RenderTarget::set_width(unsigned width)
{
    _width = width;
}

void RenderTarget::set_height(unsigned height)
{
    _height = height;
}
