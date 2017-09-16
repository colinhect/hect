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
#include "VectorRenderer.h"

using namespace hect;

VectorRenderer::Frame::~Frame()
{
    _renderer.on_end_frame();
    _renderer._in_frame = false;
}

VectorRenderer::Frame::Frame(Frame&& frame) :
    _renderer(frame._renderer)
{
}

VectorRenderer::Frame::Frame(VectorRenderer& renderer, RenderTarget& target) :
    _renderer(renderer)
{
    renderer.on_begin_frame(target);
}

VectorRenderer::FrameStateScope::FrameStateScope(Frame& frame) :
    _frame(frame)
{
    _frame.push_state();
}

VectorRenderer::FrameStateScope::~FrameStateScope()
{
    _frame.pop_state();
}

VectorRenderer::~VectorRenderer()
{
    shutdown();
}

VectorRenderer::Frame VectorRenderer::begin_frame(RenderTarget& target)
{
    if (_in_frame)
    {
        throw InvalidOperation("Cannot begin a new frame when there is an active frame");
    }
    _in_frame = true;

    return Frame(*this, target);
}

VectorRenderer::VectorRenderer(Renderer& renderer) :
    _renderer(renderer)
{
    initialize();
}
