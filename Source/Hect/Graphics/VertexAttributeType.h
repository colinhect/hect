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

#include <cstdint>

namespace hect
{

///
/// A type of a vertex attribute.
enum VertexAttributeType : uint8_t
{
    ///
    /// An 8-bit signed integer.
    VertexAttributeType_Byte,

    ///
    /// An 8-bit unsigned integer.
    VertexAttributeType_UnsignedByte,

    ///
    /// A 16-bit signed integer.
    VertexAttributeType_Short,

    ///
    /// A 16-bit unsigned integer.
    VertexAttributeType_UnsignedShort,

    ///
    /// A 32-bit signed integer.
    VertexAttributeType_Int,

    ///
    /// A 32-bit unsigned integer.
    VertexAttributeType_UnsignedInt,

    ///
    /// A 16-bit float.
    /// \note Also known as "half".
    VertexAttributeType_Half,

    ///
    /// A 32-bit float.
    VertexAttributeType_Float
};

}