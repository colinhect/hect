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
    class HECT_EXPORT Descriptor
    {
    public:

        ///
        /// Constructs a default mesh descriptor.
        Descriptor();

        ///
        /// The name of the mesh.
        Name name;

        ///
        /// The vertex layout.
        VertexLayout vertex_layout { VertexLayout::create_default() };

        ///
        /// The primitive type.
        PrimitiveType primitive_type { PrimitiveType::Triangles };

        ///
        /// The index type.
        IndexType index_type { IndexType::UInt16 };
    };

    ///
    /// Creates a box mesh with the specified dimensions.
    ///
    /// \param dimensions The dimensions of the box.
    /// \param descriptor The descriptor of the mesh to construct.
    ///
    /// \returns The box mesh.
    static Mesh create_box(Vector3 dimensions, const Descriptor& descriptor = Descriptor());

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
    const VertexLayout& vertex_layout() const;

    ///
    /// Sets the vertex layout.
    ///
    /// \param vertex_layout The new vertex layout.
    ///
    /// \note If the mesh is uploaded to a renderer then it will be
    /// destroyed before the primitive type is set.
    ///
    /// \throws InvalidOperation If the mesh already has vertex data.
    void set_vertex_layout(const VertexLayout& vertex_layout);

    ///
    /// Returns the primitive type.
    PrimitiveType primitive_type() const;

    ///
    /// Sets the primitive type.
    ///
    /// \param primitive_type The new primitive type.
    ///
    /// \note If the mesh is uploaded to a renderer then it will be
    /// destroyed before the primitive type is set.
    void set_primitive_type(PrimitiveType primitive_type);

    ///
    /// Returns the index type.
    IndexType index_type() const;

    ///
    /// Sets the index type.
    ///
    /// \param index_type The new index type.
    ///
    /// \note If the mesh is uploaded to a renderer then it will be
    /// destroyed before the index type is set.
    ///
    /// \throws InvalidOperation If the mesh already has index data.
    void set_index_type(IndexType index_type);

    ///
    /// Returns the raw vertex data.
    const VertexData& vertex_data() const;

    ///
    /// Sets the raw vertex data.
    ///
    /// \param vertex_data The vertex data to set.  Assumed to conform to the
    /// vertex layout.
    ///
    /// \note If the mesh is uploaded to a renderer then it will be
    /// destroyed before the vertex data is set.
    void set_vertex_data(const VertexData& vertex_data);

    ///
    /// Clears all vertex data.
    void clear_vertex_data();

    ///
    /// Returns the number of vertices.
    size_t vertex_count() const;

    ///
    /// Returns the raw index data.
    const IndexData& index_data() const;

    ///
    /// Sets the raw index data.
    ///
    /// \param index_data The index data to set.  Assumed to conform to the
    /// index type.
    ///
    /// \note If the mesh is uploaded to a renderer then it will be
    /// destroyed before the index data is set.
    void set_index_data(const IndexData& index_data);

    ///
    /// Clears all index data.
    void clear_index_data();

    ///
    /// Returns the number of indices.
    size_t index_count() const;

    ///
    /// Returns the size of an index in bytes.
    unsigned index_size() const;

    ///
    /// Returns an axis aligned box bounding the mesh.
    AxisAlignedBox& axis_aligned_box();

    ///
    /// Returns an axis aligned box bounding the mesh.
    const AxisAlignedBox& axis_aligned_box() const;

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
    VertexLayout _vertex_layout;
    PrimitiveType _primitive_type { PrimitiveType::Triangles };
    IndexType _index_type { IndexType::UInt16 };

    VertexData _vertex_data;
    size_t _vertex_count { 0 };

    IndexData _index_data;
    size_t _index_count { 0 };

    AxisAlignedBox _axis_aligned_box;
};

}
