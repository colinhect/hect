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
#include "Hect/Graphics/MeshWriter.h"

using namespace hect;

Mesh::Mesh() :
    _vertexLayout(VertexLayout::createDefault()),
    _primitiveType(PrimitiveType::Triangles),
    _indexType(IndexType::UnsignedShort),
    _vertexCount(0),
    _indexCount(0)
{
}

Mesh::Mesh(const std::string& name) :
    _name(name),
    _vertexLayout(VertexLayout::createDefault()),
    _primitiveType(PrimitiveType::Triangles),
    _indexType(IndexType::UnsignedShort),
    _vertexCount(0),
    _indexCount(0)
{
}

Mesh::Mesh(const std::string& name, const VertexLayout& vertexLayout, PrimitiveType primitiveType, IndexType indexType) :
    _name(name),
    _vertexLayout(vertexLayout),
    _primitiveType(primitiveType),
    _indexType(indexType),
    _vertexCount(0),
    _indexCount(0)
{
}

Mesh::Mesh(const Mesh& mesh) :
    _vertexLayout(mesh._vertexLayout),
    _primitiveType(mesh._primitiveType),
    _indexType(mesh._indexType),
    _vertexData(mesh._vertexData),
    _indexData(mesh._indexData),
    _vertexCount(mesh._vertexCount),
    _indexCount(mesh._indexCount),
    _boundingBox(mesh._boundingBox)
{
}

const std::string& Mesh::name() const
{
    return _name;
}

void Mesh::setVertexData(const VertexData& vertexData)
{
    if (_vertexData.size() > 0)
    {
        throw Error("Attempt to set the vertex data of a mesh with vertex data");
    }

    _vertexData = vertexData;
    _vertexCount = vertexData.size() / _vertexLayout.vertexSize();
}

const Mesh::VertexData& Mesh::vertexData() const
{
    return _vertexData;
}

size_t Mesh::vertexCount() const
{
    return _vertexCount;
}

void Mesh::setIndexData(const IndexData& indexData)
{
    if (_indexData.size() > 0)
    {
        throw Error("Attempt to set the index data of a mesh with index data");
    }

    _indexData = indexData;
    _indexCount = indexData.size() / indexSize();
}

const Mesh::IndexData& Mesh::indexData() const
{
    return _indexData;
}

size_t Mesh::indexCount() const
{
    return _indexCount;
}

unsigned Mesh::indexSize() const
{
    switch (_indexType)
    {
    case IndexType::UnsignedByte:
        return 1;
    case IndexType::UnsignedShort:
        return 2;
    case IndexType::UnsignedInt:
        return 4;
    }
    return 0;
}

const VertexLayout& Mesh::vertexLayout() const
{
    return _vertexLayout;
}

PrimitiveType Mesh::primitiveType() const
{
    return _primitiveType;
}

IndexType Mesh::indexType() const
{
    return _indexType;
}

AxisAlignedBox& Mesh::boundingBox()
{
    return _boundingBox;
}

const AxisAlignedBox& Mesh::boundingBox() const
{
    return _boundingBox;
}

void Mesh::save(ObjectWriter& writer) const
{
    writer;
    throw Error("Mesh serialization is not implemented");
}

void Mesh::load(ObjectReader& reader, AssetCache& assetCache)
{
    assetCache;

    // Index type (optional)
    IndexType indexType = _indexType;
    if (reader.hasMember("indexType"))
    {
        indexType = _parseIndexType(reader.readString("indexType"));
    }

    // Primitive type (optional)
    PrimitiveType primitiveType = _primitiveType;
    if (reader.hasMember("primitiveType"))
    {
        primitiveType = _parsePrimitiveType(reader.readString("primitiveType"));
    }

    // Vertex layout (optional)
    VertexLayout vertexLayout = _vertexLayout;
    if (reader.hasMember("vertexLayout"))
    {
        VertexAttribute::Array attributes;
        ArrayReader vertexLayoutReader = reader.readArray("vertexLayout");

        while (vertexLayoutReader.hasMoreElements())
        {
            ObjectReader attributeReader = vertexLayoutReader.readObject();

            auto semantic =_parseAttributeSemantic(attributeReader.readString("semantic"));
            auto type = _parseAttributeType(attributeReader.readString("type"));
            auto cardinality = attributeReader.readUnsignedInt("cardinality");

            attributes.push_back(VertexAttribute(semantic, type, cardinality));
        }

        vertexLayout = VertexLayout(attributes);
    }

    *this = Mesh(_name, vertexLayout, primitiveType, indexType);
    MeshWriter meshWriter(*this);

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

                auto semantic = _parseAttributeSemantic(attributeReader.readString("semantic"));

                switch (semantic)
                {
                case VertexAttributeSemantic::Position:
                case VertexAttributeSemantic::Normal:
                case VertexAttributeSemantic::Tangent:
                case VertexAttributeSemantic::Binormal:
                    meshWriter.writeAttributeData(semantic, attributeReader.readVector3("data"));
                    break;
                default:
                    meshWriter.writeAttributeData(semantic, attributeReader.readVector2("data"));
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
            unsigned indexValue = indicesReader.readUnsignedInt();
            meshWriter.addIndex(indexValue);
        }
    }
}

IndexType Mesh::_parseIndexType(const std::string& value)
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

PrimitiveType Mesh::_parsePrimitiveType(const std::string& value)
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

VertexAttributeSemantic Mesh::_parseAttributeSemantic(const std::string& value)
{
    static std::map<std::string, VertexAttributeSemantic> attributeSemantics;

    if (attributeSemantics.empty())
    {
        attributeSemantics["Position"] = VertexAttributeSemantic::Position;
        attributeSemantics["Normal"] = VertexAttributeSemantic::Normal;
        attributeSemantics["Color"] = VertexAttributeSemantic::Color;
        attributeSemantics["Tangent"] = VertexAttributeSemantic::Tangent;
        attributeSemantics["Binormal"] = VertexAttributeSemantic::Binormal;
        attributeSemantics["Weight0"] = VertexAttributeSemantic::Weight0;
        attributeSemantics["Weight1"] = VertexAttributeSemantic::Weight1;
        attributeSemantics["Weight2"] = VertexAttributeSemantic::Weight2;
        attributeSemantics["Weight3"] = VertexAttributeSemantic::Weight3;
        attributeSemantics["TextureCoords0"] = VertexAttributeSemantic::TextureCoords0;
        attributeSemantics["TextureCoords1"] = VertexAttributeSemantic::TextureCoords1;
        attributeSemantics["TextureCoords2"] = VertexAttributeSemantic::TextureCoords2;
        attributeSemantics["TextureCoords3"] = VertexAttributeSemantic::TextureCoords3;
    }

    auto it = attributeSemantics.find(value);
    if (it == attributeSemantics.end())
    {
        throw Error(format("Invalid vertex attribute semantic '%s'", value.c_str()));
    }

    return (*it).second;
}

VertexAttributeType Mesh::_parseAttributeType(const std::string& value)
{
    static std::map<std::string, VertexAttributeType> attributeTypes;

    if (attributeTypes.empty())
    {
        attributeTypes["Half"] = VertexAttributeType::Half;
        attributeTypes["Float"] = VertexAttributeType::Float;
    }

    auto it = attributeTypes.find(value);
    if (it == attributeTypes.end())
    {
        throw Error(format("Invalid vertex attribute type '%s'", value.c_str()));
    }

    return (*it).second;
}