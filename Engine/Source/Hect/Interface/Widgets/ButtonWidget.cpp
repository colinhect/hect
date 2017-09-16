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
#include "ButtonWidget.h"

using namespace hect;

ButtonWidget::ButtonWidget(InterfaceSystem& interface_system) :
    Widget(interface_system)
{
}

void ButtonWidget::render(VectorRenderer::Frame& frame, Rectangle clipping)
{
    StyleColor background_style_color = StyleColor::Background;
    StyleColor border_style_color = StyleColor::Border;

    if (is_pressed())
    {
        background_style_color = StyleColor::BackgroundPressed;
        border_style_color = StyleColor::BorderPressed;
    }
    else if (is_mouse_over())
    {
        background_style_color = StyleColor::BackgroundMouseOver;
        border_style_color = StyleColor::BorderMouseOver;
    }

    VectorRenderer::FrameStateScope scope(frame);

    //frame.set_clipping(clipping);
    frame.begin_path();
    frame.rectangle(bounds());
    frame.set_fill_color(style_color(background_style_color));
    frame.fill();
    frame.begin_path();
    frame.rectangle(bounds());
    frame.set_stroke_color(style_color(border_style_color));
    frame.stroke();

    frame.translate(position());

    WidgetBase::render(frame, clipping);
}
