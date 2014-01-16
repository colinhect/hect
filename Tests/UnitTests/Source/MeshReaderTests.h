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
SUITE(MeshReader)
{
    TEST(EmptyMesh)
    {
        Mesh mesh("Test", createVetexLayout(), PrimitiveType::Triangles, IndexType::UnsignedByte);
        MeshReader meshReader(mesh);

        CHECK(!meshReader.nextVertex());
        CHECK(!meshReader.nextIndex());
    }

    TEST(ReadAttributeVector3)
    {
        Mesh mesh("Test", createVetexLayout(), PrimitiveType::Triangles, IndexType::UnsignedByte);

        {
            MeshWriter meshWriter(mesh);
            meshWriter.addVertex();
            meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3<>(1, 2, 3));
            meshWriter.writeAttributeData(VertexAttributeSemantic::Normal, Vector3<>(4, 5, 6));
            meshWriter.addVertex();
            meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3<>(7, 8, 9));
            meshWriter.writeAttributeData(VertexAttributeSemantic::Normal, Vector3<>(10, 11, 12));
            meshWriter.addVertex();
            meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3<>(13, 14, 15));
            meshWriter.writeAttributeData(VertexAttributeSemantic::Normal, Vector3<>(16, 17, 18));
        }

        Vector3<> value;
        MeshReader meshReader(mesh);

        CHECK(meshReader.nextVertex());

        value = meshReader.readAttributeVector3(VertexAttributeSemantic::Position);
        CHECK_EQUAL(1, value.x);
        CHECK_EQUAL(2, value.y);
        CHECK_EQUAL(3, value.z);

        value = meshReader.readAttributeVector3(VertexAttributeSemantic::Normal);
        CHECK_EQUAL(4, value.x);
        CHECK_EQUAL(5, value.y);
        CHECK_EQUAL(6, value.z);

        CHECK(meshReader.nextVertex());

        value = meshReader.readAttributeVector3(VertexAttributeSemantic::Position);
        CHECK_EQUAL(7, value.x);
        CHECK_EQUAL(8, value.y);
        CHECK_EQUAL(9, value.z);

        value = meshReader.readAttributeVector3(VertexAttributeSemantic::Normal);
        CHECK_EQUAL(10, value.x);
        CHECK_EQUAL(11, value.y);
        CHECK_EQUAL(12, value.z);

        CHECK(meshReader.nextVertex());

        value = meshReader.readAttributeVector3(VertexAttributeSemantic::Position);
        CHECK_EQUAL(13, value.x);
        CHECK_EQUAL(14, value.y);
        CHECK_EQUAL(15, value.z);

        value = meshReader.readAttributeVector3(VertexAttributeSemantic::Normal);
        CHECK_EQUAL(16, value.x);
        CHECK_EQUAL(17, value.y);
        CHECK_EQUAL(18, value.z);

        CHECK(!meshReader.nextVertex());
    }

    TEST(ReadIndexByte)
    {
        Mesh mesh("Test", createVetexLayout(), PrimitiveType::Triangles, IndexType::UnsignedByte);

        {
            MeshWriter meshWriter(mesh);
            meshWriter.addIndex(0);
            meshWriter.addIndex(1);
            meshWriter.addIndex(2);
        }

        Vector3<> value;
        MeshReader meshReader(mesh);

        CHECK(meshReader.nextIndex());
        CHECK_EQUAL(0, meshReader.readIndexByte());

        CHECK(meshReader.nextIndex());
        CHECK_EQUAL(1, meshReader.readIndexByte());

        CHECK(meshReader.nextIndex());
        CHECK_EQUAL(2, meshReader.readIndexByte());

        CHECK(!meshReader.nextIndex());
    }
}