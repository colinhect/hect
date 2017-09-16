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
#include <Hect/Graphics/MeshReader.h>
#include <Hect/Graphics/MeshWriter.h>
using namespace hect;

#include <catch.hpp>

VertexLayout create_vetex_layout()
{
    VertexLayout vertex_layout;
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float32, 3));
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Normal, VertexAttributeType::Float32, 3));

    return vertex_layout;
}

TEST_CASE("Read from an empty mesh", "[MeshReader]")
{
    Mesh mesh("Test");
    mesh.set_vertex_layout(create_vetex_layout());
    mesh.set_primitive_type(PrimitiveType::Triangles);
    mesh.set_index_type(IndexType::UInt8);

    MeshReader mesh_reader(mesh);

    REQUIRE(!mesh_reader.next_vertex());
    REQUIRE(!mesh_reader.next_index());
}

TEST_CASE("Read 3-dimensional vector attributes from a mesh", "[MeshReader]")
{
    Mesh mesh("Test");
    mesh.set_vertex_layout(create_vetex_layout());
    mesh.set_primitive_type(PrimitiveType::Triangles);
    mesh.set_index_type(IndexType::UInt8);

    {
        MeshWriter mesh_writer(mesh);
        mesh_writer.add_vertex();
        mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(1, 2, 3));
        mesh_writer.write_attribute_data(VertexAttributeSemantic::Normal, Vector3(4, 5, 6));
        mesh_writer.add_vertex();
        mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(7, 8, 9));
        mesh_writer.write_attribute_data(VertexAttributeSemantic::Normal, Vector3(10, 11, 12));
        mesh_writer.add_vertex();
        mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(13, 14, 15));
        mesh_writer.write_attribute_data(VertexAttributeSemantic::Normal, Vector3(16, 17, 18));
    }

    Vector3 value;
    MeshReader mesh_reader(mesh);

    REQUIRE(mesh_reader.next_vertex());

    value = mesh_reader.read_attribute_vector3(VertexAttributeSemantic::Position);
    REQUIRE(value.x == 1);
    REQUIRE(value.y == 2);
    REQUIRE(value.z == 3);

    value = mesh_reader.read_attribute_vector3(VertexAttributeSemantic::Normal);
    REQUIRE(value.x == 4);
    REQUIRE(value.y == 5);
    REQUIRE(value.z == 6);

    REQUIRE(mesh_reader.next_vertex());

    value = mesh_reader.read_attribute_vector3(VertexAttributeSemantic::Position);
    REQUIRE(value.x == 7);
    REQUIRE(value.y == 8);
    REQUIRE(value.z == 9);

    value = mesh_reader.read_attribute_vector3(VertexAttributeSemantic::Normal);
    REQUIRE(value.x == 10);
    REQUIRE(value.y == 11);
    REQUIRE(value.z == 12);

    REQUIRE(mesh_reader.next_vertex());

    value = mesh_reader.read_attribute_vector3(VertexAttributeSemantic::Position);
    REQUIRE(value.x == 13);
    REQUIRE(value.y == 14);
    REQUIRE(value.z == 15);

    value = mesh_reader.read_attribute_vector3(VertexAttributeSemantic::Normal);
    REQUIRE(value.x == 16);
    REQUIRE(value.y == 17);
    REQUIRE(value.z == 18);

    REQUIRE(!mesh_reader.next_vertex());
}

TEST_CASE("Read 8-bit indices from a mesh", "[MeshReader]")
{
    Mesh mesh("Test");
    mesh.set_vertex_layout(create_vetex_layout());
    mesh.set_primitive_type(PrimitiveType::Triangles);
    mesh.set_index_type(IndexType::UInt8);

    {
        MeshWriter mesh_writer(mesh);
        mesh_writer.add_index(0);
        mesh_writer.add_index(1);
        mesh_writer.add_index(2);
    }

    MeshReader mesh_reader(mesh);

    REQUIRE(mesh_reader.next_index());
    REQUIRE(mesh_reader.read_index_uint8() == 0);

    REQUIRE(mesh_reader.next_index());
    REQUIRE(mesh_reader.read_index_uint8() == 1);

    REQUIRE(mesh_reader.next_index());
    REQUIRE(mesh_reader.read_index_uint8() == 2);

    REQUIRE(!mesh_reader.next_index());
}