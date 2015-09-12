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
#include "Hect/Graphics/VectorRenderer.h"

#include "Hect/Core/Configuration.h"

#ifndef HECT_RENDERER_OPENGL

using namespace hect;

void VectorRenderer::Frame::pushState()
{
}

void VectorRenderer::Frame::popState()
{
}

void VectorRenderer::Frame::beginPath()
{
}

void VectorRenderer::Frame::setFillColor(const Color& color)
{
    (void)color;
}

void VectorRenderer::Frame::setFillGradient(const Vector2& startPosition, const Vector2& endPosition, const Color& startColor, const Color& endColor)
{
    (void)startPosition;
    (void)endPosition;
    (void)startColor;
    (void)endColor;
}

void VectorRenderer::Frame::fill()
{
}

void VectorRenderer::Frame::setStrokeColor(const Color& color)
{
    (void)color;
}

void VectorRenderer::Frame::stroke()
{
}

void VectorRenderer::Frame::rectangle(const Vector2& position, const Vector2& dimensions)
{
    (void)position;
    (void)dimensions;
}

void VectorRenderer::Frame::setClipping(const Rectangle& bounds)
{
    (void)bounds;
}

void VectorRenderer::Frame::setFont(const Font& font, double size)
{
    (void)font;
    (void)size;
}

void VectorRenderer::Frame::renderText(const std::string& text, const Vector2& position)
{
    (void)text;
    (void)position;
}

Vector2 VectorRenderer::measureTextDimensions(const std::string& text, const Font& font, double size)
{
    (void)text;
    (void)font;
    (double)size;

    return Vector2();
}

void VectorRenderer::initialize()
{
}

void VectorRenderer::shutdown()
{
}

void VectorRenderer::onBeginFrame(RenderTarget& target)
{
    (void)target;
}

void VectorRenderer::onEndFrame()
{
}

#endif
