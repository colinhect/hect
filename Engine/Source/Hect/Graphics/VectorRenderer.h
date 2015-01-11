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
#include "Hect/Core/Uncopyable.h"
#include "Hect/Graphics/Font.h"
#include "Hect/Graphics/RenderTarget.h"
#include "Hect/Math/Vector2.h"
#include "Hect/Spacial/Rectangle.h"

namespace hect
{

///
/// Vertical alignment mode.
enum VerticalAlign
{
    ///
    /// Align to the bottom.
    VerticalAlign_Bottom,

    ///
    /// Align to the center.
    VerticalAlign_Center,

    ///
    /// Align to the top.
    VerticalAlign_Top
};

///
/// Horizontal alignment mode.
enum HorizontalAlign
{
    ///
    /// Align to the left.
    HorizontalAlign_Left,

    ///
    /// Align to the center.
    HorizontalAlign_Center,

    ///
    /// Align to the right.
    HorizontalAlign_Right
};

///
/// Provides the functionality for rendering vector graphics.
class HECT_EXPORT VectorRenderer :
    public Uncopyable
{
public:
    virtual ~VectorRenderer() { }

    ///
    /// Establishes the beginning of a frame.
    ///
    /// \note Any state changes are only persistent within a frame.
    ///
    /// \param target The target to render to.
    virtual void beginFrame(RenderTarget& target) = 0;

    ///
    /// Establishes the ending of a frame.
    virtual void endFrame() = 0;

    ///
    /// Pushes the current state onto the stack.
    virtual void pushState() = 0;

    ///
    /// Pops the previously pushed state as the new current state.
    virtual void popState() = 0;

    ///
    /// Begins a new path.
    virtual void beginPath() = 0;

    ///
    /// Sets the active fill style to a solid color.
    ///
    /// \param color The color to set.
    virtual void setFillColor(const Vector4& color) = 0;

    ///
    /// Fills the current path with the current fill style.
    virtual void fill() = 0;

    ///
    /// Creates a new rectangle shape sub-path.
    ///
    /// \param bounds The bounds of the rectangle.
    virtual void rectangle(const Rectangle& bounds) = 0;

    ///
    /// Sets the current font face and size.
    ///
    /// \param font The font.
    /// \param size The font size.
    virtual void setFont(Font& font, Real size) = 0;

    ///
    /// Renders text.
    ///
    /// \param text The text to render.
    /// \param bounds The bounds of the text.
    /// \param horizontalAlign The horizontal alignment within the bounds.
    /// \param verticalAlign The vertical alignment within the bounds.
    virtual void text(const std::string& text, const Rectangle& bounds, HorizontalAlign horizontalAlign = HorizontalAlign_Center, VerticalAlign verticalAlign = VerticalAlign_Center) = 0;
};

}
