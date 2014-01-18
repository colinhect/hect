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

VertexLayout createVetexLayout()
{
    VertexAttribute::Array attributes;
    attributes.push_back(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float, 3));
    attributes.push_back(VertexAttribute(VertexAttributeSemantic::Normal, VertexAttributeType::Float, 3));

    return VertexLayout(attributes);
}

SUITE(MeshWriter)
{
    TEST(EmptyMesh)
    {
        Mesh mesh("Test", createVetexLayout(), PrimitiveType::Triangles, IndexType::UnsignedByte);
        MeshWriter meshWriter(mesh);

        CHECK_EQUAL(0u, mesh.vertexCount());
        CHECK_EQUAL(0u, mesh.indexCount());
    }

    TEST(AddVertexWithoutAttributeData)
    {
        Mesh mesh("Test", createVetexLayout(), PrimitiveType::Triangles, IndexType::UnsignedByte);
        MeshWriter meshWriter(mesh);
        meshWriter.addVertex();

        CHECK_EQUAL(1u, mesh.vertexCount());
        CHECK_EQUAL(0u, mesh.indexCount());

        const float* vertexData = (const float*)&mesh.vertexData()[0];
        CHECK_EQUAL(0.0f, vertexData[ 0]);
        CHECK_EQUAL(0.0f, vertexData[ 1]);
        CHECK_EQUAL(0.0f, vertexData[ 2]);
        CHECK_EQUAL(0.0f, vertexData[ 3]);
        CHECK_EQUAL(0.0f, vertexData[ 4]);
        CHECK_EQUAL(0.0f, vertexData[ 5]);
    }

    TEST(SetAttributeDataOfSameTypeAndCardinality)
    {
        Mesh mesh("Test", createVetexLayout(), PrimitiveType::Triangles, IndexType::UnsignedByte);
        MeshWriter meshWriter(mesh);
        meshWriter.addVertex();
        meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(1.0, 2.0, 3.0));

        CHECK_EQUAL(1u, mesh.vertexCount());
        CHECK_EQUAL(0u, mesh.indexCount());

        const float* vertexData = (const float*)&mesh.vertexData()[0];
        CHECK_EQUAL(1.0f, vertexData[ 0]);
        CHECK_EQUAL(2.0f, vertexData[ 1]);
        CHECK_EQUAL(3.0f, vertexData[ 2]);
        CHECK_EQUAL(0.0f, vertexData[ 3]);
        CHECK_EQUAL(0.0f, vertexData[ 4]);
        CHECK_EQUAL(0.0f, vertexData[ 5]);
    }

    TEST(SetAttributeDataOfSameTypeAndLarger)
    {
        Mesh mesh("Test", createVetexLayout(), PrimitiveType::Triangles, IndexType::UnsignedByte);
        MeshWriter meshWriter(mesh);
        meshWriter.addVertex();
        meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector4(1.0, 2.0, 3.0, 4.0));

        CHECK_EQUAL(1u, mesh.vertexCount());
        CHECK_EQUAL(0u, mesh.indexCount());

        const float* vertexData = (const float*)&mesh.vertexData()[0];
        CHECK_EQUAL(1.0f, vertexData[ 0]);
        CHECK_EQUAL(2.0f, vertexData[ 1]);
        CHECK_EQUAL(3.0f, vertexData[ 2]);
        CHECK_EQUAL(0.0f, vertexData[ 3]);
        CHECK_EQUAL(0.0f, vertexData[ 4]);
        CHECK_EQUAL(0.0f, vertexData[ 5]);
    }

    TEST(SetAttributeDataOfSameTypeAndSmaller)
    {
        Mesh mesh("Test", createVetexLayout(), PrimitiveType::Triangles, IndexType::UnsignedByte);
        MeshWriter meshWriter(mesh);
        meshWriter.addVertex();
        meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector2(1.0, 2.0));

        CHECK_EQUAL(1u, mesh.vertexCount());
        CHECK_EQUAL(0u, mesh.indexCount());

        const float* vertexData = (const float*)&mesh.vertexData()[0];
        CHECK_EQUAL(1.0f, vertexData[ 0]);
        CHECK_EQUAL(2.0f, vertexData[ 1]);
        CHECK_EQUAL(0.0f, vertexData[ 2]);
        CHECK_EQUAL(0.0f, vertexData[ 3]);
        CHECK_EQUAL(0.0f, vertexData[ 4]);
        CHECK_EQUAL(0.0f, vertexData[ 5]);
    }

    TEST(SimpleMesh)
    {
        Mesh mesh("Test", createVetexLayout(), PrimitiveType::Triangles, IndexType::UnsignedByte);
        MeshWriter meshWriter(mesh);
        meshWriter.addVertex();
        meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(1.0, 2.0, 3.0));
        meshWriter.writeAttributeData(VertexAttributeSemantic::Normal, Vector3(-1.0, -2.0, -3.0));
        meshWriter.addVertex();
        meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(4.0, 5.0, 6.0));
        meshWriter.writeAttributeData(VertexAttributeSemantic::Normal, Vector3(-4.0, -5.0, -6.0));
        meshWriter.addVertex();
        meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(7.0, 8.0, 9.0));
        meshWriter.writeAttributeData(VertexAttributeSemantic::Normal, Vector3(-7.0, -8.0, -9.0));

        meshWriter.addIndex(0);
        meshWriter.addIndex(1);
        meshWriter.addIndex(2);
        meshWriter.addIndex(0);

        CHECK_EQUAL(3u, mesh.vertexCount());
        CHECK_EQUAL(4u, mesh.indexCount());

        const float* vertexData = (const float*)&mesh.vertexData()[0];
        CHECK_EQUAL( 1.0f, vertexData[ 0]);
        CHECK_EQUAL( 2.0f, vertexData[ 1]);
        CHECK_EQUAL( 3.0f, vertexData[ 2]);
        CHECK_EQUAL(-1.0f, vertexData[ 3]);
        CHECK_EQUAL(-2.0f, vertexData[ 4]);
        CHECK_EQUAL(-3.0f, vertexData[ 5]);
        CHECK_EQUAL( 4.0f, vertexData[ 6]);
        CHECK_EQUAL( 5.0f, vertexData[ 7]);
        CHECK_EQUAL( 6.0f, vertexData[ 8]);
        CHECK_EQUAL(-4.0f, vertexData[ 9]);
        CHECK_EQUAL(-5.0f, vertexData[10]);
        CHECK_EQUAL(-6.0f, vertexData[11]);
        CHECK_EQUAL( 7.0f, vertexData[12]);
        CHECK_EQUAL( 8.0f, vertexData[13]);
        CHECK_EQUAL( 9.0f, vertexData[14]);
        CHECK_EQUAL(-7.0f, vertexData[15]);
        CHECK_EQUAL(-8.0f, vertexData[16]);
        CHECK_EQUAL(-9.0f, vertexData[17]);

        const uint8_t* indexData = (const uint8_t*)&mesh.indexData()[0];
        CHECK_EQUAL(0, indexData[0]);
        CHECK_EQUAL(1, indexData[1]);
        CHECK_EQUAL(2, indexData[2]);
        CHECK_EQUAL(0, indexData[3]);
    }
}