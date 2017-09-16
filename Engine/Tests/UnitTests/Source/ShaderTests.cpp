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
#include <Hect/Graphics/Shader.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Add modules to a shader and iterate over them", "[Shader]")
{
    Shader shader;
    shader.add_module(ShaderModule(ShaderModuleType::Vertex, "A", "..."));
    shader.add_module(ShaderModule(ShaderModuleType::Fragment, "B", "..."));

    REQUIRE(shader.modules()[0].name() == "A");
    REQUIRE(shader.modules()[1].name() == "B");
}

TEST_CASE("Add uniforms to a shader and iterate over them", "[Shader]")
{
    Shader shader;
    shader.add_uniform(Uniform("A", UniformType::Float));
    shader.add_uniform(Uniform("B", UniformType::Vector2));
    shader.add_uniform(Uniform("C", UniformType::Vector3));

    REQUIRE(shader.uniforms()[0].name() == "A");
    REQUIRE(shader.uniforms()[1].name() == "B");
    REQUIRE(shader.uniforms()[2].name() == "C");
}

TEST_CASE("Get an existing uniform from a shader by name", "[Shader]")
{
    Shader shader;
    shader.add_uniform(Uniform("A", UniformType::Float));

    REQUIRE(shader.uniform("A").name() == "A");
    REQUIRE(shader.uniform(std::string("A")).name() == "A");
}

TEST_CASE("Get a non-existing uniform from a shader by name", "[Shader]")
{
    Shader shader;
    shader.add_uniform(Uniform("A", UniformType::Float));

    REQUIRE_THROWS_AS(shader.uniform("B"), InvalidOperation);
    REQUIRE_THROWS_AS(shader.uniform(std::string("B")), InvalidOperation);
}

TEST_CASE("Get an existing uniform from a shader by index", "[Shader]")
{
    Shader shader;
    UniformIndex index = shader.add_uniform(Uniform("A", UniformType::Float));

    REQUIRE(shader.uniform(index).name() == "A");
}

TEST_CASE("Get a non-existing uniform from a shader by index", "[Shader]")
{
    Shader shader;
    shader.add_uniform(Uniform("A", UniformType::Float));

    REQUIRE_THROWS_AS(shader.uniform(1), InvalidOperation);
}

TEST_CASE("Get and set the render stage of a shader", "[Shader]")
{
    Shader shader;
    REQUIRE(shader.render_stage() == RenderStage::None);
    shader.set_render_stage(RenderStage::PhysicalGeometry);
    REQUIRE(shader.render_stage() == RenderStage::PhysicalGeometry);
}

TEST_CASE("Get and set the blend mode of a shader", "[Shader]")
{
    Shader shader;
    REQUIRE(shader.blend_mode() == BlendMode());
    BlendMode blend_mode(BlendFunction::Subtract, BlendFactor::One, BlendFactor::One);
    shader.set_blend_mode(blend_mode);
    REQUIRE(shader.blend_mode() == blend_mode);
}

TEST_CASE("Get and set whether a shader is depth tested", "[Shader]")
{
    Shader shader;
    REQUIRE(shader.is_depth_tested() == true);
    shader.set_depth_tested(false);
    REQUIRE(shader.is_depth_tested() == false);
}

TEST_CASE("Get and set the priority of a shader", "[Shader]")
{
    Shader shader;
    REQUIRE(shader.priority() == 0);
    shader.set_priority(1);
    REQUIRE(shader.priority() == 1);
}