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

SUITE(MeshEncoder)
{
    TEST_FIXTURE(AssetCacheFixture, DefaultLayoutIndexAndPrimitiveType)
    {
        Mesh& mesh = assetCache.get<Mesh>("Meshes/DefaultLayoutIndexAndPrimitiveType.mesh");

        CHECK_EQUAL(4u, mesh.vertexCount());
        CHECK_EQUAL(6u, mesh.indexCount());
        CHECK_EQUAL(false, mesh.isUploaded());
        CHECK(IndexType::UnsignedShort == mesh.indexType());
        CHECK(PrimitiveType::Triangles == mesh.primitiveType());

        testEncodable(mesh, assetCache);
    }

    TEST_FIXTURE(AssetCacheFixture, IndexTypeAndNoData)
    {
        Mesh& mesh = assetCache.get<Mesh>("Meshes/IndexTypeAndNoData.mesh");

        CHECK(IndexType::UnsignedByte == mesh.indexType());

        testEncodable(mesh, assetCache);
    }

    TEST_FIXTURE(AssetCacheFixture, PrimitiveTypeAndNoData)
    {
        Mesh& mesh = assetCache.get<Mesh>("Meshes/PrimitiveTypeAndNoData.mesh");

        CHECK(PrimitiveType::Points == mesh.primitiveType());

        testEncodable(mesh, assetCache);
    }

    TEST_FIXTURE(AssetCacheFixture, VertexLayoutAndData)
    {
        Mesh& mesh = assetCache.get<Mesh>("Meshes/VertexLayoutAndData.mesh");

        CHECK(IndexType::UnsignedByte == mesh.indexType());
        CHECK(PrimitiveType::Lines == mesh.primitiveType());
        CHECK_EQUAL(2u, mesh.vertexLayout().attributes().size());
        CHECK_EQUAL(4u, mesh.vertexCount());
        CHECK_EQUAL(10u, mesh.indexCount());

        testEncodable(mesh, assetCache);
    }
}