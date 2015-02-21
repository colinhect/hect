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

#include "Hect/Core/Export.h"
#include "Hect/Graphics/VertexAttributeSemantic.h"
#include "Hect/Graphics/VertexAttributeType.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"

namespace hect
{

///
/// An attribute of a vertex (position, normal, etc).
///
/// \note A vertex attribute is made up of a semantic, a type, and a
/// cardinality.  The semantic describes how the attribute is used.  The
/// type describes the type of each component in the attribute.  The
/// cardinality is number of components (of the attribute's type) the
/// attribute has.
class HECT_EXPORT VertexAttribute
{
    friend class VertexLayout;
public:

    ///
    /// Constructs a default vertex attribute.
    VertexAttribute();

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
    /// Returns the type.
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

    ///
    /// Returns whether the attribute is equivalent to another.
    ///
    /// \param vertexAttribute The other attribute.
    bool operator==(const VertexAttribute& vertexAttribute) const;

    ///
    /// Returns whether the attribute is different from another.
    ///
    /// \param vertexAttribute The other attribute.
    bool operator!=(const VertexAttribute& vertexAttribute) const;

    friend HECT_EXPORT Encoder& operator<<(Encoder& encoder, const VertexAttribute& vertexAttribute);
    friend HECT_EXPORT Decoder& operator>>(Decoder& decoder, VertexAttribute& vertexAttribute);

private:
    VertexAttributeSemantic _semantic { VertexAttributeSemantic_Position };
    VertexAttributeType _type { VertexAttributeType_Float32 };
    unsigned _cardinality { 3 };
    unsigned _offset { 0 };
};

}