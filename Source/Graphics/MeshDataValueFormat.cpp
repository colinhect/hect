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
#include "Hect.h"

using namespace hect;

void MeshDataValueFormat::load(Mesh& mesh, const std::string& name, const DataValue& dataValue)
{
    // Index type (optional)
    IndexType indexType = mesh.indexType();
    if (dataValue["indexType"].isString())
    {
        indexType = _parseIndexType(dataValue["indexType"].asString());
    }

    // Primitive type (optional)
    PrimitiveType primitiveType = mesh.primitiveType();
    if (dataValue["primitiveType"].isString())
    {
        primitiveType = _parsePrimitiveType(dataValue["primitiveType"].asString());
    }

    // Vertex layout (optional)
    VertexLayout vertexLayout = mesh.vertexLayout();
    if (dataValue["vertexLayout"].isArray())
    {
        VertexAttribute::Array attributes;
        for (const DataValue& attribute : dataValue["vertexLayout"])
        {
            auto semantic =_parseAttributeSemantic(attribute["semantic"].asString());
            auto type = _parseAttributeType(attribute["type"].asString());
            auto cardinality = attribute["cardinality"].asUnsigned();

            attributes.push_back(VertexAttribute(semantic, type, cardinality));
        }
        vertexLayout = VertexLayout(attributes);
    }

    mesh = Mesh(name, vertexLayout, primitiveType, indexType);
    MeshWriter meshWriter(mesh);

    // Add the vertices
    for (const DataValue& vertex : dataValue["vertices"])
    {
        meshWriter.addVertex();

        // For each attribute
        for (const DataValue& attribute : vertex)
        {
            auto semantic = _parseAttributeSemantic(attribute["semantic"].asString());

            switch (semantic)
            {
            case VertexAttributeSemantic::Position:
            case VertexAttributeSemantic::Normal:
            case VertexAttributeSemantic::Tangent:
            case VertexAttributeSemantic::Binormal:
                meshWriter.writeAttributeData(semantic, attribute["data"].asVector3());
                break;
            default:
                meshWriter.writeAttributeData(semantic, attribute["data"].asVector2());
            }
        }
    }

    // Add the indices
    for (const DataValue& index : dataValue["indices"])
    {
        unsigned indexValue = index.asUnsigned();
        meshWriter.addIndex(indexValue);
    }
}

IndexType MeshDataValueFormat::_parseIndexType(const DataValue& dataValue)
{
    static std::map<std::string, IndexType> indexTypes;

    if (indexTypes.empty())
    {
        indexTypes["UnsignedByte"] = IndexType::UnsignedByte;
        indexTypes["UnsignedShort"] = IndexType::UnsignedShort;
        indexTypes["UnsignedInt"] = IndexType::UnsignedInt;
    }

    auto it = indexTypes.find(dataValue.asString());
    if (it == indexTypes.end())
    {
        throw Error(format("Invalid index type '%s'", dataValue.asString().c_str()));
    }

    return (*it).second;
}

PrimitiveType MeshDataValueFormat::_parsePrimitiveType(const DataValue& dataValue)
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

    auto it = primitiveTypes.find(dataValue.asString());
    if (it == primitiveTypes.end())
    {
        throw Error(format("Invalid primitive type '%s'", dataValue.asString().c_str()));
    }

    return (*it).second;
}

VertexAttributeSemantic MeshDataValueFormat::_parseAttributeSemantic(const DataValue& dataValue)
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

    auto it = attributeSemantics.find(dataValue.asString());
    if (it == attributeSemantics.end())
    {
        throw Error(format("Invalid vertex attribute semantic '%s'", dataValue.asString().c_str()));
    }

    return (*it).second;
}

VertexAttributeType MeshDataValueFormat::_parseAttributeType(const DataValue& dataValue)
{
    static std::map<std::string, VertexAttributeType> attributeTypes;

    if (attributeTypes.empty())
    {
        attributeTypes["Half"] = VertexAttributeType::Half;
        attributeTypes["Float"] = VertexAttributeType::Float;
    }

    auto it = attributeTypes.find(dataValue.asString());
    if (it == attributeTypes.end())
    {
        throw Error(format("Invalid vertex attribute type '%s'", dataValue.asString().c_str()));
    }

    return (*it).second;
}