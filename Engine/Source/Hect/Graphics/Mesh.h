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
#include "Hect/Graphics/IndexType.h"
#include "Hect/Graphics/PrimitiveType.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/Graphics/VertexLayout.h"
#include "Hect/IO/Asset.h"
#include "Hect/IO/ByteVector.h"
#include "Hect/Math/AxisAlignedBox.h"

namespace hect
{

///
/// A mesh of vertices and indices.
class HECT_EXPORT Mesh :
    public Asset<Mesh>,
    public Renderer::Object<Mesh>
{
    friend class MeshWriter;
public:

    ///
    /// Raw vertex data.
    typedef ByteVector VertexData;

    ///
    /// Raw index data.
    typedef ByteVector IndexData;

    ///
    /// Describes how to construct a mesh.
    struct Descriptor
    {
        ///
        /// The name of the mesh.
        Name name;

        ///
        /// The vertex layout.
        VertexLayout vertexLayout { VertexLayout::createDefault() };

        ///
        /// The primitive type.
        PrimitiveType primitiveType { PrimitiveType::Triangles };

        ///
        /// The index type.
        IndexType indexType { IndexType::UInt16 };
    };

    ///
    /// Creates a box mesh with the specified dimensions.
    ///
    /// \param dimensions The dimensions of the box.
    /// \param descriptor The descriptor of the mesh to construct.
    ///
    /// \returns The box mesh.
    static Mesh createBox(Vector3 dimensions, const Descriptor& descriptor = Descriptor::Descriptor());

    ///
    /// Constructs a mesh with the default vertex layout, primitive type,
    /// and index type.
    Mesh();

    ///
    /// Constructs a mesh with the default vertex layout, primitive type,
    /// and index type and a name.
    ///
    /// \param name The name of the mesh.
    Mesh(Name name);

    ///
    /// Constructs a mesh from a descriptor.
    ///
    /// \param descriptor The descriptor of the mesh to construct.
    Mesh(const Descriptor& descriptor);

    ///
    /// Returns the vertex layout.
    const VertexLayout& vertexLayout() const;

    ///
    /// Sets the vertex layout.
    ///
    /// \param vertexLayout The new vertex layout.
    ///
    /// \note If the mesh is uploaded to a renderer then it will be
    /// destroyed before the primitive type is set.
    ///
    /// \throws InvalidOperation If the mesh already has vertex data.
    void setVertexLayout(const VertexLayout& vertexLayout);

    ///
    /// Returns the primitive type.
    PrimitiveType primitiveType() const;

    ///
    /// Sets the primitive type.
    ///
    /// \param primitiveType The new primitive type.
    ///
    /// \note If the mesh is uploaded to a renderer then it will be
    /// destroyed before the primitive type is set.
    void setPrimitiveType(PrimitiveType primitiveType);

    ///
    /// Returns the index type.
    IndexType indexType() const;

    ///
    /// Sets the index type.
    ///
    /// \param indexType The new index type.
    ///
    /// \note If the mesh is uploaded to a renderer then it will be
    /// destroyed before the index type is set.
    ///
    /// \throws InvalidOperation If the mesh already has index data.
    void setIndexType(IndexType indexType);

    ///
    /// Returns the raw vertex data.
    const VertexData& vertexData() const;

    ///
    /// Sets the raw vertex data.
    ///
    /// \param vertexData The vertex data to set.  Assumed to conform to the
    /// vertex layout.
    ///
    /// \note If the mesh is uploaded to a renderer then it will be
    /// destroyed before the vertex data is set.
    void setVertexData(const VertexData& vertexData);

    ///
    /// Clears all vertex data.
    void clearVertexData();

    ///
    /// Returns the number of vertices.
    size_t vertexCount() const;

    ///
    /// Returns the raw index data.
    const IndexData& indexData() const;

    ///
    /// Sets the raw index data.
    ///
    /// \param indexData The index data to set.  Assumed to conform to the
    /// index type.
    ///
    /// \note If the mesh is uploaded to a renderer then it will be
    /// destroyed before the index data is set.
    void setIndexData(const IndexData& indexData);

    ///
    /// Clears all index data.
    void clearIndexData();

    ///
    /// Returns the number of indices.
    size_t indexCount() const;

    ///
    /// Returns the size of an index in bytes.
    unsigned indexSize() const;

    ///
    /// Returns an axis aligned box bounding the mesh.
    AxisAlignedBox& axisAlignedBox();

    ///
    /// Returns an axis aligned box bounding the mesh.
    const AxisAlignedBox& axisAlignedBox() const;

    ///
    /// Returns whether the mesh is equivalent to another.
    ///
    /// \note Does not compare the name.
    ///
    /// \param mesh The other mesh.
    bool operator==(const Mesh& mesh) const;

    ///
    /// Returns whether the mesh is different from another.
    ///
    /// \note Does not compare the name.
    ///
    /// \param mesh The other mesh.
    bool operator!=(const Mesh& mesh) const;

    void encode(Encoder& encoder) const override;
    void decode(Decoder& decoder) override;

private:
    VertexLayout _vertexLayout;
    PrimitiveType _primitiveType { PrimitiveType::Triangles };
    IndexType _indexType { IndexType::UInt16 };

    VertexData _vertexData;
    size_t _vertexCount { 0 };

    IndexData _indexData;
    size_t _indexCount { 0 };

    AxisAlignedBox _axisAlignedBox;
};

}
