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
#include "MeshWriter.h"

#include "Hect/Core/Exception.h"

using namespace hect;

MeshWriter::MeshWriter(Mesh& mesh) :
    _mesh(mesh),
    _vertex_stream(mesh._vertex_data),
    _index_stream(mesh._index_data)
{
}

size_t MeshWriter::add_vertex()
{
    _vertex_position = _vertex_stream.position();

    // Push back zeroed data for the added vertex
    for (unsigned i = 0; i < _mesh.vertex_layout().vertex_size(); ++i)
    {
        _vertex_stream << static_cast<uint8_t>(0);
    }

    return _mesh._vertex_count++;
}

void MeshWriter::write_attribute_data(VertexAttributeSemantic semantic, double value)
{
    const VertexLayout& vertex_layout = _mesh.vertex_layout();
    if (vertex_layout.has_attribute_with_semantic(semantic))
    {
        const VertexAttribute& attribute = vertex_layout.attribute_with_semantic(semantic);
        set_component_value(attribute, 0, static_cast<float>(value));
    }
}

void MeshWriter::write_attribute_data(VertexAttributeSemantic semantic, Vector2 value)
{
    const VertexLayout& vertex_layout = _mesh.vertex_layout();
    if (vertex_layout.has_attribute_with_semantic(semantic))
    {
        const VertexAttribute& attribute = vertex_layout.attribute_with_semantic(semantic);

        unsigned cardinality = attribute.cardinality();

        if (cardinality > 0)
        {
            set_component_value(attribute, 0, static_cast<float>(value.x));
        }

        if (cardinality > 1)
        {
            set_component_value(attribute, 1, static_cast<float>(value.y));
        }
    }
}

void MeshWriter::write_attribute_data(VertexAttributeSemantic semantic, Vector3 value)
{
    // If this data is a position then expand the bounding box to include it
    if (semantic == VertexAttributeSemantic::Position)
    {
        _mesh.axis_aligned_box().expand_to_include(value);
    }

    const VertexLayout& vertex_layout = _mesh.vertex_layout();
    if (vertex_layout.has_attribute_with_semantic(semantic))
    {
        const VertexAttribute& attribute = vertex_layout.attribute_with_semantic(semantic);

        unsigned cardinality = attribute.cardinality();

        if (cardinality > 0)
        {
            set_component_value(attribute, 0, static_cast<float>(value.x));
        }

        if (cardinality > 1)
        {
            set_component_value(attribute, 1, static_cast<float>(value.y));
        }

        if (cardinality > 2)
        {
            set_component_value(attribute, 2, static_cast<float>(value.z));
        }
    }
}

void MeshWriter::write_attribute_data(VertexAttributeSemantic semantic, Vector4 value)
{
    const VertexLayout& vertex_layout = _mesh.vertex_layout();
    if (vertex_layout.has_attribute_with_semantic(semantic))
    {
        const VertexAttribute& attribute = vertex_layout.attribute_with_semantic(semantic);

        unsigned cardinality = attribute.cardinality();

        if (cardinality > 0)
        {
            set_component_value(attribute, 0, static_cast<float>(value.x));
        }

        if (cardinality > 1)
        {
            set_component_value(attribute, 1, static_cast<float>(value.y));
        }

        if (cardinality > 2)
        {
            set_component_value(attribute, 2, static_cast<float>(value.z));
        }

        if (cardinality > 3)
        {
            set_component_value(attribute, 3, static_cast<float>(value.w));
        }
    }
}

void MeshWriter::write_attribute_data(VertexAttributeSemantic semantic, Color value)
{
    write_attribute_data(semantic, Vector4(value.r, value.g, value.b, value.a));
}

void MeshWriter::add_index(uint64_t value)
{
    // Write the index data based on the type
    switch (_mesh.index_type())
    {
    case IndexType::UInt8:
        _index_stream << static_cast<uint8_t>(value);
        break;
    case IndexType::UInt16:
        _index_stream << static_cast<uint16_t>(value);
        break;
    case IndexType::UInt32:
        _index_stream << static_cast<uint32_t>(value);
        break;
    }

    ++_mesh._index_count;
}

void MeshWriter::set_component_value(const VertexAttribute& attribute, unsigned index, float value)
{
    size_t position = _vertex_stream.position();
    size_t offset = _vertex_position + attribute.offset();

    // Write the vertex data based on the type
    switch (attribute.type())
    {
    case VertexAttributeType::Int8:
        _vertex_stream.seek(offset + index * sizeof(int8_t));
        _vertex_stream << static_cast<int8_t>(value);
        break;
    case VertexAttributeType::UInt8:
        _vertex_stream.seek(offset + index * sizeof(uint8_t));
        _vertex_stream << static_cast<uint8_t>(value);
        break;
    case VertexAttributeType::Int16:
        _vertex_stream.seek(offset + index * sizeof(int16_t));
        _vertex_stream << static_cast<int16_t>(value);
        break;
    case VertexAttributeType::UInt16:
        _vertex_stream.seek(offset + index * sizeof(uint16_t));
        _vertex_stream << static_cast<uint16_t>(value);
        break;
    case VertexAttributeType::Int32:
        _vertex_stream.seek(offset + index * sizeof(int32_t));
        _vertex_stream << static_cast<int32_t>(value);
        break;
    case VertexAttributeType::UInt32:
        _vertex_stream.seek(offset + index * sizeof(uint32_t));
        _vertex_stream << static_cast<uint32_t>(value);
        break;
    case VertexAttributeType::Reserved:
        // Float 16
        break;
    case VertexAttributeType::Float32:
        _vertex_stream.seek(offset + index * sizeof(float));
        _vertex_stream << value;
        break;
    }

    _vertex_stream.seek(position);
}