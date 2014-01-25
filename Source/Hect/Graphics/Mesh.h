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
    UnsignedByte,
    UnsignedShort,
    UnsignedInt
};

///
/// A mesh of vertices and indices.
class Mesh :
    public RendererObject,
    public Encodable
{
    friend class MeshWriter;
    friend class MeshEncoder;
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
    /// Constructs a mesh.
    ///
    /// \param name The name of the mesh.
    /// \param vertexLayout The vertex layout.
    /// \param primitiveType The primitive type.
    /// \param indexType The index type.
    Mesh(const std::string& name, const VertexLayout& vertexLayout, PrimitiveType primitiveType, IndexType indexType);

    ///
    /// Constructs a mesh as a copy from another mesh.
    ///
    /// \param mesh The mesh to copy from.
    Mesh(const Mesh& mesh);

    ///
    /// Returns the name.
    const std::string& name() const;

    ///
    /// Sets the raw vertex data.
    ///
    /// \param vertexData The vertex data to set.  Assumed to conform to the
    /// vertex layout.
    ///
    /// \throws Error If the mesh has vertex data.
    void setVertexData(const VertexData& vertexData);

    ///
    /// Returns the raw vertex data.
    const VertexData& vertexData() const;

    ///
    /// Returns the number of vertices.
    size_t vertexCount() const;

    ///
    /// Sets the raw index data.
    ///
    /// \param indexData The index data to set.  Assumed to conform to the
    /// index type.
    ///
    /// \throws Error If the mesh has vertex data.
    void setIndexData(const IndexData& indexData);

    ///
    /// Returns the raw index data.
    const IndexData& indexData() const;

    ///
    /// Returns the number of indices.
    size_t indexCount() const;

    ///
    /// Returns the size of an index in bytes.
    unsigned indexSize() const;

    ///
    /// Returns the vertex layout.
    const VertexLayout& vertexLayout() const;

    ///
    /// Returns the primitive type.
    PrimitiveType primitiveType() const;

    ///
    /// Returns the index type.
    IndexType indexType() const;

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
    /// Clears all mesh data
    ///
    /// \note If the mesh was uploaded to a renderer then it will be destroyed.
    void clear();

    ///
    /// Decodes the mesh.
    ///
    /// \note All vertex/index and layout data is cleared before
    /// decoding begins.  If the mesh was uploaded to a renderer
    /// then it will be destroyed before decoding begins.
    ///
    /// \param decoder The decoder to use.
    /// \param assetCache The asset cache to get referenced assets from.
    void decode(ObjectDecoder& decoder, AssetCache& assetCache);

    ///
    /// Returns whether the mesh is equivalent to another.
    ///
    /// \remarks Does not compare the name.
    ///
    /// \param mesh The other mesh.
    bool operator==(const Mesh& mesh) const;

    ///
    /// Returns whether the mesh is different from another.
    ///
    /// \remarks Does not compare the name.
    ///
    /// \param mesh The other mesh.
    bool operator!=(const Mesh& mesh) const;

private:
    static IndexType _parseIndexType(const std::string& value);
    static PrimitiveType _parsePrimitiveType(const std::string& value);

    std::string _name;

    VertexLayout _vertexLayout;
    PrimitiveType _primitiveType;
    IndexType _indexType;

    VertexData _vertexData;
    IndexData _indexData;

    size_t _vertexCount;
    size_t _indexCount;

    AxisAlignedBox _boundingBox;
};

}