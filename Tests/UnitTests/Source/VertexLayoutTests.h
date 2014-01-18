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
#pragma once

SUITE(VertexLayout)
{
    TEST(DefaultConstructor)
    {
        VertexLayout vertexLayout;
        CHECK_EQUAL(0u, vertexLayout.attributes().size());
    }

    TEST(AttributeConstructor)
    {
        VertexAttribute::Array attributes;
        attributes.push_back(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float, 3));
        attributes.push_back(VertexAttribute(VertexAttributeSemantic::Normal, VertexAttributeType::Half, 3));

        VertexLayout vertexLayout(attributes);
        CHECK_EQUAL(2u, vertexLayout.attributes().size());
    }

    TEST(AttributeIndexing)
    {
        VertexAttribute::Array attributes;
        attributes.push_back(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float, 3));
        attributes.push_back(VertexAttribute(VertexAttributeSemantic::Normal, VertexAttributeType::Half, 3));

        VertexLayout vertexLayout(attributes);

        CHECK(VertexAttributeSemantic::Position == vertexLayout.attributes()[0].semantic());
        CHECK(VertexAttributeSemantic::Normal == vertexLayout.attributes()[1].semantic());
    }

    TEST(AttributeOffsets)
    {
        VertexAttribute::Array attributes;
        attributes.push_back(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float, 3));
        attributes.push_back(VertexAttribute(VertexAttributeSemantic::Normal, VertexAttributeType::Half, 3));

        VertexLayout vertexLayout(attributes);

        CHECK_EQUAL(0u, vertexLayout.attributes()[0].offset());
        CHECK_EQUAL(vertexLayout.attributes()[0].size(), vertexLayout.attributes()[1].offset());
    }

    TEST(AttributeWithSemantic)
    {
        VertexAttribute::Array attributes;
        attributes.push_back(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float, 4));
        attributes.push_back(VertexAttribute(VertexAttributeSemantic::Normal, VertexAttributeType::Half, 3));

        VertexLayout vertexLayout(attributes);

        CHECK_EQUAL(4u, vertexLayout.attributeWithSemantic(VertexAttributeSemantic::Position)->cardinality());
        CHECK_EQUAL(3u, vertexLayout.attributeWithSemantic(VertexAttributeSemantic::Normal)->cardinality());
    }

    TEST(AttributeWithSemanticNegative)
    {
        VertexAttribute::Array attributes;
        attributes.push_back(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float, 3));
        attributes.push_back(VertexAttribute(VertexAttributeSemantic::Normal, VertexAttributeType::Half, 3));

        VertexLayout vertexLayout(attributes);

        CHECK(!vertexLayout.attributeWithSemantic(VertexAttributeSemantic::Color));
    }

    TEST(VertexSize)
    {
        VertexAttribute::Array attributes;
        attributes.push_back(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float, 3));
        attributes.push_back(VertexAttribute(VertexAttributeSemantic::Normal, VertexAttributeType::Half, 3));

        VertexLayout vertexLayout(attributes);

        CHECK_EQUAL(4u * 3u + 2u * 3u, vertexLayout.vertexSize());
    }
}