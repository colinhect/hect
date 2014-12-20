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
/// A factor applied to either the source or destination pixel when blending.
enum BlendFactor
{
    ///
    /// (0, 0, 0, 0)
    BlendFactor_Zero,

    ///
    /// (1, 1, 1, 1)
    BlendFactor_One,

    ///
    /// (s.r, s.g, s.b, s.a)
    BlendFactor_SourceColor,

    ///
    /// (1, 1, 1, 1) - (s.r, s.g, s.b, s.a)
    BlendFactor_OneMinusSourceColor,

    ///
    /// (d.r, d.g, d.b, d.a)
    BlendFactor_DestinationColor,

    ///
    /// (1, 1, 1, 1) - (d.r, d.g, d.b, d.a)
    BlendFactor_OneMinusDestinationColor,

    ///
    /// (s.a, s.a, s.a, s.a)
    BlendFactor_SourceAlpha,

    ///
    /// (1, 1, 1, 1) - (s.a, s.a, s.a, s.a)
    BlendFactor_OneMinusSourceAlpha,

    ///
    /// (d.a, d.a, d.a, d.a)
    BlendFactor_DestinationAlpha,

    ///
    /// (1, 1, 1, 1) - (d.a, d.a, d.a, d.a)
    BlendFactor_OneMinusDestinationAlpha
};

}