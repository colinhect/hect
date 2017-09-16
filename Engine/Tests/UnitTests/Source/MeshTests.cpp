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
#include <Hect/Graphics/Mesh.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Construct empty mesh", "[Mesh]")
{
    Mesh mesh;

    const VertexLayout& mesh_vertex_layout = mesh.vertex_layout();
    REQUIRE(VertexAttributeSemantic::Position == mesh_vertex_layout.attributes().begin()->semantic());
    REQUIRE(PrimitiveType::Triangles == mesh.primitive_type());
    REQUIRE(IndexType::UInt16 == mesh.index_type());
}

TEST_CASE("Construct a mesh with a descriptor", "[Mesh]")
{
    VertexLayout vertex_layout;
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float32, 3));
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Normal, VertexAttributeType::Int16, 3));

    Mesh::Descriptor descriptor;
    descriptor.name = "Test";
    descriptor.vertex_layout = vertex_layout;
    descriptor.primitive_type = PrimitiveType::TriangleStrip;
    descriptor.index_type = IndexType::UInt8;

    Mesh mesh(descriptor);

    const VertexLayout& mesh_vertex_layout = mesh.vertex_layout();
    REQUIRE(mesh_vertex_layout.attribute_count() == 2u);
    REQUIRE(VertexAttributeSemantic::Position == mesh_vertex_layout.attributes().begin()->semantic());
    REQUIRE(VertexAttributeType::Float32 == mesh_vertex_layout.attributes().begin()->type());
    REQUIRE(mesh_vertex_layout.attributes().begin()->cardinality() == 3u);

    REQUIRE(PrimitiveType::TriangleStrip == mesh.primitive_type());
    REQUIRE(IndexType::UInt8 == mesh.index_type());
}

TEST_CASE("Construct mesh with name, then set the vertex layout, primitive type, and index type", "[Mesh]")
{
    VertexLayout vertex_layout;
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float32, 3));
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Normal, VertexAttributeType::Int16, 3));

    Mesh mesh("Test");
    mesh.set_vertex_layout(vertex_layout);
    mesh.set_primitive_type(PrimitiveType::Triangles);
    mesh.set_index_type(IndexType::UInt8);

    const VertexLayout& mesh_vertex_layout = mesh.vertex_layout();
    REQUIRE(mesh_vertex_layout.attribute_count() == 2u);
    REQUIRE(VertexAttributeSemantic::Position == mesh_vertex_layout.attributes().begin()->semantic());
    REQUIRE(VertexAttributeType::Float32 == mesh_vertex_layout.attributes().begin()->type());
    REQUIRE(mesh_vertex_layout.attributes().begin()->cardinality() == 3u);

    REQUIRE(PrimitiveType::Triangles == mesh.primitive_type());
    REQUIRE(IndexType::UInt8 == mesh.index_type());
}

TEST_CASE("Set the vertex data of a mesh", "[Mesh]")
{
    VertexLayout vertex_layout;
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float32, 3));

    Mesh mesh("Test");
    mesh.set_vertex_layout(vertex_layout);
    mesh.set_primitive_type(PrimitiveType::Triangles);
    mesh.set_index_type(IndexType::UInt8);

    const float raw_vertex_data[9] =
    {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    std::vector<uint8_t> vertex_data(sizeof(float)* 9, 0);
    std::memcpy(&vertex_data[0], raw_vertex_data, sizeof(float)* 9);

    mesh.set_vertex_data(vertex_data);

    REQUIRE(mesh.vertex_count() == 3u);
}

TEST_CASE("Get the vertex data of a mesh", "[Mesh]")
{
    VertexLayout vertex_layout;
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float32, 3));

    Mesh mesh("Test");
    mesh.set_vertex_layout(vertex_layout);
    mesh.set_primitive_type(PrimitiveType::Triangles);
    mesh.set_index_type(IndexType::UInt8);

    const float raw_original_vertex_data[9] =
    {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    std::vector<uint8_t> original_vertex_data(sizeof(float)* 9, 0);
    std::memcpy(&original_vertex_data[0], raw_original_vertex_data, sizeof(float)* 9);

    mesh.set_vertex_data(original_vertex_data);

    const float* vertex_data = (const float*)&mesh.vertex_data()[0];

    REQUIRE(vertex_data[0] == 1.0f);
    REQUIRE(vertex_data[1] == 0.0f);
    REQUIRE(vertex_data[2] == 0.0f);
    REQUIRE(vertex_data[3] == 0.0f);
    REQUIRE(vertex_data[4] == 1.0f);
    REQUIRE(vertex_data[5] == 0.0f);
    REQUIRE(vertex_data[6] == 0.0f);
    REQUIRE(vertex_data[7] == 0.0f);
    REQUIRE(vertex_data[8] == 1.0f);
}

TEST_CASE("Set the index data of a mesh", "[Mesh]")
{
    VertexLayout vertex_layout;
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float32, 3));

    Mesh mesh("Test");
    mesh.set_vertex_layout(vertex_layout);
    mesh.set_primitive_type(PrimitiveType::Triangles);
    mesh.set_index_type(IndexType::UInt8);

    std::vector<uint8_t> index_data;
    index_data.push_back(0);
    index_data.push_back(1);
    index_data.push_back(2);
    index_data.push_back(2);
    index_data.push_back(3);
    index_data.push_back(0);

    mesh.set_index_data(index_data);

    REQUIRE(mesh.index_count() == 6u);
}

TEST_CASE("Get the index data of a mesh", "[Mesh]")
{
    VertexLayout vertex_layout;
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float32, 3));

    Mesh mesh("Test");
    mesh.set_vertex_layout(vertex_layout);
    mesh.set_primitive_type(PrimitiveType::Triangles);
    mesh.set_index_type(IndexType::UInt8);

    std::vector<uint8_t> original_index_data;
    original_index_data.push_back(0);
    original_index_data.push_back(1);
    original_index_data.push_back(2);
    original_index_data.push_back(2);
    original_index_data.push_back(3);
    original_index_data.push_back(0);

    mesh.set_index_data(original_index_data);
    REQUIRE(mesh.index_count() == 6u);

    const uint8_t* index_data = &mesh.index_data()[0];

    REQUIRE(index_data[0] == 0);
    REQUIRE(index_data[1] == 1);
    REQUIRE(index_data[2] == 2);
    REQUIRE(index_data[3] == 2);
    REQUIRE(index_data[4] == 3);
    REQUIRE(index_data[5] == 0);
}
