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

#include "Hect/Graphics/RendererObject.h"
#include "Hect/Graphics/VertexLayout.h"
#include "Hect/IO/Encodable.h"
#include "Hect/Math/AxisAlignedBox.h"

namespace hect
{

///
/// How the primitives of a mesh are rendered.
enum class PrimitiveType : uint8_t
{
    Triangles,
    TriangleStrip,
    Lines,
    LineStrip,
    Points
};

///
/// The type of each index in an index sequence.
enum class IndexType : uint8_t
{
    ///
    /// An 8-bit unsigned integer.
    UnsignedByte,

    ///
    /// A 16-bit unsigned integer.
    UnsignedShort,

    ///
    /// A 32-bit unsigned integer.
    UnsignedInt
};

///
/// A mesh of vertices and indices.
class Mesh :
    public RendererObject,
    public Encodable
{
    friend class MeshWriter;
public:

    ///
    /// Raw vertex data.
    typedef std::vector<uint8_t> VertexData;

    ///
    /// Raw index data.
    typedef std::vector<uint8_t> IndexData;

    ///
    /// Constructs a mesh with the default vertex layout, primitive type,
    /// and index type.
    Mesh();

    ///
    /// Constructs a mesh with the default vertex layout, primitive type,
    /// and index type and a name.
    ///
    /// \param name The name of the mesh.
    Mesh(const std::string& name);

    ///
    /// Constructs a mesh as a copy of another.
    ///
    /// \param mesh The mesh to copy.
    Mesh(const Mesh& mesh);

    ///
    /// Constructs a mesh moved from another.
    ///
    /// \param mesh The mesh to move.
    Mesh(Mesh&& mesh);

    ///
    /// Destroys the shader if it is uploaded.
    ~Mesh();

    ///
    /// Returns the name.
    const std::string& name() const;

    ///
    /// Sets the name.
    ///
    /// \param name The new name.
    void setName(const std::string& name);

    ///
    /// Returns the vertex layout.
    const VertexLayout& vertexLayout() const;

    ///
    /// Sets the vertex layout.
    ///
    /// \param vertexLayout The new vertex layout.
    ///
    /// \note If the mesh is uploaded to a renderer then it will be destroyed
    /// before the primitive type is set.
    ///
    /// \throws Error If the mesh already has vertex data.
    void setVertexLayout(const VertexLayout& vertexLayout);
    
    ///
    /// Returns the primitive type.
    PrimitiveType primitiveType() const;

    ///
    /// Sets the primitive type.
    ///
    /// \param primitiveType The new primitive type.
    ///
    /// \note If the mesh is uploaded to a renderer then it will be destroyed
    /// before the primitive type is set.
    void setPrimitiveType(PrimitiveType primitiveType);

    ///
    /// Returns the index type.
    IndexType indexType() const;

    ///
    /// Sets the index type.
    ///
    /// \param indexType The new index type.
    ///
    /// \note If the mesh is uploaded to a renderer then it will be destroyed
    /// before the index type is set.
    ///
    /// \throws Error If the mesh already has index data.
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
    /// \note If the mesh is uploaded to a renderer then it will be destroyed
    /// before the vertex data is set.
    void setVertexData(const VertexData& vertexData);

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
    /// \note If the mesh is uploaded to a renderer then it will be destroyed
    /// before the index data is set.
    void setIndexData(const IndexData& indexData);

    ///
    /// Returns the number of indices.
    size_t indexCount() const;

    ///
    /// Returns the size of an index in bytes.
    unsigned indexSize() const;

    ///
    /// Returns the bounding box.
    AxisAlignedBox& boundingBox();

    ///
    /// Returns the bounding box.
    const AxisAlignedBox& boundingBox() const;

    ///
    /// Encodes the mesh.
    ///
    /// \param encoder The encoder to use.
    void encode(ObjectEncoder& encoder) const;

    ///
    /// Decodes the mesh.
    ///
    /// \param decoder The decoder to use.
    /// \param assetCache The asset cache to get referenced assets from.
    void decode(ObjectDecoder& decoder, AssetCache& assetCache);

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

    ///
    /// Replaces the mesh with a copy of another.
    ///
    /// \note If the mesh was uploaded to a renderer then it will be destroyed
    /// before assigned.
    ///
    /// \param mesh The mesh to copy.
    ///
    /// \returns A reference to the mesh.
    Mesh& operator=(const Mesh& mesh);

    ///
    /// Replaces the mesh by moving another.
    ///
    /// \note If the mesh was uploaded to a renderer then it will be destroyed
    /// before assigned.
    ///
    /// \param mesh The mesh to move.
    ///
    /// \returns A reference to the mesh.
    Mesh& operator=(Mesh&& mesh);

private:
    static IndexType _parseIndexType(const std::string& value);
    static PrimitiveType _parsePrimitiveType(const std::string& value);

    std::string _name;

    VertexLayout _vertexLayout;
    PrimitiveType _primitiveType;
    IndexType _indexType;

    VertexData _vertexData;
    size_t _vertexCount;

    IndexData _indexData;
    size_t _indexCount;

    AxisAlignedBox _boundingBox;
};

}