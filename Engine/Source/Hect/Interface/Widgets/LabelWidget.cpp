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
#include "LabelWidget.h"

#include "Hect/Scene/Systems/InterfaceSystem.h"

using namespace hect;

LabelWidget::LabelWidget(InterfaceSystem& interface_system) :
    Widget(interface_system)
{
}

const std::string& LabelWidget::text() const
{
    return _text;
}

void LabelWidget::set_text(const std::string& text)
{
    _text = text;
    mark_layout_dirty();
}

AssetHandle<Font> LabelWidget::font() const
{
    return _font;
}

void LabelWidget::set_font(AssetHandle<Font> font, double size)
{
    _font = font;
    _font_size = size;
    mark_layout_dirty();
}

void LabelWidget::render(VectorRenderer::Frame& frame, Rectangle clipping)
{
    VectorRenderer::FrameStateScope scope(frame);

    //frame.set_clipping(bounds);

    if (!_text.empty())
    {
        StyleColor forground_style_color = StyleColor::Foreground;

        const Font& font = effective_font();
        const double font_size = effective_font_size();

        frame.translate(position());
        frame.set_font(font, font_size);
        frame.set_fill_color(style_color(forground_style_color));
        frame.render_text(_text);
    }

    WidgetBase::render(frame, clipping);
}

void LabelWidget::update_layout()
{
    Vector2 dimensions = interface_system().measure_text_dimensions(_text, effective_font(), effective_font_size());
    set_dimensions(dimensions);
    WidgetBase::update_layout();
}

Font& LabelWidget::effective_font()
{
    // If no font is set then defer to the interface system
    AssetHandle<Font> font = _font;
    if (!font)
    {
        font = interface_system().default_font;
    }

    return *font;
}

double LabelWidget::effective_font_size()
{
    // If no font size is set then defer to the interface system
    double font_size = _font_size;
    if (font_size <= 0.0)
    {
        font_size = interface_system().default_font_size;
    }

    return font_size;
}
