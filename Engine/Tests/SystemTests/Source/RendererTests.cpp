///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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

Mesh createTestMesh()
{
    Mesh mesh("Test");

    MeshWriter meshWriter(mesh);
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3::UnitX);
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3::UnitY);
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3::UnitZ);
    meshWriter.addIndex(0);
    meshWriter.addIndex(1);
    meshWriter.addIndex(2);

    return mesh;
}

TEST_CASE("Upload and destroy render object", "[Renderer]")
{
    Engine& engine = Engine::instance();
    Renderer& renderer = engine.renderer();

    Mesh mesh = createTestMesh();
    REQUIRE(!mesh.isUploaded());
    renderer.uploadMesh(mesh);
    REQUIRE(mesh.isUploaded());
    renderer.destroyMesh(mesh);
    REQUIRE(!mesh.isUploaded());
}

TEST_CASE("Re-upload destroyed render object", "[Renderer]")
{
    Engine& engine = Engine::instance();
    Renderer& renderer = engine.renderer();

    Mesh mesh = createTestMesh();
    REQUIRE(!mesh.isUploaded());
    renderer.uploadMesh(mesh);
    REQUIRE(mesh.isUploaded());
    renderer.destroyMesh(mesh);
    REQUIRE(!mesh.isUploaded());
    renderer.uploadMesh(mesh);
    REQUIRE(mesh.isUploaded());
}

TEST_CASE("Copy uploaded render object", "[Renderer]")
{
    Engine& engine = Engine::instance();
    Renderer& renderer = engine.renderer();

    Mesh mesh = createTestMesh();
    renderer.uploadMesh(mesh);
    REQUIRE(mesh.isUploaded());

    Mesh meshCopy(mesh);
    REQUIRE(!meshCopy.isUploaded());
    REQUIRE(mesh.isUploaded());

    renderer.destroyMesh(mesh);
    REQUIRE(!mesh.isUploaded());

    REQUIRE(!meshCopy.isUploaded());
}

TEST_CASE("Move uploaded render object", "[Renderer]")
{
    Engine& engine = Engine::instance();
    Renderer& renderer = engine.renderer();

    Mesh mesh = createTestMesh();
    renderer.uploadMesh(mesh);
    REQUIRE(mesh.isUploaded());

    Mesh meshMove(std::move(mesh));
    REQUIRE(meshMove.isUploaded());
    REQUIRE(!mesh.isUploaded());

    renderer.destroyMesh(meshMove);
    REQUIRE(!meshMove.isUploaded());
}
