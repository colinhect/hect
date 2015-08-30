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

using namespace hect;

Label::Label() :
    _fontSize(0)
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

void Label::setAlignment(HorizontalAlign horizontalAlign, VerticalAlign verticalAlign)
{
    _horizontalAlign = horizontalAlign;
    _verticalAlign = verticalAlign;
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
    if (_font && _fontSize > 0.0 && !_text.empty())
    {
        frame.setClipping(bounds);
        frame.setFont(*_font, _fontSize);
        frame.renderText(_text, position(), dimensions(), _horizontalAlign, _verticalAlign);
    }
}
