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
#include "RenderState.h"

using namespace hect;

RenderState::RenderState() :
    _flagBits(RenderStateFlag::DepthTest | RenderStateFlag::CullFace),
    _sourceFactor(BlendFactor::One),
    _destFactor(BlendFactor::One)
{
}

void RenderState::enable(RenderStateFlag::Enum flag)
{
    _flagBits |= flag;
}

void RenderState::disable(RenderStateFlag::Enum flag)
{
    _flagBits &= ~flag;
}

bool RenderState::isEnabled(RenderStateFlag::Enum flag) const
{
    return (_flagBits & flag) == flag;
}

void RenderState::setBlendFactors(BlendFactor::Enum sourceFactor, BlendFactor::Enum destFactor)
{
    _sourceFactor = sourceFactor;
    _destFactor = destFactor;
}

BlendFactor::Enum RenderState::sourceBlendFactor() const
{
    return _sourceFactor;
}

BlendFactor::Enum RenderState::destBlendFactor() const
{
    return _destFactor;
}

bool RenderState::operator==(const RenderState& renderState) const
{
    return _flagBits == renderState._flagBits
           && _sourceFactor == renderState.sourceBlendFactor()
           && _destFactor == renderState.destBlendFactor();
}

bool RenderState::operator!=(const RenderState& renderState) const
{
    return !(*this == renderState);
}