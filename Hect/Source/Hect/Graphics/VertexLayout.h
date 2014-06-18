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

#include "Hect/Core/Export.h"
#include "Hect/Graphics/VertexAttribute.h"
#include "Hect/IO/Encodable.h"

namespace hect
{

///
/// An ordered layout of vertex attributes.
class VertexLayout :
    public Encodable
{
public:

    ///
    /// Creates the default vertex layout.
    static VertexLayout createDefault();

    ///
    /// Constructs an empty vertex layout.
    VertexLayout();

    ///
    /// Appends an attribute to the end of the layout.
    ///
    /// \param attribute The new attribute to add.
    void addAttribute(const VertexAttribute& attribute);

    ///
    /// Clears all attributes from the layout.
    void clearAttributes();

    ///
    /// Returns whether the vertex layout has an attribute with the given semantic.
    ///
    /// \param semantic The semantic.
    bool hasAttributeWithSemantic(VertexAttributeSemantic::Enum semantic) const;

    ///
    /// Returns the attribute with the given semantic.
    ///
    /// \param semantic The semantic to find the attribute for.
    ///
    /// \throws Error If the vertex layout does not have an attribute with the
    /// given semantic
    const VertexAttribute& attributeWithSemantic(VertexAttributeSemantic::Enum semantic) const;

    /// Returns the attributes.
    const VertexAttribute::Array& attributes() const;

    ///
    /// Returns the total size in bytes of a vertex in this layout.
    unsigned vertexSize() const;

    ///
    /// Encodes the layout.
    ///
    /// \param encoder The encoder to use.
    void encode(ObjectEncoder& encoder) const;

    ///
    /// Decodes the layout.
    ///
    /// \note Any pre-existing attributes the layout has will be removed before
    /// decoding begins.
    ///
    /// \param decoder The decoder to use.
    /// \param assetCache The asset cache to get referenced assets from.
    void decode(ObjectDecoder& decoder, AssetCache& assetCache);

    ///
    /// Returns whether the layout is equivalent to another.
    ///
    /// \param vertexLayout The other layout.
    bool operator==(const VertexLayout& vertexLayout) const;

    ///
    /// Returns whether the layout is different from another.
    ///
    /// \param vertexLayout The other layout.
    bool operator!=(const VertexLayout& vertexLayout) const;

private:
    void _computeAttributeOffsets();

    VertexAttribute::Array _attributes;
    unsigned _vertexSize;
};

}