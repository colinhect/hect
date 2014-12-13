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
#pragma once

namespace hect
{

///
/// A formula used to compute blending from either the source or destination.
enum BlendFactor
{
    ///
    /// Blend forumla: 0 0 0 0
    BlendFactor_Zero,

    ///
    /// Blend forumla: 1 1 1 1
    BlendFactor_One,

    ///
    /// Blend forumla: R s0 k R G s0 k G B s0 k B A s0 k A
    BlendFactor_SourceColor,

    ///
    /// Blend forumla: 1 1 1 1 - R s0 k R G s0 k G B s0 k B A s0 k A
    BlendFactor_OneMinusSourceColor,

    ///
    /// Blend forumla: R d k R G d k G B d k B A d k A
    BlendFactor_DestColor,

    ///
    /// Blend forumla: 1 1 1 1 - R d k R G d k G B d k B A d k A
    BlendFactor_OneMinusDestColor,

    ///
    /// Blend forumla: A s0 k A A s0 k A A s0 k A A s0 k A
    BlendFactor_SourceAlpha,

    ///
    /// Blend forumla: 1 1 1 1 - A s0 k A A s0 k A A s0 k A A s0 k A
    BlendFactor_OneMinusSourceAlpha,

    ///
    /// Blend forumla: A d k A A d k A A d k A A d k A
    BlendFactor_DestAlpha,

    ///
    /// Blend forumla: 1 1 1 1 - A d k A A d k A A d k A A d k A
    BlendFactor_OneMinusDestAlpha
};

}