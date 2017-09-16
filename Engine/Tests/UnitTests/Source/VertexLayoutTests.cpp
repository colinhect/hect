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
#include <Hect/Graphics/VertexLayout.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Constrct an empty vertex layout", "[VertexLayout]")
{
    VertexLayout vertex_layout;
    REQUIRE(vertex_layout.attribute_count() == 0u);
}

TEST_CASE("Add attributes to a vertex layout", "[VertexLayout]")
{
    VertexLayout vertex_layout;
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float32, 3));
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Normal, VertexAttributeType::Int16, 3));

    REQUIRE(vertex_layout.attribute_count() == 2u);
}

TEST_CASE("Index attributes in a vertex layout", "[VertexLayout]")
{
    VertexLayout vertex_layout;
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float32, 3));
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Normal, VertexAttributeType::Int16, 3));

    REQUIRE(VertexAttributeSemantic::Position == vertex_layout.attributes()[0].semantic());
    REQUIRE(VertexAttributeSemantic::Normal == vertex_layout.attributes()[1].semantic());
}

TEST_CASE("Get the offsets of the attributes in a vertex layout", "[VertexLayout]")
{
    VertexLayout vertex_layout;
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float32, 3));
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Normal, VertexAttributeType::Int16, 3));

    REQUIRE(vertex_layout.attributes()[0].offset() == 0u);
    REQUIRE(vertex_layout.attributes()[1].offset() == vertex_layout.attributes()[0].size());
}

TEST_CASE("Get the attribute of a specified semantic in a vertex layout", "[VertexLayout]")
{
    VertexLayout vertex_layout;
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float32, 4));
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Normal, VertexAttributeType::Int16, 3));

    REQUIRE(vertex_layout.attribute_with_semantic(VertexAttributeSemantic::Position).cardinality() == 4u);
    REQUIRE(vertex_layout.attribute_with_semantic(VertexAttributeSemantic::Normal).cardinality() == 3u);
}

TEST_CASE("Check if a vertex layout has an attribute with a specified semantic", "[VertexLayout]")
{
    VertexLayout vertex_layout;
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float32, 4));
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Normal, VertexAttributeType::Int16, 3));

    REQUIRE(vertex_layout.has_attribute_with_semantic(VertexAttributeSemantic::Position));
}

TEST_CASE("Check if a vertex layout does not have an attribute with a specified semantic", "[VertexLayout]")
{
    VertexLayout vertex_layout;
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float32, 3));
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Normal, VertexAttributeType::Int16, 3));

    REQUIRE(!vertex_layout.has_attribute_with_semantic(VertexAttributeSemantic::Color));
}

TEST_CASE("Get the vertex size of a vertex layout", "[VertexLayout]")
{
    VertexLayout vertex_layout;
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float32, 3));
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Normal, VertexAttributeType::Int16, 3));

    REQUIRE(vertex_layout.vertex_size() == 4u * 3u + 2u * 3u);
}