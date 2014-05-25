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
#include <Hect.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("VertexAttribute_ConstructorAndAccessors")
{
    VertexAttribute attribute(VertexAttributeSemantic::Position, VertexAttributeType::Float, 3);

    REQUIRE(VertexAttributeSemantic::Position == attribute.semantic());
    REQUIRE(VertexAttributeType::Float == attribute.type());
    REQUIRE(attribute.cardinality() == 3u);
}

TEST_CASE("VertexAttribute_Size")
{
    VertexAttribute attribute(VertexAttributeSemantic::Position, VertexAttributeType::Half, 3);
    REQUIRE(attribute.size() == 2u * 3u);

    attribute = VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float, 3);
    REQUIRE(attribute.size() == 4u * 3u);

    attribute = VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float, 2);
    REQUIRE(attribute.size() == 4u * 2u);
}

TEST_CASE("VertexAttribute_Equality")
{
    VertexAttribute a(VertexAttributeSemantic::Position, VertexAttributeType::Half, 3);
    VertexAttribute b(VertexAttributeSemantic::Position, VertexAttributeType::Half, 3);
    VertexAttribute c(VertexAttributeSemantic::Position, VertexAttributeType::Float, 3);

    REQUIRE(a == b);
    REQUIRE(a != c);
}