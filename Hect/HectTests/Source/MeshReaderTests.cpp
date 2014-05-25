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

VertexLayout createVetexLayout()
{
    VertexLayout vertexLayout;
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float, 3));
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic::Normal, VertexAttributeType::Float, 3));

    return vertexLayout;
}

TEST_CASE("MeshReader_EmptyMesh")
{
    Mesh mesh("Test");
    mesh.setVertexLayout(createVetexLayout());
    mesh.setPrimitiveType(PrimitiveType::Triangles);
    mesh.setIndexType(IndexType::UnsignedByte);

    MeshReader meshReader(mesh);

    REQUIRE(!meshReader.nextVertex());
    REQUIRE(!meshReader.nextIndex());
}

TEST_CASE("MeshReader_ReadAttributeVector3")
{
    Mesh mesh("Test");
    mesh.setVertexLayout(createVetexLayout());
    mesh.setPrimitiveType(PrimitiveType::Triangles);
    mesh.setIndexType(IndexType::UnsignedByte);

    {
        MeshWriter meshWriter(mesh);
        meshWriter.addVertex();
        meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(1, 2, 3));
        meshWriter.writeAttributeData(VertexAttributeSemantic::Normal, Vector3(4, 5, 6));
        meshWriter.addVertex();
        meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(7, 8, 9));
        meshWriter.writeAttributeData(VertexAttributeSemantic::Normal, Vector3(10, 11, 12));
        meshWriter.addVertex();
        meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(13, 14, 15));
        meshWriter.writeAttributeData(VertexAttributeSemantic::Normal, Vector3(16, 17, 18));
    }

    Vector3 value;
    MeshReader meshReader(mesh);

    REQUIRE(meshReader.nextVertex());

    value = meshReader.readAttributeVector3(VertexAttributeSemantic::Position);
    REQUIRE(value.x == 1);
    REQUIRE(value.y == 2);
    REQUIRE(value.z == 3);

    value = meshReader.readAttributeVector3(VertexAttributeSemantic::Normal);
    REQUIRE(value.x == 4);
    REQUIRE(value.y == 5);
    REQUIRE(value.z == 6);

    REQUIRE(meshReader.nextVertex());

    value = meshReader.readAttributeVector3(VertexAttributeSemantic::Position);
    REQUIRE(value.x == 7);
    REQUIRE(value.y == 8);
    REQUIRE(value.z == 9);

    value = meshReader.readAttributeVector3(VertexAttributeSemantic::Normal);
    REQUIRE(value.x == 10);
    REQUIRE(value.y == 11);
    REQUIRE(value.z == 12);

    REQUIRE(meshReader.nextVertex());

    value = meshReader.readAttributeVector3(VertexAttributeSemantic::Position);
    REQUIRE(value.x == 13);
    REQUIRE(value.y == 14);
    REQUIRE(value.z == 15);

    value = meshReader.readAttributeVector3(VertexAttributeSemantic::Normal);
    REQUIRE(value.x == 16);
    REQUIRE(value.y == 17);
    REQUIRE(value.z == 18);

    REQUIRE(!meshReader.nextVertex());
}

TEST_CASE("MeshReader_ReadIndexByte")
{
    Mesh mesh("Test");
    mesh.setVertexLayout(createVetexLayout());
    mesh.setPrimitiveType(PrimitiveType::Triangles);
    mesh.setIndexType(IndexType::UnsignedByte);

    {
        MeshWriter meshWriter(mesh);
        meshWriter.addIndex(0);
        meshWriter.addIndex(1);
        meshWriter.addIndex(2);
    }

    Vector3 value;
    MeshReader meshReader(mesh);

    REQUIRE(meshReader.nextIndex());
    REQUIRE(meshReader.readIndexByte() == 0);

    REQUIRE(meshReader.nextIndex());
    REQUIRE(meshReader.readIndexByte() == 1);

    REQUIRE(meshReader.nextIndex());
    REQUIRE(meshReader.readIndexByte() == 2);

    REQUIRE(!meshReader.nextIndex());
}