///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2016 Colin Hill
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

VertexLayout VertexLayout::create_default()
{
    VertexLayout vertex_layout;
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float32, 3));
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Normal, VertexAttributeType::Float32, 3));
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Tangent, VertexAttributeType::Float32, 3));
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::TextureCoords0, VertexAttributeType::Float32, 2));

    return vertex_layout;
}

void VertexLayout::add_attribute(const VertexAttribute& attribute)
{
    _attributes.push_back(attribute);
    compute_attribute_offsets();
}

void VertexLayout::clear_attributes()
{
    _attributes.clear();
    compute_attribute_offsets();
}

bool VertexLayout::has_attribute_with_semantic(VertexAttributeSemantic semantic) const
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

const VertexAttribute& VertexLayout::attribute_with_semantic(VertexAttributeSemantic semantic) const
{
    for (const VertexAttribute& attribute : _attributes)
    {
        if (attribute.semantic() == semantic)
        {
            return attribute;
        }
    }
    throw InvalidOperation(format("Vertex layout does not have an attribute with semantic '%s'", Enum::to_string(semantic).data()));
}

const VertexLayout::AttributeSequence VertexLayout::attributes() const
{
    return AttributeSequence(_attributes);
}

size_t VertexLayout::attribute_count() const
{
    return _attributes.size();
}

unsigned VertexLayout::vertex_size() const
{
    return _vertex_size;
}

bool VertexLayout::operator==(const VertexLayout& vertex_layout) const
{
    // Attribute count
    if (_attributes.size() != vertex_layout._attributes.size())
    {
        return false;
    }

    // Attributes
    size_t attribute_count = _attributes.size();
    for (size_t i = 0; i < attribute_count; ++i)
    {
        if (_attributes[i] != vertex_layout._attributes[i])
        {
            return false;
        }
    }

    // Vertex size
    if (_vertex_size != vertex_layout._vertex_size)
    {
        return false;
    }

    return true;
}

bool VertexLayout::operator!=(const VertexLayout& vertex_layout) const
{
    return !(*this == vertex_layout);
}

void VertexLayout::encode(Encoder& encoder) const
{
    encoder << encode_vector("attributes", _attributes);
}

void VertexLayout::decode(Decoder& decoder)
{
    clear_attributes();

    decoder >> decode_vector("attributes", _attributes);

    compute_attribute_offsets();
}

void VertexLayout::compute_attribute_offsets()
{
    _vertex_size = 0;
    for (VertexAttribute& attribute : _attributes)
    {
        attribute._offset = _vertex_size;
        _vertex_size += attribute.size();
    }
}
