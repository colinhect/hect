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
#include "Hect/Graphics/Renderer.h"

namespace hect
{

///
/// The format of a render buffer.
enum RenderBufferFormat
{
    ///
    /// A depth render buffer.
    RenderBufferFormat_DepthComponent
};

///
/// A render buffer to be used with a frame buffer.
class HECT_EXPORT RenderBuffer :
    public Renderer::Object<RenderBuffer>
{
public:

    ///
    /// Constructs a render buffer.
    RenderBuffer();

    ///
    /// Constructs a render buffer.
    ///
    /// \param format The format.
    /// \param width The width.
    /// \param height The height.
    RenderBuffer(RenderBufferFormat format, unsigned width, unsigned height);

    ///
    /// Returns the format.
    RenderBufferFormat format() const;

    ///
    /// Returns the width.
    unsigned width() const;

    ///
    /// Returns the height.
    unsigned height() const;

    ///
    /// Returns the number of bytes in a pixel of this render buffer.
    unsigned bytesPerPixel() const;

private:
    RenderBufferFormat _format { RenderBufferFormat_DepthComponent };
    unsigned _width { 0 };
    unsigned _height { 0 };
};

}
