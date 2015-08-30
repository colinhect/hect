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
#include "Button.h"

using namespace hect;

Button::Button()
{
}

Button::Button(const Vector2& position, const Vector2& dimensions) :
    Widget(position, dimensions)
{
}

const Color& Button::backgroundColor() const
{
    return _backgroundColor;
}

void Button::setBackgroundColor(const Color& color)
{
    _backgroundColor = color;
}

const Color& Button::borderColor() const
{
    return _borderColor;
}

void Button::setBorderColor(const Color& color)
{
    _borderColor = color;
}

void Button::render(VectorRenderer::Frame& frame, const Rectangle& bounds)
{
    frame.setClipping(bounds);
    frame.beginPath();
    frame.rectangle(position(), dimensions());

    Color lightColor = _backgroundColor * 1.1;
    lightColor.a = 1.0;
    Color darkColor = _backgroundColor * 0.9;
    darkColor.a = 1.0;
    frame.setFillGradient(position(), position() + Vector2::UnitY * dimensions().y, lightColor, darkColor);

    frame.fill();

    frame.beginPath();
    frame.rectangle(position(), dimensions());
    frame.setStrokeColor(_borderColor);
    frame.stroke();
}
