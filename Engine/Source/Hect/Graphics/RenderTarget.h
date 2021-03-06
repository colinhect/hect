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
#include "Hect/Math/Vector2.h"

#include <memory>

namespace hect
{

class Renderer;

///
/// A target that can be rendered to.
class HECT_EXPORT RenderTarget
{
public:

    ///
    /// Constructs a render target.
    RenderTarget();

    ///
    /// Constructs a render target.
    ///
    /// \param width The width.
    /// \param height The height.
    RenderTarget(unsigned width, unsigned height);

    virtual ~RenderTarget() { }

    ///
    /// Sets this target as the active target to the given renderer.
    ///
    /// \param renderer The renderer to set the target for.
    virtual void bind(Renderer& renderer) = 0;

    ///
    /// Swaps the back buffer.
    ///
    /// \throws InvalidOperation If the render target does not have a back
    /// buffer.
    virtual void swap_buffers();

    ///
    /// Returns the width.
    unsigned width() const;

    ///
    /// Returns the height.
    unsigned height() const;

    ///
    /// Returns the dimensions.
    Vector2 dimensions() const;

    ///
    /// Returns the aspect ratio.
    float aspect_ratio() const;

protected:

    ///
    /// Sets the width.
    ///
    /// \param width The new width.
    void set_width(unsigned width);

    ///
    /// Sets the height.
    ///
    /// \param height The new height.
    void set_height(unsigned height);

private:
    unsigned _width { 0 };
    unsigned _height { 0 };
};

}