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
#include "MeshEncoder.h"

#include "Hect/Graphics/MeshWriter.h"
#include "Hect/Graphics/MeshReader.h"
#include "Hect/IO/Encoders/VertexLayoutEncoder.h"
#include "Hect/Core/Enum.h"

using namespace hect;

void MeshEncoder::encode(const Mesh& mesh, ObjectEncoder& encoder)
{
    // Vertex layout
    {
        ObjectEncoder vertexLayoutEncoder = encoder.encodeObject("vertexLayout");
        mesh.vertexLayout().encode(vertexLayoutEncoder);
    }

    // Index type
    encoder.encodeEnum("indexType", mesh.indexType());

    // Primitive type
    encoder.encodeEnum("primitiveType", mesh.primitiveType());

    if (encoder.isBinaryStream())
    {
        WriteStream& stream = encoder.binaryStream();

        // Vertex data
        uint32_t vertexDataSize = (uint32_t)mesh.vertexData().size();
        stream.writeUnsignedInt(vertexDataSize);
        if (vertexDataSize > 0)
        {
            stream.writeBytes(&mesh.vertexData()[0], vertexDataSize);
        }

        // Index data
        uint32_t indexDataSize = (uint32_t)mesh.indexData().size();
        stream.writeUnsignedInt(indexDataSize);
        if (indexDataSize > 0)
        {
            stream.writeBytes(&mesh.indexData()[0], indexDataSize);
        }
    }
    else
    {
        MeshReader reader(mesh);

        // Vertex data
        {
            ArrayEncoder verticesEncoder = encoder.encodeArray("vertices");
            while (reader.nextVertex())
            {
                ArrayEncoder attributesEncoder = verticesEncoder.encodeArray();
                for (const VertexAttribute& attribute : mesh.vertexLayout().attributes())
                {
                    ArrayEncoder attributeEncoder = attributesEncoder.encodeArray();
                    VertexAttributeSemantic::Enum semantic = attribute.semantic();

                    attributeEncoder.encodeEnum(semantic);

                    unsigned cardinality = attribute.cardinality();
                    if (cardinality == 1)
                    {
                        attributeEncoder.encodeReal(reader.readAttributeReal(semantic));
                    }
                    else if (cardinality == 2)
                    {
                        attributeEncoder.encodeVector2(reader.readAttributeVector2(semantic));
                    }
                    else if (cardinality == 3)
                    {
                        attributeEncoder.encodeVector3(reader.readAttributeVector3(semantic));
                    }
                    else if (cardinality == 4)
                    {
                        attributeEncoder.encodeVector4(reader.readAttributeVector4(semantic));
                    }
                }
            }
        }

        // Index data
        {
            ArrayEncoder indicesEncoder = encoder.encodeArray("indices");
            while (reader.nextIndex())
            {
                switch (mesh.indexType())
                {
                case IndexType::UnsignedByte:
                    indicesEncoder.encodeUnsignedByte(reader.readIndexByte());
                    break;
                case IndexType::UnsignedShort:
                    indicesEncoder.encodeUnsignedShort(reader.readIndexShort());
                    break;
                case IndexType::UnsignedInt:
                    indicesEncoder.encodeUnsignedInt(reader.readIndexInt());
                    break;
                }
            }
        }
    }
}

void MeshEncoder::decode(Mesh& mesh, ObjectDecoder& decoder, AssetCache& assetCache)
{
    // Vertex layout
    if (decoder.hasMember("vertexLayout"))
    {
        ObjectDecoder vertexLayoutDecoder = decoder.decodeObject("vertexLayout");

        VertexLayout vertexLayout;
        vertexLayout.decode(vertexLayoutDecoder, assetCache);

        mesh.setVertexLayout(vertexLayout);
    }

    // Index type
    if (decoder.hasMember("indexType"))
    {
        IndexType::Enum indexType = decoder.decodeEnum<IndexType::Enum>("indexType");

        mesh.setIndexType(indexType);
    }

    // Primitive type
    if (decoder.hasMember("primitiveType"))
    {
        PrimitiveType::Enum primitiveType = decoder.decodeEnum<PrimitiveType::Enum>("primitiveType");
        mesh.setPrimitiveType(primitiveType);
    }

    // Vertex and index data
    if (decoder.isBinaryStream())
    {
        ReadStream& stream = decoder.binaryStream();

        // Vertex data
        Mesh::VertexData vertexData;
        uint32_t vertexDataSize = stream.readUnsignedInt();
        vertexData = Mesh::VertexData(vertexDataSize);
        if (vertexDataSize > 0)
        {
            stream.readBytes(&vertexData[0], vertexDataSize);
        }

        // Index data
        Mesh::IndexData indexData;
        uint32_t indexDataSize = stream.readUnsignedInt();
        indexData = Mesh::VertexData(indexDataSize);
        if (indexDataSize > 0)
        {
            stream.readBytes(&indexData[0], indexDataSize);
        }

        // Set vertex/index data
        mesh.setVertexData(vertexData);
        mesh.setIndexData(indexData);
    }
    else
    {
        const VertexLayout& vertexLayout = mesh.vertexLayout();

        // Use a mesh writer to write vertex and index data
        MeshWriter meshWriter(mesh);

        // Vertex data
        if (decoder.hasMember("vertices"))
        {
            ArrayDecoder verticesDecoder = decoder.decodeArray("vertices");
            while (verticesDecoder.hasMoreElements())
            {
                meshWriter.addVertex();

                // For each attribute
                ArrayDecoder attributesDecoder = verticesDecoder.decodeArray();
                while (attributesDecoder.hasMoreElements())
                {
                    ArrayDecoder attributeDecoder = attributesDecoder.decodeArray();

                    auto semantic = attributeDecoder.decodeEnum<VertexAttributeSemantic::Enum>();

                    if (vertexLayout.hasAttributeWithSemantic(semantic))
                    {
                        const VertexAttribute& attribute = vertexLayout.attributeWithSemantic(semantic);
                        unsigned cardinality = attribute.cardinality();

                        if (cardinality == 1)
                        {
                            meshWriter.writeAttributeData(semantic, attributeDecoder.decodeReal());
                        }
                        else if (cardinality == 2)
                        {
                            meshWriter.writeAttributeData(semantic, attributeDecoder.decodeVector2());
                        }
                        else if (cardinality == 3)
                        {
                            meshWriter.writeAttributeData(semantic, attributeDecoder.decodeVector3());
                        }
                        else if (cardinality == 4)
                        {
                            meshWriter.writeAttributeData(semantic, attributeDecoder.decodeVector4());
                        }
                    }
                }
            }
        }

        // Index data
        if (decoder.hasMember("indices"))
        {
            ArrayDecoder indicesDecoder = decoder.decodeArray("indices");
            while (indicesDecoder.hasMoreElements())
            {
                switch (mesh.indexType())
                {
                case IndexType::UnsignedByte:
                    meshWriter.addIndex(indicesDecoder.decodeUnsignedByte());
                    break;
                case IndexType::UnsignedShort:
                    meshWriter.addIndex(indicesDecoder.decodeUnsignedShort());
                    break;
                case IndexType::UnsignedInt:
                    meshWriter.addIndex(indicesDecoder.decodeUnsignedInt());
                    break;
                }
            }
        }
    }
}