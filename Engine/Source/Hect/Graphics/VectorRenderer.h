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

enum VerticalAlign
{
    VerticalAlign_Bottom,
    VerticalAlign_Center,
    VerticalAlign_Top
};

enum HorizontalAlign
{
    HorizontalAlign_Left,
    HorizontalAlign_Center,
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

    virtual void pushState() = 0;
    virtual void popState() = 0;

    virtual void beginPath() = 0;
    virtual void selectFillColor(const Vector4& color) = 0;
    virtual void fill() = 0;
    virtual void rectangle(const Rectangle& bounds) = 0;

    virtual void selectFont(Font& font, Real size) = 0;
    virtual void text(const std::string& text, const Rectangle& bounds, HorizontalAlign horizontalAlign = HorizontalAlign_Center, VerticalAlign verticalAlign = VerticalAlign_Center) = 0;
};

}
