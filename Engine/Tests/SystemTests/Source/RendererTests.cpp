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
#include <Hect.h>
using namespace hect;

#include <catch.hpp>

Mesh create_test_mesh()
{
    Mesh mesh("Test");

    MeshWriter mesh_writer(mesh);
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3::UnitX);
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3::UnitY);
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3::UnitZ);
    mesh_writer.add_index(0);
    mesh_writer.add_index(1);
    mesh_writer.add_index(2);

    return mesh;
}

Texture2 create_test_texture2()
{
    const unsigned width = 4;
    const unsigned height = 8;

    Texture2 texture("Test", width, height);
    for (unsigned x = 0; x < width; ++x)
    {
        for (unsigned y = 0; y < height; ++y)
        {
            Color color(static_cast<double>(x) / width, static_cast<double>(y) / height, 1.0);
            texture.image().write_pixel(x, y, color);
        }
    }

    return texture;
}

Texture3 create_test_texture3()
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
                texture.image(z).write_pixel(x, y, color);
            }
        }
    }

    return texture;
}

TEST_CASE("Upload and destroy render object", "[Renderer]")
{
    Engine& engine = Engine::instance();
    Renderer& renderer = engine.renderer();

    Mesh mesh = create_test_mesh();
    REQUIRE(!mesh.is_uploaded());
    renderer.upload_mesh(mesh);
    REQUIRE(mesh.is_uploaded());
    renderer.destroy_mesh(mesh);
    REQUIRE(!mesh.is_uploaded());
}

TEST_CASE("Re-upload destroyed render object", "[Renderer]")
{
    Engine& engine = Engine::instance();
    Renderer& renderer = engine.renderer();

    Mesh mesh = create_test_mesh();
    REQUIRE(!mesh.is_uploaded());
    renderer.upload_mesh(mesh);
    REQUIRE(mesh.is_uploaded());
    renderer.destroy_mesh(mesh);
    REQUIRE(!mesh.is_uploaded());
    renderer.upload_mesh(mesh);
    REQUIRE(mesh.is_uploaded());
}

TEST_CASE("Copy uploaded render object", "[Renderer]")
{
    Engine& engine = Engine::instance();
    Renderer& renderer = engine.renderer();

    Mesh mesh = create_test_mesh();
    renderer.upload_mesh(mesh);
    REQUIRE(mesh.is_uploaded());

    Mesh mesh_copy(mesh);
    REQUIRE(!mesh_copy.is_uploaded());
    REQUIRE(mesh.is_uploaded());

    renderer.destroy_mesh(mesh);
    REQUIRE(!mesh.is_uploaded());

    REQUIRE(!mesh_copy.is_uploaded());
}

TEST_CASE("Move uploaded render object", "[Renderer]")
{
    Engine& engine = Engine::instance();
    Renderer& renderer = engine.renderer();

    Mesh mesh = create_test_mesh();
    renderer.upload_mesh(mesh);
    REQUIRE(mesh.is_uploaded());

    Mesh mesh_move(std::move(mesh));
    REQUIRE(mesh_move.is_uploaded());
    REQUIRE(!mesh.is_uploaded());

    renderer.destroy_mesh(mesh_move);
    REQUIRE(!mesh_move.is_uploaded());
}

TEST_CASE("Upload and download a 2-dimensional texture", "[Renderer]")
{
    Engine& engine = Engine::instance();
    Renderer& renderer = engine.renderer();

    Texture2 texture = create_test_texture2();
    REQUIRE(!texture.is_uploaded());

    // Remember the bytes that were uploaded
    ByteVector uploaded_pixel_data = texture.image().pixel_data();

    // Upload the texture
    renderer.upload_texture(texture);
    REQUIRE(texture.is_uploaded());

    // Get the downloaded pixel data
    texture.invalidate_local_image();
    ByteVector downloaded_pixel_data = texture.image().pixel_data();

    // Verify that the downloaded pixel data is equal to the uploaded pixel data
    REQUIRE(uploaded_pixel_data.size() == downloaded_pixel_data.size());
    for (size_t i = 0; i < uploaded_pixel_data.size(); ++i)
    {
        REQUIRE(uploaded_pixel_data[i] == downloaded_pixel_data[i]);
    }
}

TEST_CASE("Upload and download a 3-dimensional texture", "[Renderer]")
{
    Engine& engine = Engine::instance();
    Renderer& renderer = engine.renderer();

    Texture3 texture = create_test_texture3();
    REQUIRE(!texture.is_uploaded());

    // Remember the bytes that were uploaded
    std::vector<ByteVector> uploaded_pixel_data;
    for (unsigned z = 0; z < texture.depth(); ++z)
    {
        uploaded_pixel_data.push_back(texture.image(z).pixel_data());
    }

    // Upload the texture
    renderer.upload_texture(texture);
    REQUIRE(texture.is_uploaded());

    // Get the downloaded pixel data
    texture.invalidate_local_images();
    std::vector<ByteVector> downloaded_pixel_data;
    for (unsigned z = 0; z < texture.depth(); ++z)
    {
        downloaded_pixel_data.push_back(texture.image(z).pixel_data());
    }

    // Verify that the downloaded pixel data is equal to the uploaded pixel data
    REQUIRE(uploaded_pixel_data.size() == downloaded_pixel_data.size());
    for (size_t i = 0; i < uploaded_pixel_data.size(); ++i)
    {
        REQUIRE(uploaded_pixel_data[i].size() == downloaded_pixel_data[i].size());
        for (size_t j = 0; j < uploaded_pixel_data[i].size(); ++j)
        {
            REQUIRE(uploaded_pixel_data[i][j] == downloaded_pixel_data[i][j]);
        }
    }
}

TEST_CASE("Attach a 3-dimensional texture to a frame buffer", "[Renderer]")
{
    Engine& engine = Engine::instance();
    Renderer& renderer = engine.renderer();

    FrameBuffer frame_buffer(128, 128);

    Texture3 texture("Test", 128, 128, 64);
    frame_buffer.attach(FrameBufferSlot::Color0, texture);

    Renderer::Frame frame = renderer.begin_frame(frame_buffer);
}
