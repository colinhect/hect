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
#include "Mesh.h"

#include "Hect/Core/Exception.h"
#include "Hect/Graphics/MeshReader.h"
#include "Hect/Graphics/MeshWriter.h"
#include "Hect/Graphics/Renderer.h"

using namespace hect;

Mesh::Descriptor::Descriptor()
{
}

Mesh Mesh::create_box(Vector3 dimensions, const Descriptor& descriptor)
{
    Mesh mesh(descriptor);
    MeshWriter mesh_writer(mesh);

    const double half_x = dimensions.x * 0.5;
    const double half_y = dimensions.y * 0.5;
    const double half_z = dimensions.z * 0.5;

    // Top
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(-half_x, -half_y, half_z));
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(half_x, -half_y, half_z));
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(half_x, half_y, half_z));
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(-half_x, half_y, half_z));
    mesh_writer.add_index(0);
    mesh_writer.add_index(1);
    mesh_writer.add_index(2);
    mesh_writer.add_index(2);
    mesh_writer.add_index(3);
    mesh_writer.add_index(0);

    // Bottom
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(-half_x, -half_y, -half_z));
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(half_x, -half_y, -half_z));
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(half_x, half_y, -half_z));
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(-half_x, half_y, -half_z));
    mesh_writer.add_index(4);
    mesh_writer.add_index(5);
    mesh_writer.add_index(6);
    mesh_writer.add_index(6);
    mesh_writer.add_index(7);
    mesh_writer.add_index(4);

    // Left
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(-half_x, -half_y, -half_z));
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(-half_x, -half_y, half_z));
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(-half_x, half_y, half_z));
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(-half_x, half_y, -half_z));
    mesh_writer.add_index(8);
    mesh_writer.add_index(9);
    mesh_writer.add_index(10);
    mesh_writer.add_index(10);
    mesh_writer.add_index(11);
    mesh_writer.add_index(8);

    // Right
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(half_x, -half_y, -half_z));
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(half_x, -half_y, half_z));
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(half_x, half_y, half_z));
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(half_x, half_y, -half_z));
    mesh_writer.add_index(12);
    mesh_writer.add_index(13);
    mesh_writer.add_index(14);
    mesh_writer.add_index(14);
    mesh_writer.add_index(15);
    mesh_writer.add_index(12);

    // Back
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(-half_x, -half_y, -half_z));
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(half_x, -half_y, -half_z));
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(half_x, -half_y, half_z));
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(-half_x, -half_y, half_z));
    mesh_writer.add_index(15);
    mesh_writer.add_index(16);
    mesh_writer.add_index(17);
    mesh_writer.add_index(17);
    mesh_writer.add_index(18);
    mesh_writer.add_index(15);

    // Front
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(-half_x, half_y, -half_z));
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(half_x, half_y, -half_z));
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(half_x, half_y, half_z));
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(-half_x, half_y, half_z));
    mesh_writer.add_index(19);
    mesh_writer.add_index(20);
    mesh_writer.add_index(21);
    mesh_writer.add_index(21);
    mesh_writer.add_index(22);
    mesh_writer.add_index(19);

    return mesh;
}

Mesh::Mesh() :
    _vertex_layout(VertexLayout::create_default())
{
}

Mesh::Mesh(Name name) :
    Asset(name),
    _vertex_layout(VertexLayout::create_default())
{
}

Mesh::Mesh(const Descriptor& descriptor) :
    Asset(descriptor.name),
    _vertex_layout(descriptor.vertex_layout),
    _primitive_type(descriptor.primitive_type),
    _index_type(descriptor.index_type)
{
}

const VertexLayout& Mesh::vertex_layout() const
{
    return _vertex_layout;
}

void Mesh::set_vertex_layout(const VertexLayout& vertex_layout)
{
    if (_vertex_data.size() != 0)
    {
        throw InvalidOperation("Cannot change the vertex layout of a mesh with vertex data");
    }

    if (is_uploaded())
    {
        renderer().destroy_mesh(*this);
    }

    _vertex_layout = vertex_layout;
}

PrimitiveType Mesh::primitive_type() const
{
    return _primitive_type;
}

void Mesh::set_primitive_type(PrimitiveType primitive_type)
{
    if (is_uploaded())
    {
        renderer().destroy_mesh(*this);
    }

    _primitive_type = primitive_type;
}

IndexType Mesh::index_type() const
{
    return _index_type;
}

void Mesh::set_index_type(IndexType index_type)
{
    if (_index_data.size() != 0)
    {
        throw InvalidOperation("Cannot change the index type of a mesh with index data");
    }

    if (is_uploaded())
    {
        renderer().destroy_mesh(*this);
    }

    _index_type = index_type;
}

const Mesh::VertexData& Mesh::vertex_data() const
{
    return _vertex_data;
}

void Mesh::set_vertex_data(const VertexData& vertex_data)
{
    if (is_uploaded())
    {
        renderer().destroy_mesh(*this);
    }

    _vertex_data = vertex_data;
    _vertex_count = vertex_data.size() / _vertex_layout.vertex_size();
}

void Mesh::clear_vertex_data()
{
    if (is_uploaded())
    {
        renderer().destroy_mesh(*this);
    }

    _vertex_data.clear();
    _vertex_count = 0;
}

size_t Mesh::vertex_count() const
{
    return _vertex_count;
}

const Mesh::IndexData& Mesh::index_data() const
{
    return _index_data;
}

void Mesh::set_index_data(const IndexData& index_data)
{
    if (is_uploaded())
    {
        renderer().destroy_mesh(*this);
    }

    _index_data = index_data;
    _index_count = index_data.size() / index_size();
}

void Mesh::clear_index_data()
{
    if (is_uploaded())
    {
        renderer().destroy_mesh(*this);
    }

    _index_data.clear();
    _index_count = 0;
}

size_t Mesh::index_count() const
{
    return _index_count;
}

unsigned Mesh::index_size() const
{
    switch (_index_type)
    {
    case IndexType::UInt8:
        return 1;
    case IndexType::UInt16:
        return 2;
    case IndexType::UInt32:
        return 4;
    }
    return 0;
}

AxisAlignedBox& Mesh::axis_aligned_box()
{
    return _axis_aligned_box;
}

const AxisAlignedBox& Mesh::axis_aligned_box() const
{
    return _axis_aligned_box;
}

bool Mesh::operator==(const Mesh& mesh) const
{
    // Vertex layout
    if (_vertex_layout != mesh._vertex_layout)
    {
        return false;
    }

    // Primitive/index types
    if (_primitive_type != mesh._primitive_type || _index_type != mesh._index_type)
    {
        return false;
    }

    // Vertex/index counts
    if (_vertex_count != mesh._vertex_count || _index_count != mesh._index_count)
    {
        return false;
    }

    // Vertex data
    for (size_t i = 0; i < _vertex_count; ++i)
    {
        if (_vertex_data[i] != mesh._vertex_data[i])
        {
            return false;
        }
    }

    // Index data
    for (size_t i = 0; i < _index_count; ++i)
    {
        if (_index_data[i] != mesh._index_data[i])
        {
            return false;
        }
    }

    return true;
}

bool Mesh::operator!=(const Mesh& mesh) const
{
    return !(*this == mesh);
}

void Mesh::encode(Encoder& encoder) const
{
    encoder << encode_value("vertex_layout", _vertex_layout)
            << encode_enum("index_type", _index_type)
            << encode_enum("primitive_type", _primitive_type);

    if (encoder.is_binary_stream())
    {
        WriteStream& stream = encoder.binary_stream();

        // Vertex data
        uint32_t vertex_data_size = static_cast<uint32_t>(_vertex_data.size());
        stream << vertex_data_size;
        if (vertex_data_size > 0)
        {
            stream.write(&_vertex_data[0], vertex_data_size);
        }

        // Index data
        uint32_t index_data_size = static_cast<uint32_t>(_index_data.size());
        stream << index_data_size;
        if (index_data_size > 0)
        {
            stream.write(&_index_data[0], index_data_size);
        }
    }
    else
    {
        MeshReader reader(*this);

        // Vertex data
        encoder << begin_array("vertices");
        while (reader.next_vertex())
        {
            encoder << begin_object()
                    << begin_array("attributes");

            for (const VertexAttribute& attribute : _vertex_layout.attributes())
            {
                VertexAttributeSemantic semantic = attribute.semantic();

                encoder << begin_object()
                        << encode_enum("semantic", semantic);

                unsigned cardinality = attribute.cardinality();
                if (cardinality == 1)
                {
                    double value = reader.read_attribute_double(semantic);
                    encoder << encode_value("data", value);
                }
                else if (cardinality == 2)
                {
                    Vector2 value = reader.read_attribute_vector2(semantic);
                    encoder << encode_value("data", value);
                }
                else if (cardinality == 3)
                {
                    Vector3 value = reader.read_attribute_vector3(semantic);
                    encoder << encode_value("data", value);
                }
                else if (cardinality == 4)
                {
                    Vector4 value = reader.read_attribute_vector4(semantic);
                    encoder << encode_value("data", value);
                }

                encoder << end_object();
            }

            encoder << end_array()
                    << end_object();
        }
        encoder << end_array();

        // Index data
        encoder << begin_array("indices");
        while (reader.next_index())
        {
            encoder << encode_value(reader.read_index_uint32());
        }
        encoder << end_array();
    }
}

void Mesh::decode(Decoder& decoder)
{
    // Clear any data the mesh already had
    *this = Mesh(name());

    decoder >> decode_value("vertex_layout", _vertex_layout)
            >> decode_enum("index_type", _index_type)
            >> decode_enum("primitive_type", _primitive_type);

    // Vertex and index data
    if (decoder.is_binary_stream())
    {
        ReadStream& stream = decoder.binary_stream();

        // Vertex data
        uint32_t vertex_data_size;
        stream >> vertex_data_size;
        Mesh::VertexData vertex_data(vertex_data_size);
        if (vertex_data_size > 0)
        {
            stream.read(&vertex_data[0], vertex_data_size);
        }

        // Index data
        uint32_t index_data_size;
        stream >> index_data_size;
        Mesh::IndexData index_data(index_data_size);
        if (index_data_size > 0)
        {
            stream.read(&index_data[0], index_data_size);
        }

        // Set vertex/index data
        set_vertex_data(vertex_data);
        set_index_data(index_data);

        // Compute the bounding box based on the vertex positions
        MeshReader mesh_reader(*this);
        while (mesh_reader.next_vertex())
        {
            Vector3 position = mesh_reader.read_attribute_vector3(VertexAttributeSemantic::Position);
            _axis_aligned_box.expand_to_include(position);
        }
    }
    else
    {
        const VertexLayout& vertex_layout = _vertex_layout;

        // Use a mesh writer to write vertex and index data
        MeshWriter mesh_writer(*this);

        // Vertex data
        if (decoder.select_member("vertices"))
        {
            decoder >> begin_array();
            while (decoder.has_more_elements())
            {
                mesh_writer.add_vertex();
                decoder >> begin_object();

                if (!decoder.select_member("attributes"))
                {
                    decoder >> end_object();
                    continue;
                }

                // For each attribute
                decoder >> begin_array();
                while (decoder.has_more_elements())
                {
                    decoder.begin_object();

                    VertexAttributeSemantic semantic;
                    decoder >> decode_enum("semantic", semantic, true);

                    if (vertex_layout.has_attribute_with_semantic(semantic))
                    {
                        const VertexAttribute& attribute = vertex_layout.attribute_with_semantic(semantic);
                        unsigned cardinality = attribute.cardinality();

                        if (cardinality == 1)
                        {
                            double value;
                            decoder >> decode_value("data", value, true);
                            mesh_writer.write_attribute_data(semantic, value);
                        }
                        else if (cardinality == 2)
                        {
                            Vector2 value;
                            decoder >> decode_value("data", value, true);
                            mesh_writer.write_attribute_data(semantic, value);
                        }
                        else if (cardinality == 3)
                        {
                            Vector3 value;
                            decoder >> decode_value("data", value, true);
                            mesh_writer.write_attribute_data(semantic, value);
                        }
                        else if (cardinality == 4)
                        {
                            Vector4 value;
                            decoder >> decode_value("data", value, true);
                            mesh_writer.write_attribute_data(semantic, value);
                        }
                    }
                    decoder >> end_object();
                }
                decoder >> end_array();

                decoder >> end_object();
            }
            decoder >> end_array();
        }

        // Index data
        if (decoder.select_member("indices"))
        {
            decoder >> begin_array();
            while (decoder.has_more_elements())
            {
                mesh_writer.add_index(decoder.decode_uint32());
            }
            decoder >> end_array();
        }
    }
}
