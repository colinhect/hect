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

#include "Hect/Core/Sequence.h"
#include "Hect/Graphics/GraphicsContext.h"
#include "Hect/Graphics/RenderTarget.h"
#include "Hect/Graphics/Texture.h"

namespace hect
{

///
/// A multi-target buffer on the GPU that can be rendered to.
class FrameBuffer :
    public RenderTarget,
    public GraphicsContext::Object<FrameBuffer>
{
    typedef std::vector<Texture> TextureContainer;
public:

    ///
    /// A sequence of textures.
    typedef Sequence<Texture, TextureContainer> TextureSequence;

    void bind(GraphicsContext& graphicsContext) override;

    ///
    /// Returns the targets.
    TextureSequence targets();

    ///
    /// Returns the targets.
    const TextureSequence targets() const;

    ///
    /// Adds a target to the frame buffer.
    ///
    /// \note If the frame buffer is uploaded to a graphics context then it
    /// will be destroyed before the target is added.
    ///
    /// \param target The target to add to the frame buffer.
    void addTarget(const Texture& target);

    ///
    /// Returns whether the frame buffer has a depth component.
    bool hasDepthComponent() const;

    ///
    /// Sets whether the frame buffer has a depth component.
    ///
    /// \note If the frame buffer is uploaded to a graphics context then it
    /// will be destroyed before the depth component changes.
    ///
    /// \param depthComponent True if the frame buffer has a depth component;
    /// false otherwise.
    void setDepthComponent(bool depthComponent);

private:
    bool _depthComponent { true };
    TextureContainer _targets;
};

}