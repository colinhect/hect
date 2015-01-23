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

extern Engine* engine;

TEST_CASE("Upload and destroy render object", "[Renderer]")
{
    AssetCache& assetCache = engine->assetCache();
    Renderer& renderer = engine->renderer();

    Mesh mesh = assetCache.get<Mesh>("Hect/Box.mesh");
    REQUIRE(!mesh.isUploaded());
    renderer.uploadMesh(mesh);
    REQUIRE(mesh.isUploaded());
    renderer.destroyMesh(mesh);
    REQUIRE(!mesh.isUploaded());
}

TEST_CASE("Re-upload destroyed render object", "[Renderer]")
{
    AssetCache& assetCache = engine->assetCache();
    Renderer& renderer = engine->renderer();

    Mesh mesh = assetCache.get<Mesh>("Hect/Box.mesh");
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
    AssetCache& assetCache = engine->assetCache();
    Renderer& renderer = engine->renderer();

    Mesh mesh = assetCache.get<Mesh>("Hect/Box.mesh");
    renderer.uploadMesh(mesh);
    REQUIRE(mesh.isUploaded());

    Mesh meshCopy(mesh);
    REQUIRE(!meshCopy.isUploaded());
    REQUIRE(mesh.isUploaded());

    renderer.destroyMesh(mesh);
    REQUIRE(!mesh.isUploaded());

    REQUIRE(!meshCopy.isUploaded());
}

// Disabled due to Visual Studio 2013 compiler bug (issue #111)
#ifndef HECT_WINDOWS_BUILD

TEST_CASE("Move uploaded render object", "[Renderer]")
{
    AssetCache& assetCache = engine->assetCache();
    Renderer& renderer = engine->renderer();

    Mesh mesh = assetCache.get<Mesh>("Hect/Box.mesh");
    renderer.uploadMesh(mesh);
    REQUIRE(mesh.isUploaded());

    Mesh meshMove(std::move(mesh));
    REQUIRE(meshMove.isUploaded());
    REQUIRE(!mesh.isUploaded());

    renderer.destroyMesh(meshMove);
    REQUIRE(!meshMove.isUploaded());
}

#endif
