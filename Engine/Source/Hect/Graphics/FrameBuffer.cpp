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
#include "FrameBuffer.h"

#include <cassert>

using namespace hect;

FrameBuffer::FrameBuffer()
{
}

FrameBuffer::FrameBuffer(unsigned width, unsigned height) :
    RenderTarget(width, height)
{
}

void FrameBuffer::bind(Renderer& renderer)
{
    renderer.setTarget(*this);
}

void FrameBuffer::attach(FrameBufferSlot slot, Texture2& texture)
{
    ensureSlotEmpty(slot);

    if (texture.width() != width() || texture.height() != height())
    {
        throw InvalidOperation("Cannot attach texture to a frame buffer of a different size");
    }
    else if (slot == FrameBufferSlot::Depth)
    {
        PixelFormat pixelFormat = texture.pixelFormat();
        if (pixelFormat.cardinality() != 1)
        {
            throw InvalidOperation("Cannot attach multi-channel texture to the depth slot of a frame buffer");
        }
    }

    if (isUploaded())
    {
        renderer().destroyFrameBuffer(*this);
    }

    _attachments.push_back(FrameBufferAttachment(slot, texture));
}

void FrameBuffer::attach(FrameBufferSlot slot, Texture3& texture)
{
    ensureSlotEmpty(slot);

    if (texture.width() != width() || texture.height() != height())
    {
        throw InvalidOperation("Cannot attach texture to a frame buffer of a different size");
    }
    else if (slot == FrameBufferSlot::Depth)
    {
        throw InvalidOperation("Cannot attach 3-dimensional texture to the depth slot of a frame buffer");
    }

    if (isUploaded())
    {
        renderer().destroyFrameBuffer(*this);
    }

    _attachments.push_back(FrameBufferAttachment(slot, texture));
}

void FrameBuffer::attach(FrameBufferSlot slot, CubeSide side, TextureCube& texture)
{
    ensureSlotEmpty(slot);

    if (texture.width() != width() || texture.height() != height())
    {
        throw InvalidOperation("Cannot attach texture to a frame buffer of a different size");
    }
    else if (slot == FrameBufferSlot::Depth)
    {
        throw InvalidOperation("Cannot attach cubic texture to the depth slot of a frame buffer");
    }

    if (isUploaded())
    {
        renderer().destroyFrameBuffer(*this);
    }

    _attachments.push_back(FrameBufferAttachment(slot, side, texture));
}

FrameBuffer::AttachmentSequence FrameBuffer::attachments()
{
    return _attachments;
}

void FrameBuffer::ensureSlotEmpty(FrameBufferSlot slot)
{
    bool empty = true;

    // Check if any other attachments are attached to this slot
    for (const FrameBufferAttachment& attachment : _attachments)
    {
        if (attachment.slot() == slot)
        {
            empty = false;
        }
    }

    if (!empty)
    {
        throw InvalidOperation(format("Frame buffer slot '%s' is not empty", Enum::toString(slot).data()));
    }
}
