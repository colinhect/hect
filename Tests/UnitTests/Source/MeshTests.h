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

SUITE(Mesh)
{
    TEST(DefaultConstructor)
    {
        Mesh mesh;

        const VertexLayout& meshVertexLayout = mesh.vertexLayout();
        CHECK(VertexAttributeSemantic::Position == meshVertexLayout.attributes()[0].semantic());
        CHECK(PrimitiveType::Triangles == mesh.primitiveType());
        CHECK(IndexType::UnsignedShort == mesh.indexType());
    }

    TEST(Constructor)
    {
        VertexAttribute::Array attributes;
        attributes.push_back(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float, 3));
        attributes.push_back(VertexAttribute(VertexAttributeSemantic::Normal, VertexAttributeType::Half, 3));

        VertexLayout vertexLayout(attributes);

        Mesh mesh("Test", vertexLayout, PrimitiveType::Triangles, IndexType::UnsignedByte);

        const VertexLayout& meshVertexLayout = mesh.vertexLayout();
        CHECK_EQUAL(2u, meshVertexLayout.attributes().size());
        CHECK(VertexAttributeSemantic::Position == meshVertexLayout.attributes()[0].semantic());
        CHECK(VertexAttributeType::Float == meshVertexLayout.attributes()[0].type());
        CHECK_EQUAL(3u, meshVertexLayout.attributes()[0].cardinality());

        CHECK(PrimitiveType::Triangles == mesh.primitiveType());
        CHECK(IndexType::UnsignedByte == mesh.indexType());
    }

    TEST(SetVertexData)
    {
        VertexAttribute::Array attributes;
        attributes.push_back(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float, 3));

        VertexLayout vertexLayout(attributes);

        Mesh mesh("Test", vertexLayout, PrimitiveType::Triangles, IndexType::UnsignedByte);

        const float rawVertexData[9] =
        {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f
        };
        std::vector<uint8_t> vertexData(sizeof(float) * 9, 0);
        std::memcpy(&vertexData[0], rawVertexData, sizeof(float) * 9);

        mesh.setVertexData(vertexData);

        CHECK_EQUAL(3u, mesh.vertexCount());
    }

    TEST(GetVertexData)
    {
        VertexAttribute::Array attributes;
        attributes.push_back(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float, 3));

        VertexLayout vertexLayout(attributes);

        Mesh mesh("Test", vertexLayout, PrimitiveType::Triangles, IndexType::UnsignedByte);

        const float rawOriginalVertexData[9] =
        {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f
        };
        std::vector<uint8_t> originalVertexData(sizeof(float) * 9, 0);
        std::memcpy(&originalVertexData[0], rawOriginalVertexData, sizeof(float) * 9);

        mesh.setVertexData(originalVertexData);

        const float* vertexData = (const float*)&mesh.vertexData()[0];

        CHECK_EQUAL(1.0f, vertexData[0]);
        CHECK_EQUAL(0.0f, vertexData[1]);
        CHECK_EQUAL(0.0f, vertexData[2]);
        CHECK_EQUAL(0.0f, vertexData[3]);
        CHECK_EQUAL(1.0f, vertexData[4]);
        CHECK_EQUAL(0.0f, vertexData[5]);
        CHECK_EQUAL(0.0f, vertexData[6]);
        CHECK_EQUAL(0.0f, vertexData[7]);
        CHECK_EQUAL(1.0f, vertexData[8]);
    }

    TEST(SetIndexData)
    {
        VertexAttribute::Array attributes;
        attributes.push_back(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float, 3));

        VertexLayout vertexLayout(attributes);

        Mesh mesh("Test", vertexLayout, PrimitiveType::Triangles, IndexType::UnsignedByte);

        std::vector<uint8_t> indexData;
        indexData.push_back(0);
        indexData.push_back(1);
        indexData.push_back(2);
        indexData.push_back(2);
        indexData.push_back(3);
        indexData.push_back(0);

        mesh.setIndexData(indexData);

        CHECK_EQUAL(6u, mesh.indexCount());
    }

    TEST(GetIndexDataUnsignedByte)
    {
        VertexAttribute::Array attributes;
        attributes.push_back(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float, 3));

        VertexLayout vertexLayout(attributes);

        Mesh mesh("Test", vertexLayout, PrimitiveType::Triangles, IndexType::UnsignedByte);

        std::vector<uint8_t> originalIndexData;
        originalIndexData.push_back(0);
        originalIndexData.push_back(1);
        originalIndexData.push_back(2);
        originalIndexData.push_back(2);
        originalIndexData.push_back(3);
        originalIndexData.push_back(0);

        mesh.setIndexData(originalIndexData);
        CHECK_EQUAL(6u, mesh.indexCount());

        const uint8_t* indexData = &mesh.indexData()[0];

        CHECK_EQUAL(0, indexData[0]);
        CHECK_EQUAL(1, indexData[1]);
        CHECK_EQUAL(2, indexData[2]);
        CHECK_EQUAL(2, indexData[3]);
        CHECK_EQUAL(3, indexData[4]);
        CHECK_EQUAL(0, indexData[5]);
    }
}