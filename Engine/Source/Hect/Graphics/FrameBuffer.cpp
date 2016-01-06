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

FrameBuffer::Attachment2::Attachment2(FrameBufferSlot slot, Texture2& texture) :
    _slot(slot),
    _texture(&texture)
{
}

FrameBufferSlot FrameBuffer::Attachment2::slot() const
{
    return _slot;
}

Texture2& FrameBuffer::Attachment2::texture()
{
    assert(_texture);
    return *_texture;
}

const Texture2& FrameBuffer::Attachment2::texture() const
{
    assert(_texture);
    return *_texture;
}

FrameBuffer::Attachment3::Attachment3(FrameBufferSlot slot, Texture3& texture) :
    _slot(slot),
    _texture(&texture)
{
}

FrameBufferSlot FrameBuffer::Attachment3::slot() const
{
    return _slot;
}

Texture3& FrameBuffer::Attachment3::texture()
{
    assert(_texture);
    return *_texture;
}

const Texture3& FrameBuffer::Attachment3::texture() const
{
    assert(_texture);
    return *_texture;
}

FrameBuffer::AttachmentCube::AttachmentCube(FrameBufferSlot slot, CubeSide side, TextureCube& texture) :
    _slot(slot),
    _side(side),
    _texture(&texture)
{
}

FrameBufferSlot FrameBuffer::AttachmentCube::slot() const
{
    return _slot;
}

CubeSide FrameBuffer::AttachmentCube::side() const
{
    return _side;
}

TextureCube& FrameBuffer::AttachmentCube::texture()
{
    assert(_texture);
    return *_texture;
}

const TextureCube& FrameBuffer::AttachmentCube::texture() const
{
    assert(_texture);
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

    _attachments2.push_back(Attachment2(slot, texture));
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

    _attachments3.push_back(Attachment3(slot, texture));
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

    _attachmentsCube.push_back(AttachmentCube(slot, side, texture));
}

FrameBuffer::Attachment2Sequence FrameBuffer::attachments2()
{
    return _attachments2;
}

FrameBuffer::Attachment3Sequence FrameBuffer::attachments3()
{
    return _attachments3;
}

FrameBuffer::AttachmentCubeSequence FrameBuffer::attachmentsCube()
{
    return _attachmentsCube;
}

void FrameBuffer::ensureSlotEmpty(FrameBufferSlot slot)
{
    bool empty = true;

    // Check if any 2-dimensional textures are attached to this slot
    for (const Attachment2& attachment : _attachments2)
    {
        if (attachment.slot() == slot)
        {
            empty = false;
        }
    }

    // Check if any 3-dimensional textures are attached to this slot
    for (const Attachment3& attachment : _attachments3)
    {
        if (attachment.slot() == slot)
        {
            empty = false;
        }
    }

    // Check if any cubic textures are attached to this slot
    for (const AttachmentCube& attachment : _attachmentsCube)
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
