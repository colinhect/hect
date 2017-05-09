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
#include "Mesh.h"

#include "Hect/Core/Exception.h"
#include "Hect/Graphics/MeshReader.h"
#include "Hect/Graphics/MeshWriter.h"
#include "Hect/Graphics/Renderer.h"

using namespace hect;

Mesh::Descriptor::Descriptor()
{
}

Mesh Mesh::createBox(Vector3 dimensions, const Descriptor& descriptor)
{
    Mesh mesh(descriptor);
    MeshWriter meshWriter(mesh);

    const double halfX = dimensions.x * 0.5;
    const double halfY = dimensions.y * 0.5;
    const double halfZ = dimensions.z * 0.5;

    // Top
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(-halfX, -halfY, halfZ));
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(halfX, -halfY, halfZ));
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(halfX, halfY, halfZ));
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(-halfX, halfY, halfZ));
    meshWriter.addIndex(0);
    meshWriter.addIndex(1);
    meshWriter.addIndex(2);
    meshWriter.addIndex(2);
    meshWriter.addIndex(3);
    meshWriter.addIndex(0);

    // Bottom
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(-halfX, -halfY, -halfZ));
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(halfX, -halfY, -halfZ));
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(halfX, halfY, -halfZ));
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(-halfX, halfY, -halfZ));
    meshWriter.addIndex(4);
    meshWriter.addIndex(5);
    meshWriter.addIndex(6);
    meshWriter.addIndex(6);
    meshWriter.addIndex(7);
    meshWriter.addIndex(4);

    // Left
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(-halfX, -halfY, -halfZ));
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(-halfX, -halfY, halfZ));
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(-halfX, halfY, halfZ));
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(-halfX, halfY, -halfZ));
    meshWriter.addIndex(8);
    meshWriter.addIndex(9);
    meshWriter.addIndex(10);
    meshWriter.addIndex(10);
    meshWriter.addIndex(11);
    meshWriter.addIndex(8);

    // Right
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(halfX, -halfY, -halfZ));
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(halfX, -halfY, halfZ));
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(halfX, halfY, halfZ));
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(halfX, halfY, -halfZ));
    meshWriter.addIndex(12);
    meshWriter.addIndex(13);
    meshWriter.addIndex(14);
    meshWriter.addIndex(14);
    meshWriter.addIndex(15);
    meshWriter.addIndex(12);

    // Back
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(-halfX, -halfY, -halfZ));
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(halfX, -halfY, -halfZ));
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(halfX, -halfY, halfZ));
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(-halfX, -halfY, halfZ));
    meshWriter.addIndex(15);
    meshWriter.addIndex(16);
    meshWriter.addIndex(17);
    meshWriter.addIndex(17);
    meshWriter.addIndex(18);
    meshWriter.addIndex(15);

    // Front
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(-halfX, halfY, -halfZ));
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(halfX, halfY, -halfZ));
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(halfX, halfY, halfZ));
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(-halfX, halfY, halfZ));
    meshWriter.addIndex(19);
    meshWriter.addIndex(20);
    meshWriter.addIndex(21);
    meshWriter.addIndex(21);
    meshWriter.addIndex(22);
    meshWriter.addIndex(19);

    return mesh;
}

Mesh::Mesh() :
    _vertexLayout(VertexLayout::createDefault())
{
}

Mesh::Mesh(Name name) :
    Asset(name),
    _vertexLayout(VertexLayout::createDefault())
{
}

Mesh::Mesh(const Descriptor& descriptor) :
    Asset(descriptor.name),
    _vertexLayout(descriptor.vertexLayout),
    _primitiveType(descriptor.primitiveType),
    _indexType(descriptor.indexType)
{
}

const VertexLayout& Mesh::vertexLayout() const
{
    return _vertexLayout;
}

void Mesh::setVertexLayout(const VertexLayout& vertexLayout)
{
    if (_vertexData.size() != 0)
    {
        throw InvalidOperation("Cannot change the vertex layout of a mesh with vertex data");
    }

    if (isUploaded())
    {
        renderer().destroyMesh(*this);
    }

    _vertexLayout = vertexLayout;
}

PrimitiveType Mesh::primitiveType() const
{
    return _primitiveType;
}

void Mesh::setPrimitiveType(PrimitiveType primitiveType)
{
    if (isUploaded())
    {
        renderer().destroyMesh(*this);
    }

    _primitiveType = primitiveType;
}

IndexType Mesh::indexType() const
{
    return _indexType;
}

void Mesh::setIndexType(IndexType indexType)
{
    if (_indexData.size() != 0)
    {
        throw InvalidOperation("Cannot change the index type of a mesh with index data");
    }

    if (isUploaded())
    {
        renderer().destroyMesh(*this);
    }

    _indexType = indexType;
}

const Mesh::VertexData& Mesh::vertexData() const
{
    return _vertexData;
}

void Mesh::setVertexData(const VertexData& vertexData)
{
    if (isUploaded())
    {
        renderer().destroyMesh(*this);
    }

    _vertexData = vertexData;
    _vertexCount = vertexData.size() / _vertexLayout.vertexSize();
}

void Mesh::clearVertexData()
{
    if (isUploaded())
    {
        renderer().destroyMesh(*this);
    }

    _vertexData.clear();
    _vertexCount = 0;
}

size_t Mesh::vertexCount() const
{
    return _vertexCount;
}

const Mesh::IndexData& Mesh::indexData() const
{
    return _indexData;
}

void Mesh::setIndexData(const IndexData& indexData)
{
    if (isUploaded())
    {
        renderer().destroyMesh(*this);
    }

    _indexData = indexData;
    _indexCount = indexData.size() / indexSize();
}

void Mesh::clearIndexData()
{
    if (isUploaded())
    {
        renderer().destroyMesh(*this);
    }

    _indexData.clear();
    _indexCount = 0;
}

size_t Mesh::indexCount() const
{
    return _indexCount;
}

unsigned Mesh::indexSize() const
{
    switch (_indexType)
    {
    case IndexType::UInt8:
        return 1;
    case IndexType::UInt16:
        return 2;
    case IndexType::UInt32:
        return 4;
    }
    return 0;
}

AxisAlignedBox& Mesh::axisAlignedBox()
{
    return _axisAlignedBox;
}

const AxisAlignedBox& Mesh::axisAlignedBox() const
{
    return _axisAlignedBox;
}

bool Mesh::operator==(const Mesh& mesh) const
{
    // Vertex layout
    if (_vertexLayout != mesh._vertexLayout)
    {
        return false;
    }

    // Primitive/index types
    if (_primitiveType != mesh._primitiveType || _indexType != mesh._indexType)
    {
        return false;
    }

    // Vertex/index counts
    if (_vertexCount != mesh._vertexCount || _indexCount != mesh._indexCount)
    {
        return false;
    }

    // Vertex data
    for (size_t i = 0; i < _vertexCount; ++i)
    {
        if (_vertexData[i] != mesh._vertexData[i])
        {
            return false;
        }
    }

    // Index data
    for (size_t i = 0; i < _indexCount; ++i)
    {
        if (_indexData[i] != mesh._indexData[i])
        {
            return false;
        }
    }

    return true;
}

bool Mesh::operator!=(const Mesh& mesh) const
{
    return !(*this == mesh);
}

void Mesh::encode(Encoder& encoder) const
{
    encoder << encodeValue("vertexLayout", _vertexLayout)
            << encodeEnum("indexType", _indexType)
            << encodeEnum("primitiveType", _primitiveType);

    if (encoder.isBinaryStream())
    {
        WriteStream& stream = encoder.binaryStream();

        // Vertex data
        uint32_t vertexDataSize = static_cast<uint32_t>(_vertexData.size());
        stream << vertexDataSize;
        if (vertexDataSize > 0)
        {
            stream.write(&_vertexData[0], vertexDataSize);
        }

        // Index data
        uint32_t indexDataSize = static_cast<uint32_t>(_indexData.size());
        stream << indexDataSize;
        if (indexDataSize > 0)
        {
            stream.write(&_indexData[0], indexDataSize);
        }
    }
    else
    {
        MeshReader reader(*this);

        // Vertex data
        encoder << beginArray("vertices");
        while (reader.nextVertex())
        {
            encoder << beginObject()
                    << beginArray("attributes");

            for (const VertexAttribute& attribute : _vertexLayout.attributes())
            {
                VertexAttributeSemantic semantic = attribute.semantic();

                encoder << beginObject()
                        << encodeEnum("semantic", semantic);

                unsigned cardinality = attribute.cardinality();
                if (cardinality == 1)
                {
                    double value = reader.readAttributeDouble(semantic);
                    encoder << encodeValue("data", value);
                }
                else if (cardinality == 2)
                {
                    Vector2 value = reader.readAttributeVector2(semantic);
                    encoder << encodeValue("data", value);
                }
                else if (cardinality == 3)
                {
                    Vector3 value = reader.readAttributeVector3(semantic);
                    encoder << encodeValue("data", value);
                }
                else if (cardinality == 4)
                {
                    Vector4 value = reader.readAttributeVector4(semantic);
                    encoder << encodeValue("data", value);
                }

                encoder << endObject();
            }

            encoder << endArray()
                    << endObject();
        }
        encoder << endArray();

        // Index data
        encoder << beginArray("indices");
        while (reader.nextIndex())
        {
            encoder << encodeValue(reader.readIndexUInt32());
        }
        encoder << endArray();
    }
}

void Mesh::decode(Decoder& decoder)
{
    // Clear any data the mesh already had
    *this = Mesh(name());

    decoder >> decodeValue("vertexLayout", _vertexLayout)
            >> decodeEnum("indexType", _indexType)
            >> decodeEnum("primitiveType", _primitiveType);

    // Vertex and index data
    if (decoder.isBinaryStream())
    {
        ReadStream& stream = decoder.binaryStream();

        // Vertex data
        uint32_t vertexDataSize;
        stream >> vertexDataSize;
        Mesh::VertexData vertexData(vertexDataSize);
        if (vertexDataSize > 0)
        {
            stream.read(&vertexData[0], vertexDataSize);
        }

        // Index data
        uint32_t indexDataSize;
        stream >> indexDataSize;
        Mesh::IndexData indexData(indexDataSize);
        if (indexDataSize > 0)
        {
            stream.read(&indexData[0], indexDataSize);
        }

        // Set vertex/index data
        setVertexData(vertexData);
        setIndexData(indexData);

        // Compute the bounding box based on the vertex positions
        MeshReader meshReader(*this);
        while (meshReader.nextVertex())
        {
            Vector3 position = meshReader.readAttributeVector3(VertexAttributeSemantic::Position);
            _axisAlignedBox.expandToInclude(position);
        }
    }
    else
    {
        const VertexLayout& vertexLayout = _vertexLayout;

        // Use a mesh writer to write vertex and index data
        MeshWriter meshWriter(*this);

        // Vertex data
        if (decoder.selectMember("vertices"))
        {
            decoder >> beginArray();
            while (decoder.hasMoreElements())
            {
                meshWriter.addVertex();
                decoder >> beginObject();

                if (!decoder.selectMember("attributes"))
                {
                    decoder >> endObject();
                    continue;
                }

                // For each attribute
                decoder >> beginArray();
                while (decoder.hasMoreElements())
                {
                    decoder.beginObject();

                    VertexAttributeSemantic semantic;
                    decoder >> decodeEnum("semantic", semantic, true);

                    if (vertexLayout.hasAttributeWithSemantic(semantic))
                    {
                        const VertexAttribute& attribute = vertexLayout.attributeWithSemantic(semantic);
                        unsigned cardinality = attribute.cardinality();

                        if (cardinality == 1)
                        {
                            double value;
                            decoder >> decodeValue("data", value, true);
                            meshWriter.writeAttributeData(semantic, value);
                        }
                        else if (cardinality == 2)
                        {
                            Vector2 value;
                            decoder >> decodeValue("data", value, true);
                            meshWriter.writeAttributeData(semantic, value);
                        }
                        else if (cardinality == 3)
                        {
                            Vector3 value;
                            decoder >> decodeValue("data", value, true);
                            meshWriter.writeAttributeData(semantic, value);
                        }
                        else if (cardinality == 4)
                        {
                            Vector4 value;
                            decoder >> decodeValue("data", value, true);
                            meshWriter.writeAttributeData(semantic, value);
                        }
                    }
                    decoder >> endObject();
                }
                decoder >> endArray();

                decoder >> endObject();
            }
            decoder >> endArray();
        }

        // Index data
        if (decoder.selectMember("indices"))
        {
            decoder >> beginArray();
            while (decoder.hasMoreElements())
            {
                meshWriter.addIndex(decoder.decodeUInt32());
            }
            decoder >> endArray();
        }
    }
}
