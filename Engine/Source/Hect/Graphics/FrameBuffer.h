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
public:

    ///
    /// A 2-dimensional frame buffer attachment.
    struct HECT_EXPORT Attachment2
    {
    public:

        ///
        /// Constructs a frame buffer attachment.
        ///
        /// \param slot The slot that the texture is attached to.
        /// \param texture The attached texture.
        Attachment2(FrameBufferSlot slot, Texture2& texture);

        ///
        /// Returns the slot that the texture is attached to.
        FrameBufferSlot slot() const;

        ///
        /// Returns the attached texture.
        Texture2& texture();

        ///
        /// Returns the attached texture.
        const Texture2& texture() const;

    private:
        FrameBufferSlot _slot;
        Texture2* _texture;
    };

    ///
    /// A 3-dimensional frame buffer attachment.
    struct HECT_EXPORT Attachment3
    {
    public:

        ///
        /// Constructs a frame buffer attachment.
        ///
        /// \param slot The slot that the texture is attached to.
        /// \param texture The attached texture.
        Attachment3(FrameBufferSlot slot, Texture3& texture);

        ///
        /// Returns the slot that the texture is attached to.
        FrameBufferSlot slot() const;

        ///
        /// Returns the attached texture.
        Texture3& texture();

        ///
        /// Returns the attached texture.
        const Texture3& texture() const;

    private:
        FrameBufferSlot _slot;
        Texture3* _texture;
    };

    ///
    /// A cubic frame buffer attachment.
    struct HECT_EXPORT AttachmentCube
    {
    public:

        ///
        /// Constructs a frame buffer attachment.
        ///
        /// \param slot The slot that the texture is attached to.
        /// \param side The side of the cubic texture to attach.
        /// \param texture The attached texture.
        AttachmentCube(FrameBufferSlot slot, CubeSide side, TextureCube& texture);

        ///
        /// Returns the slot that the texture is attached to.
        FrameBufferSlot slot() const;

        ///
        /// Returns the side of the texture that is attached.
        CubeSide side() const;

        ///
        /// Returns the attached texture.
        TextureCube& texture();

        ///
        /// Returns the attached texture.
        const TextureCube& texture() const;

    private:
        FrameBufferSlot _slot;
        CubeSide _side;
        TextureCube* _texture;
    };

private:
    typedef std::vector<Attachment2> Attachment2Container;
    typedef std::vector<Attachment3> Attachment3Container;
    typedef std::vector<AttachmentCube> AttachmentCubeContainer;

public:

    ///
    /// A sequence of 2-dimensional texture attachments.
    typedef Sequence<Attachment2, Attachment2Container> Attachment2Sequence;

    ///
    /// A sequence of 3-dimensional texture attachments.
    typedef Sequence<Attachment3, Attachment3Container> Attachment3Sequence;

    ///
    /// A sequence of cubic texture attachments.
    typedef Sequence<AttachmentCube, AttachmentCubeContainer> AttachmentCubeSequence;

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
    /// Returns the 2-dimensional texture attachments.
    Attachment2Sequence attachments2();

    ///
    /// Returns the 3-dimensional texture attachments.
    Attachment3Sequence attachments3();

    ///
    /// Returns the cubic texture attachments.
    AttachmentCubeSequence attachmentsCube();

private:
    void ensureSlotEmpty(FrameBufferSlot slot);

    Attachment2Container _attachments2;
    Attachment3Container _attachments3;
    AttachmentCubeContainer _attachmentsCube;
};

}
