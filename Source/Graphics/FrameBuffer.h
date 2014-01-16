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

///
/// A buffer on the GPU that can be rendered to.
class FrameBuffer :
    public RenderTarget,
    public RendererObject
{
public:

    ///
    /// Constructs a frame buffer without any targets.
    FrameBuffer();

    ///
    /// Constructs a frame buffer of a given size with an optional depth
    /// component.
    ///
    /// \param targets The texture targets.
    /// \param depthComponent True if the frame buffer will have a depth
    /// component; false otherwise.
    FrameBuffer(const Texture::Array& targets, bool depthComponent = true);

    ///
    /// Destroys the frame buffer if it is uploaded.
    ~FrameBuffer();

    ///
    /// \copydoc RenderTarget::bind()
    void bind(Renderer* renderer);

    ///
    /// Returns the targets.
    Texture::Array& targets();

    ///
    /// Returns the targets.
    const Texture::Array& targets() const;

    ///
    /// Returns whether the frame buffer has a depth component.
    bool hasDepthComponent() const;

private:
    bool _depthComponent;
    Texture::Array _targets;
};

}