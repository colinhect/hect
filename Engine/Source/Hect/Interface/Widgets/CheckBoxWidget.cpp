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
#include "CheckBoxWidget.h"

using namespace hect;

CheckBoxWidget::CheckBoxWidget(InterfaceSystem& interface_system) :
    Widget(interface_system)
{
    set_dimensions(Vector2(10));
}

bool CheckBoxWidget::is_checked() const
{
    return _checked;
}

void CheckBoxWidget::set_checked(bool checked)
{
    _checked = checked;
}

void CheckBoxWidget::render(VectorRenderer::Frame& frame, Rectangle clipping)
{
    StyleColor forground_style_color = StyleColor::Foreground;
    StyleColor background_style_color = StyleColor::Background;

    if (is_mouse_over())
    {
        background_style_color = StyleColor::BackgroundMouseOver;
    }

    VectorRenderer::FrameStateScope scope(frame);

    //frame.set_clipping(clipping);
    frame.begin_path();
    frame.rectangle(bounds());
    frame.set_fill_color(style_color(background_style_color));
    frame.fill();

    frame.translate(position());

    if (_checked)
    {
        Vector2 check_position = dimensions() * 0.25;
        Vector2 check_dimensions = dimensions() * 0.75;

        frame.begin_path();
        frame.rectangle(Rectangle(check_position.ceil(), check_dimensions.floor()));
        frame.set_fill_color(style_color(forground_style_color));
        frame.fill();
    }

    WidgetBase::render(frame, clipping);
}

void CheckBoxWidget::on_pressed()
{
    _checked = !_checked;
}
