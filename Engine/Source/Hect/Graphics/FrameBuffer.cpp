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

FrameBuffer::Attachment::Attachment(FrameBufferSlot slot, Texture2& texture) :
    _slot(slot),
    _texture(&texture)
{
}

FrameBufferSlot FrameBuffer::Attachment::slot() const
{
    return _slot;
}

Texture2& FrameBuffer::Attachment::texture()
{
    return *_texture;
}

const Texture2& FrameBuffer::Attachment::texture() const
{
    return *_texture;
}

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

    _attachments.push_back(Attachment(slot, texture));
}

FrameBuffer::AttachmentSequence FrameBuffer::attachments()
{
    return _attachments;
}

void FrameBuffer::ensureSlotEmpty(FrameBufferSlot slot)
{
    bool empty = true;

    // Check if any textures are attached to this slot
    for (const Attachment& attachment : _attachments)
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