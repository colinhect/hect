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
#pragma once

#include "Hect/Core/Export.h"
#include "Hect/Graphics/Font.h"
#include "Hect/UI/HorizontalAlign.h"
#include "Hect/UI/VerticalAlign.h"
#include "Hect/UI/Widget.h"

#include <string>

namespace hect
{

///
/// A textual label.
class HECT_EXPORT Label :
    public Widget<Label>
{
public:

    ///
    /// Constructs a label widget.
    ///
    /// \param text The text.
    /// \param position The position.
    /// \param dimensions The dimensions.
    /// \param font The font.
    /// \param size The font size.
    Label(const std::string& text, const Vector2& position, const Vector2& dimensions, Font::Handle font, double size);

    ///
    /// Returns the text of the label.
    const std::string& text() const;

    ///
    /// Sets the text of the label.
    ///
    /// \param text The new text of the label.
    void setText(const std::string& text);

    ///
    /// Sets the alignment of the label's text.
    ///
    /// \param horizontalAlign The horizontal alignment.
    /// \param verticalAlign The vertical alignment.
    void setAlignment(HorizontalAlign horizontalAlign, VerticalAlign verticalAlign);

    ///
    /// Returns the font of the label.
    Font::Handle font() const;

    ///
    /// Sets the font of the label.
    ///
    /// \param font The font.
    /// \param size The font size.
    void setFont(Font::Handle font, double size);

    void render(VectorRenderer::Frame& frame) override;

private:
    std::string _text;
    Font::Handle _font;
    double _size;

    HorizontalAlign _horizontalAlign { HorizontalAlign::Left };
    VerticalAlign _verticalAlign { VerticalAlign::Top };
};

}