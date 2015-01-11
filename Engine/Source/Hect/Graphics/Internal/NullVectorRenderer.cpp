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
#include "NullVectorRenderer.h"

using namespace hect;

void NullVectorRenderer::beginFrame(RenderTarget& target)
{
    (void)target;
}

void NullVectorRenderer::endFrame()
{
}

void NullVectorRenderer::pushState()
{
}

void NullVectorRenderer::popState()
{
}

void NullVectorRenderer::beginPath()
{
}

void NullVectorRenderer::setFillColor(const Color& color)
{
    (void)color;
}

void NullVectorRenderer::fill()
{
}

void NullVectorRenderer::rectangle(const Rectangle& bounds)
{
    (void)bounds;
}

void NullVectorRenderer::setFont(Font& font, Real size)
{
    (void)font;
    (void)size;
}

void NullVectorRenderer::text(const std::string& text, const Rectangle& bounds, HorizontalAlign horizontalAlign, VerticalAlign verticalAlign)
{
    (void)text;
    (void)bounds;
    (void)horizontalAlign;
    (void)verticalAlign;
}
