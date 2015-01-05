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
#include "FrameBuffer.h"

using namespace hect;

FrameBuffer::TextureAttachment::TextureAttachment(FrameBufferSlot slot, Texture& texture) :
    _slot(slot),
    _texture(&texture)
{
}

FrameBufferSlot FrameBuffer::TextureAttachment::slot() const
{
    return _slot;
}

Texture& FrameBuffer::TextureAttachment::texture()
{
    return *_texture;
}

const Texture& FrameBuffer::TextureAttachment::texture() const
{
    return *_texture;
}

FrameBuffer::RenderBufferAttachment::RenderBufferAttachment(FrameBufferSlot slot, RenderBuffer& renderBuffer) :
    _slot(slot),
    _renderBuffer(&renderBuffer)
{
}

FrameBufferSlot FrameBuffer::RenderBufferAttachment::slot() const
{
    return _slot;
}

RenderBuffer& FrameBuffer::RenderBufferAttachment::renderBuffer()
{
    return *_renderBuffer;
}

const RenderBuffer& FrameBuffer::RenderBufferAttachment::renderBuffer() const
{
    return *_renderBuffer;
}

FrameBuffer::FrameBuffer()
{
}

FrameBuffer::FrameBuffer(unsigned width, unsigned height) :
    RenderTarget(width, height)
{
}

void FrameBuffer::select(Renderer& renderer)
{
    renderer.selectTarget(*this);
}

void FrameBuffer::attachTexture(FrameBufferSlot slot, Texture& texture)
{
    ensureSlotEmpty(slot);

    if (texture.width() != width() || texture.height() != height())
    {
        throw InvalidOperation("Cannot attach texture to a frame buffer of a different size");
    }

    if (isUploaded())
    {
        renderer().destroyFrameBuffer(*this);
    }

    _textureAttachments.push_back(TextureAttachment(slot, texture));
}

FrameBuffer::TextureAttachmentSequence FrameBuffer::textureAttachments()
{
    return _textureAttachments;
}

void FrameBuffer::attachRenderBuffer(FrameBufferSlot slot, RenderBuffer& renderBuffer)
{
    ensureSlotEmpty(slot);

    if (renderBuffer.width() != width() || renderBuffer.height() != height())
    {
        throw InvalidOperation("Cannot attach render buffer to a frame buffer of a different size");
    }

    if (isUploaded())
    {
        renderer().destroyFrameBuffer(*this);
    }

    _renderBufferAttachments.push_back(RenderBufferAttachment(slot, renderBuffer));
}

FrameBuffer::RenderBufferAttachmentSequence FrameBuffer::renderBufferAttachments()
{
    return _renderBufferAttachments;
}

void FrameBuffer::ensureSlotEmpty(FrameBufferSlot slot)
{
    bool empty = true;

    // Check if any textures are attached to this slot
    for (const TextureAttachment& attachment : _textureAttachments)
    {
        if (attachment.slot() == slot)
        {
            empty = false;
        }
    }

    // Check if any render buffers are attached to this slot
    for (const RenderBufferAttachment& attachment : _renderBufferAttachments)
    {
        if (attachment.slot() == slot)
        {
            empty = false;
        }
    }

    if (!empty)
    {
        throw InvalidOperation(format("Frame buffer slot '%s' is not empty", Enum::toString(slot).c_str()));
    }
}