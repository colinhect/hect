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
#include "Hect.h"

using namespace hect;

FrameBuffer::FrameBuffer() :
    _depthComponent(false)
{
}

FrameBuffer::FrameBuffer(const Texture::Array& targets, bool depthComponent) :
    _depthComponent(depthComponent),
    _targets(targets)
{
    unsigned width = 0;
    unsigned height = 0;

    for (Texture& target : _targets)
    {
        width = std::max(width, target.width());
        height = std::max(height, target.height());
    }

    setWidth(width);
    setHeight(height);
}

FrameBuffer::~FrameBuffer()
{
    if (isUploaded())
    {
        renderer()->destroyFrameBuffer(*this);
    }
}

void FrameBuffer::bind(Renderer* renderer)
{
    renderer->bindFrameBuffer(*this);
}

Texture::Array& FrameBuffer::targets()
{
    return _targets;
}

const Texture::Array& FrameBuffer::targets() const
{
    return _targets;
}

bool FrameBuffer::hasDepthComponent() const
{
    return _depthComponent;
}