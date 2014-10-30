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
#include "VertexLayout.h"

#include "Hect/Reflection/Enum.h"

using namespace hect;

VertexLayout VertexLayout::createDefault()
{
    VertexLayout vertexLayout;
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic_Position, VertexAttributeType_Float32, 3));
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic_Normal, VertexAttributeType_Float32, 3));
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic_Tangent, VertexAttributeType_Float32, 3));
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic_TextureCoords0, VertexAttributeType_Float32, 2));

    return vertexLayout;
}

void VertexLayout::addAttribute(const VertexAttribute& attribute)
{
    _attributes.push_back(attribute);
    computeAttributeOffsets();
}

void VertexLayout::clearAttributes()
{
    _attributes.clear();
    computeAttributeOffsets();
}

bool VertexLayout::hasAttributeWithSemantic(VertexAttributeSemantic semantic) const
{
    for (const VertexAttribute& attribute : _attributes)
    {
        if (attribute.semantic() == semantic)
        {
            return true;
        }
    }
    return false;
}

const VertexAttribute& VertexLayout::attributeWithSemantic(VertexAttributeSemantic semantic) const
{
    for (const VertexAttribute& attribute : _attributes)
    {
        if (attribute.semantic() == semantic)
        {
            return attribute;
        }
    }
    throw Error(format("Vertex layout does not have an attribute with semantic '%s'", Enum::toString(semantic).c_str()));
}

const VertexLayout::AttributeSequence VertexLayout::attributes() const
{
    return AttributeSequence(_attributes);
}

size_t VertexLayout::attributeCount() const
{
    return _attributes.size();
}

unsigned VertexLayout::vertexSize() const
{
    return _vertexSize;
}

bool VertexLayout::operator==(const VertexLayout& vertexLayout) const
{
    // Compare attribute count
    if (_attributes.size() != vertexLayout._attributes.size())
    {
        return false;
    }

    // Compare vertex size
    if (_vertexSize != vertexLayout._vertexSize)
    {
        return false;
    }

    // Compare each attribute
    size_t attributeCount = _attributes.size();
    for (size_t i = 0; i < attributeCount; ++i)
    {
        if (_attributes[i] != vertexLayout._attributes[i])
        {
            return false;
        }
    }

    return true;
}

bool VertexLayout::operator!=(const VertexLayout& vertexLayout) const
{
    return !(*this == vertexLayout);
}

void VertexLayout::computeAttributeOffsets()
{
    _vertexSize = 0;
    for (VertexAttribute& attribute : _attributes)
    {
        attribute._offset = _vertexSize;
        _vertexSize += attribute.size();
    }
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const VertexLayout& vertexLayout)
{
    return encoder << beginObject()
           << encodeVector("attributes", vertexLayout._attributes)
           << endObject();
}

Decoder& operator>>(Decoder& decoder, VertexLayout& vertexLayout)
{
    vertexLayout.clearAttributes();

    decoder >> beginObject()
            >> decodeVector("attributes", vertexLayout._attributes)
            >> endObject();

    vertexLayout.computeAttributeOffsets();

    return decoder;
}

}
