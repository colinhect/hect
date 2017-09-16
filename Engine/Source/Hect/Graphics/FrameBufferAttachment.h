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
#include "Hect/Graphics/FrameBufferAttachmentType.h"
#include "Hect/Graphics/FrameBufferSlot.h"
#include "Hect/Graphics/Texture2.h"
#include "Hect/Graphics/Texture3.h"
#include "Hect/Graphics/TextureCube.h"

namespace hect
{

///
/// An attachment to a frame buffer.
struct HECT_EXPORT FrameBufferAttachment
{
public:

    ///
    /// Constructs 2-dimensional texture frame buffer attachment.
    ///
    /// \param slot The slot that the texture is attached to.
    /// \param texture The attached texture.
    FrameBufferAttachment(FrameBufferSlot slot, Texture2& texture);

    ///
    /// Constructs 3-dimensional texture frame buffer attachment.
    ///
    /// \param slot The slot that the texture is attached to.
    /// \param texture The attached texture.
    FrameBufferAttachment(FrameBufferSlot slot, Texture3& texture);

    ///
    /// Constructs cubic texture frame buffer attachment.
    ///
    /// \param slot The slot that the texture is attached to.
    /// \param side The side of the cubic texture to attach.
    /// \param texture The attached texture.
    FrameBufferAttachment(FrameBufferSlot slot, CubeSide side, TextureCube& texture);

    ///
    /// Returns the type of the attachment.
    FrameBufferAttachmentType type() const;

    ///
    /// Returns the slot that the texture is attached to.
    FrameBufferSlot slot() const;

    ///
    /// Returns the side of the texture that is attached.
    ///
    /// \throws InvalidOperation If the attachment is not a cubic texture.
    CubeSide side() const;

    ///
    /// Returns the attached texture.
    ///
    /// \throws InvalidOperation If the attachment is not a 2-dimensional
    /// texture.
    Texture2& texture2();

    ///
    /// \copydoc FrameBufferAttachment::texture2()
    const Texture2& texture2() const;

    ///
    /// Returns the attached texture.
    ///
    /// \throws InvalidOperation If the attachment is not a 3-dimensional
    /// texture.
    Texture3& texture3();

    ///
    /// \copydoc FrameBufferAttachment::texture3()
    const Texture3& texture3() const;

    ///
    /// Returns the attached texture.
    ///
    /// \throws InvalidOperation If the attachment is not a cubic texture.
    TextureCube& texture_cube();

    ///
    /// \copydoc FrameBufferAttachment::texture_cube()
    const TextureCube& texture_cube() const;

private:
    FrameBufferSlot _slot;
    FrameBufferAttachmentType _type;
    CubeSide _side;
    void* _texture;
};

}
