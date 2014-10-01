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
#include "Mesh.h"

#include "Hect/Core/Error.h"
#include "Hect/Graphics/MeshReader.h"
#include "Hect/Graphics/MeshWriter.h"
#include "Hect/Graphics/Renderer.h"

using namespace hect;

const uint64_t Mesh::IdentifyNumber = 0x84629573;

Mesh::Mesh() :
    _vertexLayout(VertexLayout::createDefault()),
    _primitiveType(PrimitiveType_Triangles),
    _indexType(IndexType_UInt16),
    _vertexCount(0),
    _indexCount(0)
{
}

Mesh::Mesh(const std::string& name) :
    _name(name),
    _vertexLayout(VertexLayout::createDefault()),
    _primitiveType(PrimitiveType_Triangles),
    _indexType(IndexType_UInt16),
    _vertexCount(0),
    _indexCount(0)
{
}

Mesh::Mesh(const Mesh& mesh) :
    RendererObject(mesh),
    _name(mesh._name),
    _vertexLayout(mesh._vertexLayout),
    _primitiveType(mesh._primitiveType),
    _indexType(mesh._indexType),
    _vertexData(mesh._vertexData),
    _vertexCount(mesh._vertexCount),
    _indexData(mesh._indexData),
    _indexCount(mesh._indexCount),
    _axisAlignedBox(mesh._axisAlignedBox)
{
}

Mesh::Mesh(Mesh&& mesh) :
    RendererObject(mesh),
    _name(std::move(mesh._name)),
    _vertexLayout(std::move(mesh._vertexLayout)),
    _primitiveType(mesh._primitiveType),
    _indexType(mesh._indexType),
    _vertexData(std::move(mesh._vertexData)),
    _vertexCount(mesh._vertexCount),
    _indexData(std::move(mesh._indexData)),
    _indexCount(mesh._indexCount),
    _axisAlignedBox(mesh._axisAlignedBox)
{
}

Mesh::~Mesh()
{
    if (isUploaded())
    {
        renderer().destroyMesh(*this);
    }
}

const std::string& Mesh::name() const
{
    return _name;
}

void Mesh::setName(const std::string& name)
{
    _name = name;
}

const VertexLayout& Mesh::vertexLayout() const
{
    return _vertexLayout;
}

void Mesh::setVertexLayout(const VertexLayout& vertexLayout)
{
    if (isUploaded())
    {
        renderer().destroyMesh(*this);
    }

    if (_vertexData.size() != 0)
    {
        throw Error("Cannot change the vertex layout of a mesh with vertex data");
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
    if (isUploaded())
    {
        renderer().destroyMesh(*this);
    }

    if (_indexData.size() != 0)
    {
        throw Error("Cannot change the index type of a mesh with index data");
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

size_t Mesh::indexCount() const
{
    return _indexCount;
}

unsigned Mesh::indexSize() const
{
    switch (_indexType)
    {
    case IndexType_UInt8:
        return 1;
    case IndexType_UInt16:
        return 2;
    case IndexType_UInt32:
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
    // Compare vertex layout
    if (_vertexLayout != mesh._vertexLayout)
    {
        return false;
    }

    // Compare primitive/index types
    if (_primitiveType != mesh._primitiveType || _indexType != mesh._indexType)
    {
        return false;
    }

    // Compare vertex/index counts
    if (_vertexCount != mesh._vertexCount || _indexCount != mesh._indexCount)
    {
        return false;
    }

    // Compare vertex data
    for (size_t i = 0; i < _vertexCount; ++i)
    {
        if (_vertexData[i] != mesh._vertexData[i])
        {
            return false;
        }
    }

    // Compare index data
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

Mesh& Mesh::operator=(const Mesh& mesh)
{
    if (isUploaded())
    {
        renderer().destroyMesh(*this);
    }

    RendererObject::operator=(mesh);

    _name = mesh.name();
    _vertexLayout = mesh.vertexLayout();
    _primitiveType = mesh.primitiveType();
    _indexType = mesh.indexType();
    _vertexData = mesh.vertexData();
    _vertexCount = mesh.vertexCount();
    _indexData = mesh.indexData();
    _indexCount = mesh.indexCount();
    _axisAlignedBox = mesh.axisAlignedBox();

    return *this;
}

Mesh& Mesh::operator=(Mesh&& mesh)
{
    if (isUploaded())
    {
        renderer().destroyMesh(*this);
    }

    RendererObject::operator=(mesh);

    _name = std::move(mesh.name());
    _vertexLayout = std::move(mesh.vertexLayout());
    _primitiveType = mesh.primitiveType();
    _indexType = mesh.indexType();
    _vertexData = std::move(mesh.vertexData());
    _vertexCount = mesh.vertexCount();
    _indexData = std::move(mesh.indexData());
    _indexCount = mesh.indexCount();
    _axisAlignedBox = mesh.axisAlignedBox();

    return *this;
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const Mesh& mesh)
{
    encoder << beginObject();

    if (encoder.isBinaryStream())
    {
        encoder << encodeValue("identifyNumber", Mesh::IdentifyNumber);
    }

    // Vertex layout
    {
        encoder << beginObject("vertexLayout")
                << encodeValue(mesh.vertexLayout())
                << endObject();
    }

    // Index type
    encoder << encodeEnum("indexType", mesh.indexType());

    // Primitive type
    encoder << encodeEnum("primitiveType", mesh.primitiveType());

    if (encoder.isBinaryStream())
    {
        WriteStream& stream = encoder.binaryStream();

        // Vertex data
        uint32_t vertexDataSize = (uint32_t)mesh.vertexData().size();
        stream << vertexDataSize;
        if (vertexDataSize > 0)
        {
            stream.write(&mesh.vertexData()[0], vertexDataSize);
        }

        // Index data
        uint32_t indexDataSize = (uint32_t)mesh.indexData().size();
        stream << indexDataSize;
        if (indexDataSize > 0)
        {
            stream.write(&mesh.indexData()[0], indexDataSize);
        }
    }
    else
    {
        MeshReader reader(mesh);

        // Vertex data
        {
            encoder << beginArray("vertices");
            while (reader.nextVertex())
            {
                encoder << beginArray();
                for (const VertexAttribute& attribute : mesh.vertexLayout().attributes())
                {
                    encoder << beginArray();
                    VertexAttributeSemantic semantic = attribute.semantic();

                    encoder << encodeEnum(semantic);

                    unsigned cardinality = attribute.cardinality();
                    if (cardinality == 1)
                    {
                        encoder << encodeValue(reader.readAttributeReal(semantic));
                    }
                    else if (cardinality == 2)
                    {
                        encoder << encodeValue(reader.readAttributeVector2(semantic));
                    }
                    else if (cardinality == 3)
                    {
                        encoder << encodeValue(reader.readAttributeVector3(semantic));
                    }
                    else if (cardinality == 4)
                    {
                        encoder << encodeValue(reader.readAttributeVector4(semantic));
                    }
                    encoder << endArray();
                }
                encoder << endArray();
            }
            encoder << endArray();
        }

        // Index data
        {
            encoder << beginArray("indices");
            while (reader.nextIndex())
            {
                switch (mesh.indexType())
                {
                case IndexType_UInt8:
                    encoder << encodeValue(reader.readIndexUInt8());
                    break;
                case IndexType_UInt16:
                    encoder << encodeValue(reader.readIndexUInt16());
                    break;
                case IndexType_UInt32:
                    encoder << encodeValue(reader.readIndexUInt32());
                    break;
                }
            }
            encoder << endArray();
        }
    }
    return encoder;
}

Decoder& operator>>(Decoder& decoder, Mesh& mesh)
{
    if (decoder.isBinaryStream())
    {
        uint64_t identifyNumber;
        decoder >> decodeValue("identifyNumber", identifyNumber);

        if (identifyNumber != Mesh::IdentifyNumber)
        {
            throw Error("Binary data does not represent a mesh");
        }
    }

    // Vertex layout
    if (decoder.selectMember("vertexLayout"))
    {
        decoder >> beginObject();

        VertexLayout vertexLayout;
        decoder >> decodeValue(vertexLayout);
        mesh.setVertexLayout(vertexLayout);

        decoder >> endObject();
    }

    // Index type
    if (decoder.selectMember("indexType"))
    {
        IndexType indexType;
        decoder >> decodeEnum(indexType);

        mesh.setIndexType(indexType);
    }

    // Primitive type
    if (decoder.selectMember("primitiveType"))
    {
        PrimitiveType primitiveType;
        decoder >> decodeEnum(primitiveType);

        mesh.setPrimitiveType(primitiveType);
    }

    // Vertex and index data
    if (decoder.isBinaryStream())
    {
        ReadStream& stream = decoder.binaryStream();

        // Vertex data
        Mesh::VertexData vertexData;
        uint32_t vertexDataSize;
        stream >> vertexDataSize;
        vertexData = Mesh::VertexData(vertexDataSize);
        if (vertexDataSize > 0)
        {
            stream.read(&vertexData[0], vertexDataSize);
        }

        // Index data
        Mesh::IndexData indexData;
        uint32_t indexDataSize;
        stream >> indexDataSize;
        indexData = Mesh::VertexData(indexDataSize);
        if (indexDataSize > 0)
        {
            stream.read(&indexData[0], indexDataSize);
        }

        // Set vertex/index data
        mesh.setVertexData(vertexData);
        mesh.setIndexData(indexData);

        // Compute the bounding box based on the vertex positions
        MeshReader meshReader(mesh);
        while (meshReader.nextVertex())
        {
            Vector3 position = meshReader.readAttributeVector3(VertexAttributeSemantic_Position);
            mesh.axisAlignedBox().expandToInclude(position);
        }
    }
    else
    {
        const VertexLayout& vertexLayout = mesh.vertexLayout();

        // Use a mesh writer to write vertex and index data
        MeshWriter meshWriter(mesh);

        // Vertex data
        if (decoder.selectMember("vertices"))
        {
            decoder >> beginArray();
            while (decoder.hasMoreElements())
            {
                meshWriter.addVertex();

                // For each attribute
                decoder >> beginArray();
                while (decoder.hasMoreElements())
                {
                    decoder >> beginArray();

                    VertexAttributeSemantic semantic;
                    decoder >> decodeEnum(semantic);

                    if (vertexLayout.hasAttributeWithSemantic(semantic))
                    {
                        const VertexAttribute& attribute = vertexLayout.attributeWithSemantic(semantic);
                        unsigned cardinality = attribute.cardinality();

                        if (cardinality == 1)
                        {
                            Real value;
                            decoder >> decodeValue(value);
                            meshWriter.writeAttributeData(semantic, value);
                        }
                        else if (cardinality == 2)
                        {
                            Vector2 value;
                            decoder >> decodeValue(value);
                            meshWriter.writeAttributeData(semantic, value);
                        }
                        else if (cardinality == 3)
                        {
                            Vector3 value;
                            decoder >> decodeValue(value);
                            meshWriter.writeAttributeData(semantic, value);
                        }
                        else if (cardinality == 4)
                        {
                            Vector4 value;
                            decoder >> decodeValue(value);
                            meshWriter.writeAttributeData(semantic, value);
                        }
                    }

                    decoder >> endArray();
                }
                decoder >> endArray();
            }
            decoder >> endArray();
        }

        // Index data
        if (decoder.selectMember("indices"))
        {
            decoder >> beginArray();
            while (decoder.hasMoreElements())
            {
                switch (mesh.indexType())
                {
                case IndexType_UInt8:
                    meshWriter.addIndex(decoder.decodeUInt8());
                    break;
                case IndexType_UInt16:
                    meshWriter.addIndex(decoder.decodeUInt16());
                    break;
                case IndexType_UInt32:
                    meshWriter.addIndex(decoder.decodeUInt32());
                    break;
                }
            }
            decoder >> endArray();
        }
    }

    return decoder;
}

}