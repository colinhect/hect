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

#include "Hect/Core/Export.h"
#include "Hect/Core/Sequence.h"
#include "Hect/Graphics/VertexAttribute.h"
#include "Hect/IO/Encodable.h"

namespace hect
{

///
/// An ordered layout of vertex attributes.
class HECT_EXPORT VertexLayout :
    public Encodable
{
    typedef std::vector<VertexAttribute> AttributeContainer;
public:

    ///
    /// A sequence of attributes.
    typedef Sequence<VertexAttribute, AttributeContainer> AttributeSequence;

    ///
    /// Creates the default vertex layout.
    static VertexLayout create_default();

    ///
    /// Appends an attribute to the end of the layout.
    ///
    /// \param attribute The new attribute to add.
    void add_attribute(const VertexAttribute& attribute);

    ///
    /// Clears all attributes from the layout.
    void clear_attributes();

    ///
    /// Returns whether the vertex layout has an attribute with the given
    /// semantic.
    ///
    /// \param semantic The semantic.
    bool has_attribute_with_semantic(VertexAttributeSemantic semantic) const;

    ///
    /// Returns the attribute with the given semantic.
    ///
    /// \param semantic The semantic to find the attribute for.
    ///
    /// \throws InvalidOperation If the vertex layout does not have an
    /// attribute with the given semantic.
    const VertexAttribute& attribute_with_semantic(VertexAttributeSemantic semantic) const;

    ///
    /// Returns the attributes.
    const AttributeSequence attributes() const;

    ///
    /// Returns the number of attributes.
    size_t attribute_count() const;

    ///
    /// Returns the total size in bytes of a vertex in this layout.
    unsigned vertex_size() const;

    ///
    /// Returns whether the layout is equivalent to another.
    ///
    /// \param vertex_layout The other layout.
    bool operator==(const VertexLayout& vertex_layout) const;

    ///
    /// Returns whether the layout is different from another.
    ///
    /// \param vertex_layout The other layout.
    bool operator!=(const VertexLayout& vertex_layout) const;

    void encode(Encoder& encoder) const override;
    void decode(Decoder& decoder) override;

private:
    void compute_attribute_offsets();

    AttributeContainer _attributes;
    unsigned _vertex_size { 0 };
};

}