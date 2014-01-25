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
#include "VertexLayoutEncoder.h"

using namespace hect;

void VertexLayoutEncoder::encode(const VertexLayout& vertexLayout, ObjectEncoder& encoder)
{
    ArrayEncoder attributesEncoder = encoder.encodeArray("attributes");
    for (const VertexAttribute& attribute : vertexLayout.attributes())
    {
        ObjectEncoder attributeEncoder = attributesEncoder.encodeObject();

        if (attributeEncoder.isBinaryStream())
        {
            WriteStream& stream = attributeEncoder.binaryStream();
            stream.writeUnsignedByte((uint8_t)attribute.semantic());
            stream.writeUnsignedByte((uint8_t)attribute.type());
        }
        else
        {
            attributeEncoder.encodeString("semantic", attributeSemanticToString(attribute.semantic()));
            attributeEncoder.encodeString("type", attributeTypeToString(attribute.type()));
        }

        attributeEncoder.encodeUnsignedInt("cardinality", attribute.cardinality());
    }
}

void VertexLayoutEncoder::decode(VertexLayout& vertexLayout, ObjectDecoder& decoder)
{
    vertexLayout._attributes.clear();

    ArrayDecoder attributesDecoder = decoder.decodeArray("attributes");
    while (attributesDecoder.hasMoreElements())
    {
        ObjectDecoder attributeDecoder = attributesDecoder.decodeObject();

        VertexAttributeSemantic semantic;
        VertexAttributeType type;
        unsigned cardinality;

        if (attributeDecoder.isBinaryStream())
        {
            ReadStream& stream = attributeDecoder.binaryStream();
            semantic = (VertexAttributeSemantic)stream.readUnsignedByte();
            type = (VertexAttributeType)stream.readUnsignedByte();
        }
        else
        {
            semantic = attributeSemanticFromString(attributeDecoder.decodeString("semantic"));
            type = attributeTypeFromString(attributeDecoder.decodeString("type"));
        }

        cardinality = attributeDecoder.decodeUnsignedInt("cardinality");

        VertexAttribute attribute(semantic, type, cardinality);
        vertexLayout._attributes.push_back(attribute);
    }
    vertexLayout._computeAttributeOffsets();
}

VertexAttributeSemantic VertexLayoutEncoder::attributeSemanticFromString(const std::string& value)
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

std::string VertexLayoutEncoder::attributeSemanticToString(VertexAttributeSemantic semantic)
{
    static std::map<VertexAttributeSemantic, std::string> attributeSemanticNames;

    if (attributeSemanticNames.empty())
    {
        attributeSemanticNames[VertexAttributeSemantic::Position] = "Position";
        attributeSemanticNames[VertexAttributeSemantic::Normal] = "Normal";
        attributeSemanticNames[VertexAttributeSemantic::Color] = "Color";
        attributeSemanticNames[VertexAttributeSemantic::Tangent] = "Tangent";
        attributeSemanticNames[VertexAttributeSemantic::Binormal] = "Binormal";
        attributeSemanticNames[VertexAttributeSemantic::Weight0] = "Weight0";
        attributeSemanticNames[VertexAttributeSemantic::Weight1] = "Weight1";
        attributeSemanticNames[VertexAttributeSemantic::Weight2] = "Weight2";
        attributeSemanticNames[VertexAttributeSemantic::Weight3] = "Weight3";
        attributeSemanticNames[VertexAttributeSemantic::TextureCoords0] = "TextureCoords0";
        attributeSemanticNames[VertexAttributeSemantic::TextureCoords1] = "TextureCoords1";
        attributeSemanticNames[VertexAttributeSemantic::TextureCoords2] = "TextureCoords2";
        attributeSemanticNames[VertexAttributeSemantic::TextureCoords3] = "TextureCoords3";
    }

    auto it = attributeSemanticNames.find(semantic);
    if (it == attributeSemanticNames.end())
    {
        throw Error(format("Invalid vertex attribute semantic '%d'", semantic));
    }

    return (*it).second;
}

VertexAttributeType VertexLayoutEncoder::attributeTypeFromString(const std::string& value)
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

std::string VertexLayoutEncoder::attributeTypeToString(VertexAttributeType type)
{
    static std::map<VertexAttributeType, std::string> attributeTypeNames;

    if (attributeTypeNames.empty())
    {
        attributeTypeNames[VertexAttributeType::Half] = "Half";
        attributeTypeNames[VertexAttributeType::Float] = "Float";
    }

    auto it = attributeTypeNames.find(type);
    if (it == attributeTypeNames.end())
    {
        throw Error(format("Invalid vertex attribute type '%d'", type));
    }

    return (*it).second;
}