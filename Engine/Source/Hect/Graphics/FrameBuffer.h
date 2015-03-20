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

#include "Hect/Core/Sequence.h"
#include "Hect/Core/Export.h"
#include "Hect/Graphics/RenderBuffer.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/Graphics/RenderTarget.h"
#include "Hect/Graphics/Texture.h"

namespace hect
{

///
/// A slot for a frame buffer attachment.
enum class FrameBufferSlot
{
    ///
    /// The 1st color component.
    Color0,

    ///
    /// The 2nd color component.
    Color1,

    ///
    /// The 3rd color component.
    Color2,

    ///
    /// The 4th color component.
    Color3,

    ///
    /// The 5th color component.
    Color4,

    ///
    /// The 6th color component.
    Color5,

    ///
    /// The 7th color component.
    Color6,

    ///
    /// The 8th color component.
    Color7,

    ///
    /// The 9th color component.
    Color8,

    ///
    /// The 10th color component.
    Color9,

    ///
    /// The 11th color component.
    Color10,

    ///
    /// The 12th color component.
    Color11,

    ///
    /// The 13th color component.
    Color12,

    ///
    /// The 14th color component.
    Color13,

    ///
    /// The 15th color component.
    Color14,

    ///
    /// The 16th color component.
    Color15,

    ///
    /// The depth component.
    Depth,

    ///
    /// The stencil component.
    Stencil
};

///
/// A multi-target buffer on the GPU that can be rendered to.
class HECT_EXPORT FrameBuffer :
    public RenderTarget,
    public Renderer::Object<FrameBuffer>
{
public:

    ///
    /// A texture attachment.
    struct HECT_EXPORT TextureAttachment
    {
    public:

        ///
        /// Constructs a texture attachment.
        ///
        /// \param slot The slot that the texture is attached to.
        /// \param texture The attached texture.
        TextureAttachment(FrameBufferSlot slot, Texture& texture);

        ///
        /// Returns the slot that the texture is attached to.
        FrameBufferSlot slot() const;

        ///
        /// Returns the attached texture.
        Texture& texture();

        ///
        /// Returns the attached texture.
        const Texture& texture() const;

    private:
        FrameBufferSlot _slot;
        Texture* _texture;
    };

    ///
    /// A render buffer attachment.
    class HECT_EXPORT RenderBufferAttachment
    {
    public:

        ///
        /// Constructs a render buffer attachment.
        ///
        /// \param slot The slot that the render buffer is attached to.
        /// \param renderBuffer The attached render buffer.
        RenderBufferAttachment(FrameBufferSlot slot, RenderBuffer& renderBuffer);

        ///
        /// Returns the slot that the render buffer is attached to.
        FrameBufferSlot slot() const;

        ///
        /// Returns the attached render buffer.
        RenderBuffer& renderBuffer();

        ///
        /// Returns the attached render buffer.
        const RenderBuffer& renderBuffer() const;

    private:
        FrameBufferSlot _slot;
        RenderBuffer* _renderBuffer;
    };

private:
    typedef std::vector<TextureAttachment> TextureAttachmentContainer;
    typedef std::vector<RenderBufferAttachment> RenderBufferAttachmentContainer;

public:

    ///
    /// A sequence of texture attachments.
    typedef Sequence<TextureAttachment, TextureAttachmentContainer> TextureAttachmentSequence;

    ///
    /// A sequence of render buffer attachments.
    typedef Sequence<RenderBufferAttachment, RenderBufferAttachmentContainer> RenderBufferAttachmentSequence;

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
    /// Attaches a texture to the frame buffer.
    ///
    /// \note If the frame buffer is uploaded to a renderer then it
    /// will be destroyed before the texture is attached.
    ///
    /// \param slot The slot to attach the texture to.
    /// \param texture The texture to attach to the frame buffer.
    ///
    /// \throws InvalidOperation If something is already attached to the given
    /// slot or if the texture is not the same size as the frame buffer.
    void attachTexture(FrameBufferSlot slot, Texture& texture);

    ///
    /// Returns the texture attachments.
    TextureAttachmentSequence textureAttachments();

    ///
    /// Attaches a render buffer to the frame buffer.
    ///
    /// \note If the frame buffer is uploaded to a renderer then it
    /// will be destroyed before the render buffer is attached.
    ///
    /// \param slot The slot to attach the texture to.
    /// \param renderBuffer The render buffer to attach to the frame buffer.
    ///
    /// \throws InvalidOperation If something is already attached to the given
    /// slot or if the render buffer is not the same size as the frame buffer.
    void attachRenderBuffer(FrameBufferSlot slot, RenderBuffer& renderBuffer);

    ///
    /// Returns the render buffer attachments.
    RenderBufferAttachmentSequence renderBufferAttachments();

private:
    void ensureSlotEmpty(FrameBufferSlot slot);

    TextureAttachmentContainer _textureAttachments;
    RenderBufferAttachmentContainer _renderBufferAttachments;
};

}
