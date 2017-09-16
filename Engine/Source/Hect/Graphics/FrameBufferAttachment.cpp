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
#include "FrameBufferAttachment.h"

using namespace hect;

FrameBufferAttachment::FrameBufferAttachment(FrameBufferSlot slot, Texture2& texture) :
    _slot(slot),
    _type(FrameBufferAttachmentType::Texture2),
    _side(CubeSide::PositiveX),
    _texture(&texture)
{
}

FrameBufferAttachment::FrameBufferAttachment(FrameBufferSlot slot, Texture3& texture) :
    _slot(slot),
    _type(FrameBufferAttachmentType::Texture3),
    _side(CubeSide::PositiveX),
    _texture(&texture)
{
}

FrameBufferAttachment::FrameBufferAttachment(FrameBufferSlot slot, CubeSide side, TextureCube& texture) :
    _slot(slot),
    _type(FrameBufferAttachmentType::TextureCube),
    _side(side),
    _texture(&texture)
{
}

FrameBufferAttachmentType FrameBufferAttachment::type() const
{
    return _type;
}

FrameBufferSlot FrameBufferAttachment::slot() const
{
    return _slot;
}

CubeSide FrameBufferAttachment::side() const
{
    if (_type != FrameBufferAttachmentType::TextureCube)
    {
        throw InvalidOperation("Attachment is not a cubic texture");
    }

    return _side;
}

Texture2& FrameBufferAttachment::texture2()
{
    if (_type != FrameBufferAttachmentType::Texture2)
    {
        throw InvalidOperation("Attachment is not a 2-dimensional texture");
    }

    return *reinterpret_cast<Texture2*>(_texture);
}

const Texture2& FrameBufferAttachment::texture2() const
{
    if (_type != FrameBufferAttachmentType::Texture2)
    {
        throw InvalidOperation("Attachment is not a 2-dimensional texture");
    }

    return *reinterpret_cast<Texture2*>(_texture);
}

Texture3& FrameBufferAttachment::texture3()
{
    if (_type != FrameBufferAttachmentType::Texture3)
    {
        throw InvalidOperation("Attachment is not a 3-dimensional texture");
    }

    return *reinterpret_cast<Texture3*>(_texture);
}

const Texture3& FrameBufferAttachment::texture3() const
{
    if (_type != FrameBufferAttachmentType::Texture3)
    {
        throw InvalidOperation("Attachment is not a 3-dimensional texture");
    }

    return *reinterpret_cast<const Texture3*>(_texture);
}

TextureCube& FrameBufferAttachment::texture_cube()
{
    if (_type != FrameBufferAttachmentType::TextureCube)
    {
        throw InvalidOperation("Attachment is not a cubic texture");
    }

    return *reinterpret_cast<TextureCube*>(_texture);
}

const TextureCube& FrameBufferAttachment::texture_cube() const
{
    if (_type != FrameBufferAttachmentType::TextureCube)
    {
        throw InvalidOperation("Attachment is not a cubic texture");
    }

    return *reinterpret_cast<const TextureCube*>(_texture);
}
