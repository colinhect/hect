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
#include <Hect/Graphics/VertexLayout.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("VertexLayout_DefaultConstructor")
{
    VertexLayout vertexLayout;
    REQUIRE(vertexLayout.attributeCount() == 0u);
}

TEST_CASE("VertexLayout_addAttribute")
{
    VertexLayout vertexLayout;
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic_Position, VertexAttributeType_Float32, 3));
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic_Normal, VertexAttributeType_Float16, 3));

    REQUIRE(vertexLayout.attributeCount() == 2u);
}

TEST_CASE("VertexLayout_AttributeIndexing")
{
    VertexLayout vertexLayout;
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic_Position, VertexAttributeType_Float32, 3));
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic_Normal, VertexAttributeType_Float16, 3));

    REQUIRE(VertexAttributeSemantic_Position == (vertexLayout.attributes().begin() + 0)->semantic());
    REQUIRE(VertexAttributeSemantic_Normal == (vertexLayout.attributes().begin() + 1)->semantic());
}

TEST_CASE("VertexLayout_AttributeOffsets")
{
    VertexLayout vertexLayout;
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic_Position, VertexAttributeType_Float32, 3));
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic_Normal, VertexAttributeType_Float16, 3));

    REQUIRE((vertexLayout.attributes().begin() + 0)->offset() == 0u);
    REQUIRE((vertexLayout.attributes().begin() + 1)->offset() == (vertexLayout.attributes().begin() + 0)->size());
}

TEST_CASE("VertexLayout_AttributeWithSemantic")
{
    VertexLayout vertexLayout;
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic_Position, VertexAttributeType_Float32, 4));
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic_Normal, VertexAttributeType_Float16, 3));

    REQUIRE(vertexLayout.attributeWithSemantic(VertexAttributeSemantic_Position).cardinality() == 4u);
    REQUIRE(vertexLayout.attributeWithSemantic(VertexAttributeSemantic_Normal).cardinality() == 3u);
}

TEST_CASE("VertexLayout_HasAttributeWithSemantic")
{
    VertexLayout vertexLayout;
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic_Position, VertexAttributeType_Float32, 4));
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic_Normal, VertexAttributeType_Float16, 3));

    REQUIRE(vertexLayout.hasAttributeWithSemantic(VertexAttributeSemantic_Position));
}

TEST_CASE("VertexLayout_HasAttributeWithSemanticNegative")
{
    VertexLayout vertexLayout;
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic_Position, VertexAttributeType_Float32, 3));
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic_Normal, VertexAttributeType_Float16, 3));

    REQUIRE(!vertexLayout.hasAttributeWithSemantic(VertexAttributeSemantic_Color));
}

TEST_CASE("VertexLayout_VertexSize")
{
    VertexLayout vertexLayout;
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic_Position, VertexAttributeType_Float32, 3));
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic_Normal, VertexAttributeType_Float16, 3));

    REQUIRE(vertexLayout.vertexSize() == 4u * 3u + 2u * 3u);
}