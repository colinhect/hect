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

    const VertexLayout& meshVertexLayout = mesh.vertexLayout();
    REQUIRE(VertexAttributeSemantic::Position == meshVertexLayout.attributes().begin()->semantic());
    REQUIRE(PrimitiveType::Triangles == mesh.primitiveType());
    REQUIRE(IndexType::UInt16 == mesh.indexType());
}

TEST_CASE("Construct mesh with vertex layout, primitive type, and index type", "[Mesh]")
{
    VertexLayout vertexLayout;
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float32, 3));
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic::Normal, VertexAttributeType::Int16, 3));

    Mesh mesh("Test");
    mesh.setVertexLayout(vertexLayout);
    mesh.setPrimitiveType(PrimitiveType::Triangles);
    mesh.setIndexType(IndexType::UInt8);

    const VertexLayout& meshVertexLayout = mesh.vertexLayout();
    REQUIRE(meshVertexLayout.attributeCount() == 2u);
    REQUIRE(VertexAttributeSemantic::Position == meshVertexLayout.attributes().begin()->semantic());
    REQUIRE(VertexAttributeType::Float32 == meshVertexLayout.attributes().begin()->type());
    REQUIRE(meshVertexLayout.attributes().begin()->cardinality() == 3u);

    REQUIRE(PrimitiveType::Triangles == mesh.primitiveType());
    REQUIRE(IndexType::UInt8 == mesh.indexType());
}

TEST_CASE("Set the vertex data of a mesh", "[Mesh]")
{
    VertexLayout vertexLayout;
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float32, 3));

    Mesh mesh("Test");
    mesh.setVertexLayout(vertexLayout);
    mesh.setPrimitiveType(PrimitiveType::Triangles);
    mesh.setIndexType(IndexType::UInt8);

    const float rawVertexData[9] =
    {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    std::vector<uint8_t> vertexData(sizeof(float)* 9, 0);
    std::memcpy(&vertexData[0], rawVertexData, sizeof(float)* 9);

    mesh.setVertexData(vertexData);

    REQUIRE(mesh.vertexCount() == 3u);
}

TEST_CASE("Get the vertex data of a mesh", "[Mesh]")
{
    VertexLayout vertexLayout;
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float32, 3));

    Mesh mesh("Test");
    mesh.setVertexLayout(vertexLayout);
    mesh.setPrimitiveType(PrimitiveType::Triangles);
    mesh.setIndexType(IndexType::UInt8);

    const float rawOriginalVertexData[9] =
    {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    std::vector<uint8_t> originalVertexData(sizeof(float)* 9, 0);
    std::memcpy(&originalVertexData[0], rawOriginalVertexData, sizeof(float)* 9);

    mesh.setVertexData(originalVertexData);

    const float* vertexData = (const float*)&mesh.vertexData()[0];

    REQUIRE(vertexData[0] == 1.0f);
    REQUIRE(vertexData[1] == 0.0f);
    REQUIRE(vertexData[2] == 0.0f);
    REQUIRE(vertexData[3] == 0.0f);
    REQUIRE(vertexData[4] == 1.0f);
    REQUIRE(vertexData[5] == 0.0f);
    REQUIRE(vertexData[6] == 0.0f);
    REQUIRE(vertexData[7] == 0.0f);
    REQUIRE(vertexData[8] == 1.0f);
}

TEST_CASE("Set the index data of a mesh", "[Mesh]")
{
    VertexLayout vertexLayout;
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float32, 3));

    Mesh mesh("Test");
    mesh.setVertexLayout(vertexLayout);
    mesh.setPrimitiveType(PrimitiveType::Triangles);
    mesh.setIndexType(IndexType::UInt8);

    std::vector<uint8_t> indexData;
    indexData.push_back(0);
    indexData.push_back(1);
    indexData.push_back(2);
    indexData.push_back(2);
    indexData.push_back(3);
    indexData.push_back(0);

    mesh.setIndexData(indexData);

    REQUIRE(mesh.indexCount() == 6u);
}

TEST_CASE("Get the index data of a mesh", "[Mesh]")
{
    VertexLayout vertexLayout;
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float32, 3));

    Mesh mesh("Test");
    mesh.setVertexLayout(vertexLayout);
    mesh.setPrimitiveType(PrimitiveType::Triangles);
    mesh.setIndexType(IndexType::UInt8);

    std::vector<uint8_t> originalIndexData;
    originalIndexData.push_back(0);
    originalIndexData.push_back(1);
    originalIndexData.push_back(2);
    originalIndexData.push_back(2);
    originalIndexData.push_back(3);
    originalIndexData.push_back(0);

    mesh.setIndexData(originalIndexData);
    REQUIRE(mesh.indexCount() == 6u);

    const uint8_t* indexData = &mesh.indexData()[0];

    REQUIRE(indexData[0] == 0);
    REQUIRE(indexData[1] == 1);
    REQUIRE(indexData[2] == 2);
    REQUIRE(indexData[3] == 2);
    REQUIRE(indexData[4] == 3);
    REQUIRE(indexData[5] == 0);
}