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
#include "Panel.h"

using namespace hect;

Panel::Panel(Form& form) :
    Widget(form)
{
    setStyleColor(StyleColor::Background, Color(0.0, 0.0, 0.0, 0.8));
    setStyleColor(StyleColor::BackgroundMouseOver, Color(0.0, 0.0, 0.0, 0.8));
}

void Panel::render(VectorRenderer::Frame& frame, const Rectangle& clipping)
{
    StyleColor backgroundStyleColor = StyleColor::Background;
    if (isMouseOver())
    {
        backgroundStyleColor = StyleColor::BackgroundMouseOver;
    }

    StyleColor borderStyleColor = StyleColor::Border;

    frame.pushState();
    //frame.setClipping(clipping);
    frame.beginPath();
    frame.setFillColor(styleColor(backgroundStyleColor));
    frame.rectangle(bounds());
    frame.fill();
    frame.beginPath();
    frame.rectangle(bounds());
    frame.setStrokeColor(styleColor(borderStyleColor));
    frame.stroke();

    frame.translate(position());

    WidgetBase::render(frame, clipping);

    frame.popState();
}
