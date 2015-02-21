///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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
/// A semantic describing how a vertex attribute is used.
enum VertexAttributeSemantic
{

    ///
    /// The position of a vertex.
    VertexAttributeSemantic_Position,

    ///
    /// The normal of a vertex.
    VertexAttributeSemantic_Normal,

    ///
    /// The color of a vertex.
    VertexAttributeSemantic_Color,

    ///
    /// The tangent of a vertex.
    VertexAttributeSemantic_Tangent,

    ///
    /// The binormal of a vertex.
    VertexAttributeSemantic_Binormal,

    ///
    /// The first weight of a vertex.
    VertexAttributeSemantic_Weight0,

    ///
    /// The second weight of a vertex.
    VertexAttributeSemantic_Weight1,

    ///
    /// The third weight of a vertex.
    VertexAttributeSemantic_Weight2,

    ///
    /// The fourth weight of a vertex.
    VertexAttributeSemantic_Weight3,

    ///
    /// The texture coordinates of a vertex for the first texture.
    VertexAttributeSemantic_TextureCoords0,

    ///
    /// The texture coordinates of a vertex for the second texture.
    VertexAttributeSemantic_TextureCoords1,

    ///
    /// The texture coordinates of a vertex for the third texture.
    VertexAttributeSemantic_TextureCoords2,

    ///
    /// The texture coordinates of a vertex for the fourth texture.
    VertexAttributeSemantic_TextureCoords3
};

}