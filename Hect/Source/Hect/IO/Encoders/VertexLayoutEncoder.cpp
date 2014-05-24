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

#include "Hect/Reflection/Enum.h"

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
            attributeEncoder.encodeString("semantic", Enum::toString(attribute.semantic()));
            attributeEncoder.encodeString("type", Enum::toString(attribute.type()));
        }

        attributeEncoder.encodeUnsignedInt("cardinality", attribute.cardinality());
    }
}

void VertexLayoutEncoder::decode(VertexLayout& vertexLayout, ObjectDecoder& decoder)
{
    vertexLayout.clearAttributes();

    // Attributes
    ArrayDecoder attributesDecoder = decoder.decodeArray("attributes");
    while (attributesDecoder.hasMoreElements())
    {
        ObjectDecoder attributeDecoder = attributesDecoder.decodeObject();

        VertexAttributeSemantic::Enum semantic;
        VertexAttributeType::Enum type;
        unsigned cardinality;

        if (attributeDecoder.isBinaryStream())
        {
            ReadStream& stream = attributeDecoder.binaryStream();
            semantic = (VertexAttributeSemantic::Enum)stream.readUnsignedByte();
            type = (VertexAttributeType::Enum)stream.readUnsignedByte();
        }
        else
        {
            semantic = Enum::fromString<VertexAttributeSemantic::Enum>(attributeDecoder.decodeString("semantic"));
            type = Enum::fromString<VertexAttributeType::Enum>(attributeDecoder.decodeString("type"));
        }

        cardinality = attributeDecoder.decodeUnsignedInt("cardinality");

        // Append the new attribute
        VertexAttribute attribute(semantic, type, cardinality);
        vertexLayout.addAttribute(attribute);
    }
}