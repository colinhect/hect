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
#include "Hect/Graphics/Color.h"
#include "Hect/Interface/Widget.h"

namespace hect
{

///
/// A panel.
class HECT_EXPORT Panel :
    public Widget<Panel>
{
public:

    ///
    /// Constructs an empty panel widget.
    Panel();

    ///
    /// Constructs a panel widget.
    ///
    /// \param position The local position.
    /// \param dimensions The dimensions.
    Panel(const Vector2& position, const Vector2& dimensions);

    ///
    /// Returns the background color of the panel.
    const Color& backgroundColor() const;

    ///
    /// Sets the background color of the panel.
    ///
    /// \param color The new background color of the panel.
    void setBackgroundColor(const Color& color);

    void render(VectorRenderer::Frame& frame, const Rectangle& bounds) override;

private:
    Color _backgroundColor { Color(0.0, 0.0, 0.0, 0.8) };
};

}
