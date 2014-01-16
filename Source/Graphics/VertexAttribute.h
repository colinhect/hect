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
/// A semantic describing how a vertex attribute is used.
enum class VertexAttributeSemantic
{

    ///
    /// The position of a vertex.
    Position,

    ///
    /// The normal of a vertex.
    Normal,

    ///
    /// The color of a vertex.
    Color,

    ///
    /// The tangent of a vertex.
    Tangent,

    ///
    /// The binormal of a vertex.
    Binormal,

    ///
    /// The first weight of a vertex.
    Weight0,

    ///
    /// The second weight of a vertex.
    Weight1,

    ///
    /// The third weight of a vertex.
    Weight2,

    ///
    /// The fourth weight of a vertex.
    Weight3,

    ///
    /// The texture coordinates of a vertex for the first texture.
    TextureCoords0,

    ///
    /// The texture coordinates of a vertex for the second texture.
    TextureCoords1,

    ///
    /// The texture coordinates of a vertex for the third texture.
    TextureCoords2,

    ///
    /// The texture coordinates of a vertex for the fourth texture.
    TextureCoords3
};

///
/// A type of a vertex attribute.
enum class VertexAttributeType
{
    ///
    /// An 8-bit signed integer.
    Byte,
    
    ///
    /// An 8-bit unsigned integer.
    UnsignedByte,

    ///
    /// A 16-bit signed integer.
    Short,

    ///
    /// A 16-bit unsigned integer.
    UnsignedShort,

    ///
    /// A 32-bit signed integer.
    Int,

    ///
    /// A 32-bit unsigned integer.
    UnsignedInt,

    ///
    /// A 16-bit float.
    /// \remarks Also known as "half".
    Half,

    ///
    /// A 32-bit float.
    Float
};

///
/// An attribute of a vertex (position, normal, etc).
///
/// \remarks A vertex attribute is made up of a semantic, a type, and a
/// cardinality.  The semantic describes how the attribute is used.  The
/// type describes the type of each component in the attribute.  The
/// cardinality is number of components (of the attributes type) the
/// attribute has.
class VertexAttribute
{
    friend class VertexLayout;
public:

    ///
    /// An array of vertex attributes.
    typedef std::vector<VertexAttribute> Array;

    ///
    /// Constructs a vertex attribute.
    ///
    /// \param semantic The semantic.
    /// \param type The type.
    /// \param cardinality The cardinality.
    VertexAttribute(VertexAttributeSemantic semantic, VertexAttributeType type, unsigned cardinality);

    ///
    /// Returns the semantic.
    VertexAttributeSemantic semantic() const;

    ///
    /// Returns the type of the vertex attribute.
    VertexAttributeType type() const;

    ///
    /// Returns the cardinality.
    unsigned cardinality() const;

    ///
    /// Returns the total size in bytes.
    unsigned size() const;

    ///
    /// Returns the offset into the vertex data in bytes.
    unsigned offset() const;

private:
    VertexAttributeSemantic _semantic;
    VertexAttributeType _type;
    unsigned _cardinality;
    unsigned _offset;
};

}