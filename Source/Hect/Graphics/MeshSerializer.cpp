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
#include "MeshSerializer.h"

#include "Hect/Graphics/MeshWriter.h"
#include "Hect/Graphics/MeshReader.h"
#include "Hect/Graphics/VertexLayoutSerializer.h"

using namespace hect;

void MeshSerializer::save(const Mesh& mesh, ObjectWriter& writer)
{
    // Vertex layout
    {
        ObjectWriter vertexLayoutWriter = writer.writeObject("vertexLayout");
        mesh.vertexLayout().save(vertexLayoutWriter);
    }
    
    // Index type
    writer.writeString("indexType", indexTypeToString(mesh.indexType()));

    // Primitive type
    writer.writeString("primitiveType", primitiveTypeToString(mesh.primitiveType()));

    MeshReader reader(mesh);

    // Vertex data
    {
        ArrayWriter verticesWriter = writer.writeArray("vertices");
        while (reader.nextVertex())
        {
            ArrayWriter attributesWriter = verticesWriter.writeArray();
            for (const VertexAttribute& attribute : mesh.vertexLayout().attributes())
            {
                ObjectWriter attributeWriter = attributesWriter.writeObject();
                VertexAttributeSemantic semantic = attribute.semantic();

                attributeWriter.writeString("semantic", VertexLayoutSerializer::attributeSemanticToString(semantic));

                unsigned cardinality = attribute.cardinality();
                if (cardinality == 1)
                {
                    attributeWriter.writeReal("data", reader.readAttributeReal(semantic));
                }
                else if (cardinality == 2)
                {
                    attributeWriter.writeVector2("data", reader.readAttributeVector2(semantic));
                }
                else if (cardinality == 3)
                {
                    attributeWriter.writeVector3("data", reader.readAttributeVector3(semantic));
                }
                else if (cardinality == 4)
                {
                    attributeWriter.writeVector4("data", reader.readAttributeVector4(semantic));
                }
            }
        }
    }

    // Index data
    {
        ArrayWriter indicesWriter = writer.writeArray("indices");
        while (reader.nextIndex())
        {
            switch (mesh.indexType())
            {
            case IndexType::UnsignedByte:
                indicesWriter.writeUnsignedByte(reader.readIndexByte());
                break;
            case IndexType::UnsignedShort:
                indicesWriter.writeUnsignedShort(reader.readIndexShort());
                break;
            case IndexType::UnsignedInt:
                indicesWriter.writeUnsignedInt(reader.readIndexInt());
                break;
            }
        }
    }
}

void MeshSerializer::load(Mesh& mesh, ObjectReader& reader, AssetCache& assetCache)
{
    mesh.clear();

    // Vertex layout
    if (reader.hasMember("vertexLayout"))
    {
        ObjectReader vertexLayoutReader = reader.readObject("vertexLayout");
        mesh._vertexLayout.load(vertexLayoutReader, assetCache);
    }

    // Index type
    if (reader.hasMember("indexType"))
    {
        mesh._indexType = indexTypeFromString(reader.readString("indexType"));
    }

    // Primitive type
    if (reader.hasMember("primitiveType"))
    {
        mesh._primitiveType = primitiveTypeFromString(reader.readString("primitiveType"));
    }

    MeshWriter meshWriter(mesh);

    // Add the vertices
    if (reader.hasMember("vertices"))
    {
        ArrayReader verticesReader = reader.readArray("vertices");
        while (verticesReader.hasMoreElements())
        {
            meshWriter.addVertex();

            // For each attribute
            ArrayReader attributesReader = verticesReader.readArray();
            while (attributesReader.hasMoreElements())
            {
                ObjectReader attributeReader = attributesReader.readObject();

                auto semantic = VertexLayoutSerializer::attributeSemanticFromString(attributeReader.readString("semantic"));
                const VertexAttribute* attribute = mesh.vertexLayout().attributeWithSemantic(semantic);
                if (attribute)
                {
                    unsigned cardinality = attribute->cardinality();

                    if (cardinality == 1)
                    {
                        meshWriter.writeAttributeData(semantic, attributeReader.readReal("data"));
                    }
                    else if (cardinality == 2)
                    {
                        meshWriter.writeAttributeData(semantic, attributeReader.readVector2("data"));
                    }
                    else if (cardinality == 3)
                    {
                        meshWriter.writeAttributeData(semantic, attributeReader.readVector3("data"));
                    }
                    else if (cardinality == 4)
                    {
                        meshWriter.writeAttributeData(semantic, attributeReader.readVector4("data"));
                    }
                }
            }
        }
    }

    // Add the indices
    if (reader.hasMember("indices"))
    {
        ArrayReader indicesReader = reader.readArray("indices");
        while (indicesReader.hasMoreElements())
        {
            switch (mesh.indexType())
            {
            case IndexType::UnsignedByte:
                meshWriter.addIndex(indicesReader.readUnsignedByte());
                break;
            case IndexType::UnsignedShort:
                meshWriter.addIndex(indicesReader.readUnsignedShort());
                break;
            case IndexType::UnsignedInt:
                meshWriter.addIndex(indicesReader.readUnsignedInt());
                break;
            }
        }
    }
}

IndexType MeshSerializer::indexTypeFromString(const std::string& value)
{
    static std::map<std::string, IndexType> indexTypes;

    if (indexTypes.empty())
    {
        indexTypes["UnsignedByte"] = IndexType::UnsignedByte;
        indexTypes["UnsignedShort"] = IndexType::UnsignedShort;
        indexTypes["UnsignedInt"] = IndexType::UnsignedInt;
    }

    auto it = indexTypes.find(value);
    if (it == indexTypes.end())
    {
        throw Error(format("Invalid index type '%s'", value.c_str()));
    }

    return (*it).second;
}

std::string MeshSerializer::indexTypeToString(IndexType indexType)
{
    static std::map<IndexType, std::string> indexTypeNames;

    if (indexTypeNames.empty())
    {
        indexTypeNames[IndexType::UnsignedByte] = "UnsignedByte";
        indexTypeNames[IndexType::UnsignedShort] = "UnsignedShort";
        indexTypeNames[IndexType::UnsignedInt] = "UnsignedInt";
    }

    auto it = indexTypeNames.find(indexType);
    if (it == indexTypeNames.end())
    {
        throw Error(format("Invalid index type '%d'", indexType));
    }

    return (*it).second;
}
    
PrimitiveType MeshSerializer::primitiveTypeFromString(const std::string& value)
{
    static std::map<std::string, PrimitiveType> primitiveTypes;

    if (primitiveTypes.empty())
    {
        primitiveTypes["Triangles"] = PrimitiveType::Triangles;
        primitiveTypes["TriangleStrip"] = PrimitiveType::TriangleStrip;
        primitiveTypes["Lines"] = PrimitiveType::Lines;
        primitiveTypes["LineStrip"] = PrimitiveType::LineStrip;
        primitiveTypes["Points"] = PrimitiveType::Points;
    }

    auto it = primitiveTypes.find(value);
    if (it == primitiveTypes.end())
    {
        throw Error(format("Invalid primitive type '%s'", value.c_str()));
    }

    return (*it).second;
}

std::string MeshSerializer::primitiveTypeToString(PrimitiveType primitiveType)
{
    static std::map<PrimitiveType, std::string> primitiveTypeNames;

    if (primitiveTypeNames.empty())
    {
        primitiveTypeNames[PrimitiveType::Triangles] = "Triangles";
        primitiveTypeNames[PrimitiveType::TriangleStrip] = "TriangleStrip";
        primitiveTypeNames[PrimitiveType::Lines] = "Lines";
        primitiveTypeNames[PrimitiveType::LineStrip] = "LineStrip";
        primitiveTypeNames[PrimitiveType::Points] = "Points";
    }

    auto it = primitiveTypeNames.find(primitiveType);
    if (it == primitiveTypeNames.end())
    {
        throw Error(format("Invalid primitive type '%d'", primitiveType));
    }

    return (*it).second;
}