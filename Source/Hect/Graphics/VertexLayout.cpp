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

#include "Hect/IO/Encoders/VertexLayoutEncoder.h"
#include "Hect/Core/Enum.h"

using namespace hect;

VertexLayout VertexLayout::createDefault()
{
    VertexLayout vertexLayout;
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float, 3));
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic::Normal, VertexAttributeType::Float, 3));
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic::Tangent, VertexAttributeType::Float, 3));
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic::TextureCoords0, VertexAttributeType::Float, 2));

    return vertexLayout;
}

VertexLayout::VertexLayout() :
    _vertexSize(0)
{
}

void VertexLayout::addAttribute(const VertexAttribute& attribute)
{
    _attributes.push_back(attribute);
    _computeAttributeOffsets();
}

void VertexLayout::clearAttributes()
{
    _attributes.clear();
    _computeAttributeOffsets();
}

bool VertexLayout::hasAttributeWithSemantic(VertexAttributeSemantic::Enum semantic) const
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

const VertexAttribute& VertexLayout::attributeWithSemantic(VertexAttributeSemantic::Enum semantic) const
{
    for (const VertexAttribute& attribute : _attributes)
    {
        if (attribute.semantic() == semantic)
        {
            return attribute;
        }
    }
    throw Error(format("Vertex layout does not have an attribute with semantic '%s'", Enum::toString(semantic)));
}

const VertexAttribute::Array& VertexLayout::attributes() const
{
    return _attributes;
}

unsigned VertexLayout::vertexSize() const
{
    return _vertexSize;
}

void VertexLayout::encode(ObjectEncoder& encoder) const
{
    VertexLayoutEncoder::encode(*this, encoder);
}

void VertexLayout::decode(ObjectDecoder& decoder, AssetCache& assetCache)
{
    assetCache;
    VertexLayoutEncoder::decode(*this, decoder);
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

void VertexLayout::_computeAttributeOffsets()
{
    _vertexSize = 0;
    for (VertexAttribute& attribute : _attributes)
    {
        attribute._offset = _vertexSize;
        _vertexSize += attribute.size();
    }
}