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

Texture2 createTestTexture2()
{
    const unsigned width = 4;
    const unsigned height = 8;

    Texture2 texture("Test", width, height);
    for (unsigned x = 0; x < width; ++x)
    {
        for (unsigned y = 0; y < height; ++y)
        {
            Color color(static_cast<double>(x) / width, static_cast<double>(y) / height, 1.0);
            texture.image().writePixel(x, y, color);
        }
    }

    return texture;
}

Texture3 createTestTexture3()
{
    const unsigned width = 4;
    const unsigned height = 8;
    const unsigned depth = 2;

    Texture3 texture("Test", width, height, depth);
    for (unsigned x = 0; x < width; ++x)
    {
        for (unsigned y = 0; y < height; ++y)
        {
            for (unsigned z = 0; z < depth; ++z)
            {
                Color color(static_cast<double>(x) / width, static_cast<double>(y) / height, static_cast<double>(z) / depth);
                texture.image(z).writePixel(x, y, color);
            }
        }
    }

    return texture;
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

TEST_CASE("Upload and download a 2-dimensional texture", "[Renderer]")
{
    Engine& engine = Engine::instance();
    Renderer& renderer = engine.renderer();

    Texture2 texture = createTestTexture2();
    REQUIRE(!texture.isUploaded());

    // Remember the bytes that were uploaded
    ByteVector uploadedPixelData = texture.image().pixelData();

    // Upload the texture
    renderer.uploadTexture(texture);
    REQUIRE(texture.isUploaded());

    // Get the downloaded pixel data
    texture.invalidateLocalImage();
    ByteVector downloadedPixelData = texture.image().pixelData();

    // Verify that the downloaded pixel data is equal to the uploaded pixel data
    REQUIRE(uploadedPixelData.size() == downloadedPixelData.size());
    for (size_t i = 0; i < uploadedPixelData.size(); ++i)
    {
        REQUIRE(uploadedPixelData[i] == downloadedPixelData[i]);
    }
}

TEST_CASE("Upload and download a 3-dimensional texture", "[Renderer]")
{
    Engine& engine = Engine::instance();
    Renderer& renderer = engine.renderer();

    Texture3 texture = createTestTexture3();
    REQUIRE(!texture.isUploaded());

    // Remember the bytes that were uploaded
    std::vector<ByteVector> uploadedPixelData;
    for (unsigned z = 0; z < texture.depth(); ++z)
    {
        uploadedPixelData.push_back(texture.image(z).pixelData());
    }

    // Upload the texture
    renderer.uploadTexture(texture);
    REQUIRE(texture.isUploaded());

    // Get the downloaded pixel data
    texture.invalidateLocalImages();
    std::vector<ByteVector> downloadedPixelData;
    for (unsigned z = 0; z < texture.depth(); ++z)
    {
        downloadedPixelData.push_back(texture.image(z).pixelData());
    }

    // Verify that the downloaded pixel data is equal to the uploaded pixel data
    REQUIRE(uploadedPixelData.size() == downloadedPixelData.size());
    for (size_t i = 0; i < uploadedPixelData.size(); ++i)
    {
        REQUIRE(uploadedPixelData[i].size() == downloadedPixelData[i].size());
        for (size_t j = 0; j < uploadedPixelData[i].size(); ++j)
        {
            REQUIRE(uploadedPixelData[i][j] == downloadedPixelData[i][j]);
        }
    }
}

TEST_CASE("Attach a 3-dimensional texture to a frame buffer", "[Renderer]")
{
    Engine& engine = Engine::instance();
    Renderer& renderer = engine.renderer();

    FrameBuffer frameBuffer(128, 128);

    Texture3 texture("Test", 128, 128, 64);
    frameBuffer.attach(FrameBufferSlot::Color0, texture);

    Renderer::Frame frame = renderer.beginFrame(frameBuffer);
}
