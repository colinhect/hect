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
#pragma once

#include "Hect/Core/Export.h"
#include "Hect/Graphics/Font.h"
#include "Hect/Interface/Widget.h"

#include <string>

namespace hect
{

///
/// A textual label.
class HECT_EXPORT LabelWidget :
    public Widget<LabelWidget>
{
public:

    ///
    /// Constructs an empty label widget.
    ///
    /// \param interface_system The interface system.
    LabelWidget(InterfaceSystem& interface_system);

    ///
    /// Returns the text of the label.
    const std::string& text() const;

    ///
    /// Sets the text of the label.
    ///
    /// \note The dimensions of the label will automatically size to fit the
    /// text with the set font and font size.
    ///
    /// \param text The new text of the label.
    void set_text(const std::string& text);

    ///
    /// Returns the font of the label.
    Font::Handle font() const;

    ///
    /// Sets the font of the label.
    ///
    /// \note The dimensions of the label will automatically size to fit the
    /// text with the set font and font size.
    ///
    /// \param font The font.
    /// \param size The font size.
    void set_font(Font::Handle font, double size);

    // Widget overrides
    void render(VectorRenderer::Frame& frame, Rectangle clipping) override;

private:

    // Widget overrides
    void update_layout() override;

    Font& effective_font();
    double effective_font_size();

    std::string _text;
    Font::Handle _font;
    double _font_size { 0.0 };
};

}
