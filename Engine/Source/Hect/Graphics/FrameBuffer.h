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

#include "Hect/Core/Sequence.h"
#include "Hect/Core/Export.h"
#include "Hect/Graphics/FrameBufferAttachment.h"
#include "Hect/Graphics/FrameBufferSlot.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/Graphics/RenderTarget.h"
#include "Hect/Graphics/Texture2.h"
#include "Hect/Graphics/Texture3.h"
#include "Hect/Graphics/TextureCube.h"

namespace hect
{

///
/// A multi-target buffer on the GPU that can be rendered to.
class HECT_EXPORT FrameBuffer :
    public RenderTarget,
    public Renderer::Object<FrameBuffer>
{
    typedef std::vector<FrameBufferAttachment> AttachmentContainer;

public:

    ///
    /// A sequence of frame buffer attachments.
    typedef Sequence<FrameBufferAttachment, AttachmentContainer> AttachmentSequence;

    ///
    /// Constructs a frame buffer.
    FrameBuffer();

    ///
    /// Constructs a frame buffer.
    ///
    /// \param width The width.
    /// \param height The height.
    FrameBuffer(unsigned width, unsigned height);

    void bind(Renderer& renderer) override;

    ///
    /// Attaches a 2-dimensional texture to the frame buffer.
    ///
    /// \note If the frame buffer is uploaded to a renderer then it
    /// will be destroyed before the texture is attached.
    ///
    /// \param slot The slot to attach the texture to.
    /// \param texture The texture to attach to the frame buffer.
    ///
    /// \throws InvalidOperation If something is already attached to the given
    /// slot or if the texture is not the same size as the frame buffer.
    void attach(FrameBufferSlot slot, Texture2& texture);

    ///
    /// Attaches a 3-dimensional texture to the frame buffer.
    ///
    /// \note If the frame buffer is uploaded to a renderer then it
    /// will be destroyed before the texture is attached.
    ///
    /// \param slot The slot to attach the texture to.
    /// \param texture The texture to attach to the frame buffer.
    ///
    /// \throws InvalidOperation If something is already attached to the given
    /// slot or if the texture is not the same size as the frame buffer.
    void attach(FrameBufferSlot slot, Texture3& texture);

    ///
    /// Attaches a cubic texture to the frame buffer.
    ///
    /// \note If the frame buffer is uploaded to a renderer then it
    /// will be destroyed before the texture is attached.
    ///
    /// \param slot The slot to attach the texture to.
    /// \param side The side of the cubic texture to attach.
    /// \param texture The texture to attach to the frame buffer.
    ///
    /// \throws InvalidOperation If something is already attached to the given
    /// slot or if the texture is not the same size as the frame buffer.
    void attach(FrameBufferSlot slot, CubeSide side, TextureCube& texture);

    ///
    /// Returns the attachments.
    AttachmentSequence attachments();

private:
    void ensureSlotEmpty(FrameBufferSlot slot);

    AttachmentContainer _attachments;
};

}
