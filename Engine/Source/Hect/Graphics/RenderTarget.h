///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2014 Colin Hill
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

namespace hect
{

class GraphicsContext;

///
/// A target that can be rendered to.
class RenderTarget
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
    /// Binds this target as the active target to the given graphics context.
    ///
    /// \param graphicsContext The graphics context to bind the target for.
    virtual void bind(GraphicsContext& graphicsContext) = 0;

    ///
    /// Swaps the back buffer.
    ///
    /// \throws Error If the render target does not have a back buffer.
    virtual void swapBuffers();

    ///
    /// Returns the width.
    unsigned width() const;

    ///
    /// Returns the height.
    unsigned height() const;

    ///
    /// Returns the component ratio.
    float aspectRatio() const;

protected:

    ///
    /// Sets the width.
    ///
    /// \param width The new width.
    void setWidth(unsigned width);

    ///
    /// Sets the height.
    ///
    /// \param height The new height.
    void setHeight(unsigned height);

private:
    unsigned _width { 0 };
    unsigned _height { 0 };
};

}