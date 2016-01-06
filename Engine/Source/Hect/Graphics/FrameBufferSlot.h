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
#pragma once

namespace hect
{

///
/// A slot for a frame buffer attachment.
enum class FrameBufferSlot
{
    ///
    /// The 1st color component.
    Color0,

    ///
    /// The 2nd color component.
    Color1,

    ///
    /// The 3rd color component.
    Color2,

    ///
    /// The 4th color component.
    Color3,

    ///
    /// The 5th color component.
    Color4,

    ///
    /// The 6th color component.
    Color5,

    ///
    /// The 7th color component.
    Color6,

    ///
    /// The 8th color component.
    Color7,

    ///
    /// The 9th color component.
    Color8,

    ///
    /// The 10th color component.
    Color9,

    ///
    /// The 11th color component.
    Color10,

    ///
    /// The 12th color component.
    Color11,

    ///
    /// The 13th color component.
    Color12,

    ///
    /// The 14th color component.
    Color13,

    ///
    /// The 15th color component.
    Color14,

    ///
    /// The 16th color component.
    Color15,

    ///
    /// The depth component.
    Depth
};

}
