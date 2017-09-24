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
#include <Hect/Graphics/Material.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Get and set the shader of a material", "[Material]")
{
    Shader shader;
    AssetHandle<Shader> shader_handle = shader.create_handle();

    Material material;
    REQUIRE(!material.shader());

    material.set_shader(shader_handle);
    REQUIRE(material.shader() == shader_handle);
}

TEST_CASE("Get and set the cull mode of a material", "[Material]")
{
    Material material;
    REQUIRE(material.cull_mode() == CullMode::CounterClockwise);

    material.set_cull_mode(CullMode::None);
    REQUIRE(material.cull_mode() == CullMode::None);
}

TEST_CASE("Set a uniform value in a material without a set shader", "[Material]")
{
    Material material;
    REQUIRE_THROWS_AS(material.set_uniform_value("A", 0), InvalidOperation);
}

TEST_CASE("Set a uniform value in a material for a non-existing uniform in the shader", "[Material]")
{
    Shader shader;
    Material material;
    material.set_shader(shader.create_handle());
    REQUIRE_THROWS_AS(material.set_uniform_value("A", 0), InvalidOperation);
}

TEST_CASE("Set and get a uniform value in a material for a uniform of the same type in the shader", "[Material]")
{
    Shader shader;
    shader.add_uniform(Uniform("A", UniformType::Int));

    Material material;
    material.set_shader(shader.create_handle());
    material.set_uniform_value("A", 1);

    const UniformValue& uniform_value = material.uniform_values()[0];
    REQUIRE(uniform_value.type() == UniformType::Int);
    REQUIRE(uniform_value.as_int() == 1);
}

TEST_CASE("Set and get a uniform value in a material for a uniform of a different type in the shader", "[Material]")
{
    Shader shader;
    shader.add_uniform(Uniform("A", UniformType::Texture2));

    Material material;
    material.set_shader(shader.create_handle());
    REQUIRE_THROWS_AS(material.set_uniform_value("A", 1), InvalidOperation);
}

TEST_CASE("Set and get a uniform value in a material for a bound uniform in the shader", "[Material]")
{
    Shader shader;
    shader.add_uniform(Uniform("A", UniformBinding::CameraPosition));

    Material material;
    material.set_shader(shader.create_handle());
    REQUIRE_THROWS_AS(material.set_uniform_value("A", 1), InvalidOperation);
}

TEST_CASE("Clear the uniform values of a material", "[Material]")
{
    Shader shader;
    shader.add_uniform(Uniform("A", UniformType::Int));

    Material material;
    material.set_shader(shader.create_handle());
    material.set_uniform_value("A", 1);

    REQUIRE(!material.uniform_values().empty());

    material.clear_uniform_values();

    REQUIRE(material.uniform_values().empty());
}

TEST_CASE("Set the shader of a material with uniform values", "[Material]")
{
    Shader shader;
    shader.add_uniform(Uniform("A", UniformType::Int));

    Material material;
    material.set_shader(shader.create_handle());
    material.set_uniform_value("A", 1);

    REQUIRE(!material.uniform_values().empty());

    material.set_shader(AssetHandle<Shader>());

    REQUIRE(material.uniform_values().empty());
}
