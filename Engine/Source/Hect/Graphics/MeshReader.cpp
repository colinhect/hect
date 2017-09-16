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
#include "MeshReader.h"

using namespace hect;

MeshReader::MeshReader(const Mesh& mesh) :
    _mesh(mesh),
    _vertex_stream(mesh.vertex_data()),
    _index_stream(mesh.index_data())
{
}

bool MeshReader::next_vertex()
{
    if (_vertex_count != 0)
    {
        _vertex_position += _mesh.vertex_layout().vertex_size();
    }

    ++_vertex_count;
    return _mesh.vertex_count() >= _vertex_count;
}

double MeshReader::read_attribute_double(VertexAttributeSemantic semantic) const
{
    check_vertex_boundary();

    float value = 0;

    const VertexLayout& vertex_layout = _mesh.vertex_layout();
    if (vertex_layout.has_attribute_with_semantic(semantic))
    {
        const VertexAttribute& attribute = vertex_layout.attribute_with_semantic(semantic);
        value = read_component_value(attribute, 0);
    }

    return value;
}

Vector2 MeshReader::read_attribute_vector2(VertexAttributeSemantic semantic) const
{
    check_vertex_boundary();

    Vector2 value;

    const VertexLayout& vertex_layout = _mesh.vertex_layout();
    if (vertex_layout.has_attribute_with_semantic(semantic))
    {
        const VertexAttribute& attribute = vertex_layout.attribute_with_semantic(semantic);

        unsigned cardinality = attribute.cardinality();
        if (cardinality > 0)
        {
            value.x = read_component_value(attribute, 0);
        }

        if (cardinality > 1)
        {
            value.y = read_component_value(attribute, 1);
        }
    }

    return value;
}

Vector3 MeshReader::read_attribute_vector3(VertexAttributeSemantic semantic) const
{
    check_vertex_boundary();

    Vector3 value;

    const VertexLayout& vertex_layout = _mesh.vertex_layout();
    if (vertex_layout.has_attribute_with_semantic(semantic))
    {
        const VertexAttribute& attribute = vertex_layout.attribute_with_semantic(semantic);

        unsigned cardinality = attribute.cardinality();

        if (cardinality > 0)
        {
            value.x = read_component_value(attribute, 0);
        }

        if (cardinality > 1)
        {
            value.y = read_component_value(attribute, 1);
        }

        if (cardinality > 2)
        {
            value.z = read_component_value(attribute, 2);
        }
    }

    return value;
}

Vector4 MeshReader::read_attribute_vector4(VertexAttributeSemantic semantic) const
{
    check_vertex_boundary();

    Vector4 value;

    const VertexLayout& vertex_layout = _mesh.vertex_layout();
    if (vertex_layout.has_attribute_with_semantic(semantic))
    {
        const VertexAttribute& attribute = vertex_layout.attribute_with_semantic(semantic);

        unsigned cardinality = attribute.cardinality();

        if (cardinality > 0)
        {
            value.x = read_component_value(attribute, 0);
        }

        if (cardinality > 1)
        {
            value.y = read_component_value(attribute, 1);
        }

        if (cardinality > 2)
        {
            value.z = read_component_value(attribute, 2);
        }

        if (cardinality > 3)
        {
            value.w = read_component_value(attribute, 3);
        }
    }

    return value;
}

Color MeshReader::read_attribute_color(VertexAttributeSemantic semantic) const
{
    Vector4 value = read_attribute_vector4(semantic);
    return Color(value.x, value.y, value.z, value.w);
}

bool MeshReader::next_index()
{
    if (_index_count != 0)
    {
        _index_position += _mesh.index_size();
    }

    ++_index_count;
    return _mesh.index_count() >= _index_count;
}

uint8_t MeshReader::read_index_uint8() const
{
    return static_cast<uint8_t>(read_index_uint32());
}

uint16_t MeshReader::read_index_uint16() const
{
    return static_cast<uint16_t>(read_index_uint32());
}

uint32_t MeshReader::read_index_uint32() const
{
    check_index_boundary();
    _index_stream.seek(_index_position);

    // Read the index data based on the type
    uint32_t index = 0;
    switch (_mesh.index_type())
    {
    case IndexType::UInt8:
    {
        uint8_t value;
        _index_stream >> value;
        index = value;
    }
    break;
    case IndexType::UInt16:
    {
        uint16_t value;
        _index_stream >> value;
        index = value;
    }
    break;
    case IndexType::UInt32:
        _index_stream >> index;
        break;
    }

    return index;
}

void MeshReader::check_vertex_boundary() const
{
    if (_vertex_count == 0)
    {
        throw InvalidOperation("Cannot read attribute before moving to the first vertex");
    }
    else if (_vertex_count > _mesh.vertex_count())
    {
        throw InvalidOperation("Cannot read past the last vertex");
    }
}

void MeshReader::check_index_boundary() const
{
    if (_index_count == 0)
    {
        throw InvalidOperation("Cannot read index before moving to the first index");
    }
    else if (_index_count > _mesh.index_count())
    {
        throw InvalidOperation("Cannot read past the last index");
    }
}

float MeshReader::read_component_value(const VertexAttribute& attribute, unsigned index) const
{
    _vertex_stream.seek(_vertex_position);

    size_t offset = _vertex_position + attribute.offset();

    // Read the vertex data based on the type
    float value = 0;
    switch (attribute.type())
    {
    case VertexAttributeType::Int8:
    {
        _vertex_stream.seek(offset + index * sizeof(int8_t));
        int8_t read_value;
        _vertex_stream >> read_value;
        value = static_cast<float>(read_value);
    }
    break;
    case VertexAttributeType::UInt8:
    {
        _vertex_stream.seek(offset + index * sizeof(uint8_t));
        uint8_t read_value;
        _vertex_stream >> read_value;
        value = static_cast<float>(read_value);
    }
    break;
    case VertexAttributeType::Int16:
    {
        _vertex_stream.seek(offset + index * sizeof(int16_t));
        int16_t read_value;
        _vertex_stream >> read_value;
        value = static_cast<float>(read_value);
    }
    break;
    case VertexAttributeType::UInt16:
    {
        _vertex_stream.seek(offset + index * sizeof(uint16_t));
        uint16_t read_value;
        _vertex_stream >> read_value;
        value = static_cast<float>(read_value);
    }
    break;
    case VertexAttributeType::Int32:
    {
        _vertex_stream.seek(offset + index * sizeof(int32_t));
        int32_t read_value;
        _vertex_stream >> read_value;
        value = static_cast<float>(read_value);
    }
    break;
    case VertexAttributeType::UInt32:
    {
        _vertex_stream.seek(offset + index * sizeof(uint32_t));
        uint32_t read_value;
        _vertex_stream >> read_value;
        value = static_cast<float>(read_value);
    }
    break;
    case VertexAttributeType::Reserved:
        // Float 16
        break;
    case VertexAttributeType::Float32:
        _vertex_stream.seek(offset + index * sizeof(float));
        _vertex_stream >> value;
        break;
    }

    return value;
}