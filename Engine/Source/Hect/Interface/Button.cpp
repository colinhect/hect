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

Button::Button(InterfaceSystem& interfaceSystem) :
    Widget(interfaceSystem)
{
}

void Button::render(VectorRenderer::Frame& frame, const Rectangle& bounds)
{
    StyleColor backgroundStyleColor = StyleColor::Background;
    StyleColor borderStyleColor = StyleColor::Border;

    if (isPressed())
    {
        backgroundStyleColor = StyleColor::BackgroundPressed;
        borderStyleColor = StyleColor::BorderPressed;
    }
    else if (isMouseOver())
    {
        backgroundStyleColor = StyleColor::BackgroundMouseOver;
        borderStyleColor = StyleColor::BorderMouseOver;
    }

    frame.pushState();
    frame.setClipping(bounds);
    frame.beginPath();
    frame.rectangle(globalPosition(), dimensions());
    frame.setFillColor(styleColor(backgroundStyleColor));
    frame.fill();
    frame.beginPath();
    frame.rectangle(globalPosition(), dimensions());
    frame.setStrokeColor(styleColor(borderStyleColor));
    frame.stroke();
    frame.popState();

    WidgetBase::render(frame, bounds);
}