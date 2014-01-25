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

#include "Hect/Graphics/VertexLayoutEncoder.h"

using namespace hect;

VertexLayout VertexLayout::createDefault()
{
    VertexAttribute::Array attributes;
    attributes.push_back(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float, 3));
    attributes.push_back(VertexAttribute(VertexAttributeSemantic::Normal, VertexAttributeType::Float, 3));
    attributes.push_back(VertexAttribute(VertexAttributeSemantic::Tangent, VertexAttributeType::Float, 3));
    attributes.push_back(VertexAttribute(VertexAttributeSemantic::TextureCoords0, VertexAttributeType::Float, 2));

    return VertexLayout(attributes);
}

VertexLayout::VertexLayout() :
    _vertexSize(0)
{
}

VertexLayout::VertexLayout(const VertexAttribute::Array& attributes) :
    _attributes(attributes),
    _vertexSize(0)
{
    _computeAttributeOffsets();
}

const VertexAttribute* VertexLayout::attributeWithSemantic(VertexAttributeSemantic semantic) const
{
    const VertexAttribute* resultingAttribute = nullptr;
    for (const VertexAttribute& attribute : _attributes)
    {
        if (!resultingAttribute && attribute.semantic() == semantic)
        {
            resultingAttribute = &attribute;
        }
    }
    return resultingAttribute;
}

const VertexAttribute::Array& VertexLayout::attributes() const
{
    return _attributes;
}

unsigned VertexLayout::vertexSize() const
{
    return _vertexSize;
}

void VertexLayout::save(ObjectEncoder& encoder) const
{
    VertexLayoutEncoder::save(*this, encoder);
}

void VertexLayout::load(ObjectDecoder& decoder, AssetCache& assetCache)
{
    assetCache;
    VertexLayoutEncoder::load(*this, decoder);
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