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

RenderMode::RenderMode() :
    _stateBits(RenderState::DepthTest | RenderState::CullFace),
    _sourceFactor(BlendFactor::One),
    _destFactor(BlendFactor::One)
{
}

void RenderMode::enableState(RenderState state)
{
    _stateBits |= state;
}

void RenderMode::disableState(RenderState state)
{
    _stateBits &= ~state;
}

bool RenderMode::isStateEnabled(RenderState state) const
{
    return (_stateBits & state) == state;
}

void RenderMode::setBlendFactors(BlendFactor sourceFactor, BlendFactor destFactor)
{
    _sourceFactor = sourceFactor;
    _destFactor = destFactor;
}

BlendFactor RenderMode::sourceBlendFactor() const
{
    return _sourceFactor;
}

BlendFactor RenderMode::destBlendFactor() const
{
    return _destFactor;
}