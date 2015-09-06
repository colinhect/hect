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
#include "Label.h"

#include "Hect/Logic/Systems/InterfaceSystem.h"

using namespace hect;

Label::Label(InterfaceSystem& interfaceSystem) :
    Widget(interfaceSystem)
{
}

Label::Label(InterfaceSystem& interfaceSystem, const Vector2& position, const Vector2& dimensions) :
    Widget(interfaceSystem, position, dimensions)
{
}

const std::string& Label::text() const
{
    return _text;
}

void Label::setText(const std::string& text)
{
    _text = text;
}

void Label::setTextAlignment(HorizontalAlign horizontalAlign, VerticalAlign verticalAlign)
{
    _horizontalTextAlign = horizontalAlign;
    _verticalTextAlign = verticalAlign;
}

Font::Handle Label::font() const
{
    return _font;
}

void Label::setFont(Font::Handle font, double size)
{
    _font = font;
    _fontSize = size;
}

void Label::render(VectorRenderer::Frame& frame, const Rectangle& bounds)
{
    if (!_text.empty())
    {
        Font::Handle font = _font;
        if (!font)
        {
            font = interfaceSystem().defaultFont;
        }

        double fontSize = _fontSize;
        if (fontSize <= 0.0)
        {
            fontSize = interfaceSystem().defaultFontSize;
        }

        if (font && fontSize > 0.0)
        {
            frame.pushState();
            frame.setClipping(bounds);
            frame.setFont(*font, fontSize);
            frame.renderText(_text, globalPosition(), dimensions(), _horizontalTextAlign, _verticalTextAlign);
            frame.popState();
        }
    }

    WidgetBase::render(frame, bounds);
}
