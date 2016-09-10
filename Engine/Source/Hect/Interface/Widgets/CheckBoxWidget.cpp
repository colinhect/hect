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

CheckBoxWidget::CheckBoxWidget(InterfaceSystem& interfaceSystem) :
    Widget(interfaceSystem)
{
    setDimensions(Vector2(10));
}

bool CheckBoxWidget::isChecked() const
{
    return _checked;
}

void CheckBoxWidget::setChecked(bool checked)
{
    _checked = checked;
}

void CheckBoxWidget::render(VectorRenderer::Frame& frame, Rectangle clipping)
{
    StyleColor forgroundStyleColor = StyleColor::Foreground;
    StyleColor backgroundStyleColor = StyleColor::Background;

    if (isMouseOver())
    {
        backgroundStyleColor = StyleColor::BackgroundMouseOver;
    }

    VectorRenderer::FrameStateScope scope(frame);

    //frame.setClipping(clipping);
    frame.beginPath();
    frame.rectangle(bounds());
    frame.setFillColor(styleColor(backgroundStyleColor));
    frame.fill();

    frame.translate(position());

    if (_checked)
    {
        Vector2 checkPosition = dimensions() * 0.25;
        Vector2 checkDimensions = dimensions() * 0.75;

        frame.beginPath();
        frame.rectangle(Rectangle(checkPosition.ceil(), checkDimensions.floor()));
        frame.setFillColor(styleColor(forgroundStyleColor));
        frame.fill();
    }

    WidgetBase::render(frame, clipping);
}

void CheckBoxWidget::onPressed()
{
    _checked = !_checked;
}
