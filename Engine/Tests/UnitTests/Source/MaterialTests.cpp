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
#include <Hect/Graphics/Material.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Get and set the shader of a material", "[Material]")
{
    Shader shader;
    Shader::Handle shaderHandle = shader.createHandle();

    Material material;
    REQUIRE(!material.shader());

    material.setShader(shaderHandle);
    REQUIRE(material.shader() == shaderHandle);
}

TEST_CASE("Get and set the cull mode of a material", "[Material]")
{
    Material material;
    REQUIRE(material.cullMode() == CullMode::CounterClockwise);

    material.setCullMode(CullMode::None);
    REQUIRE(material.cullMode() == CullMode::None);
}

TEST_CASE("Set a uniform value in a material without a set shader", "[Material]")
{
    Material material;
    REQUIRE_THROWS_AS(material.setUniformValue("A", 0), InvalidOperation);
}

TEST_CASE("Set a uniform value in a material for a non-existing uniform in the shader", "[Material]")
{
    Shader shader;
    Material material;
    material.setShader(shader.createHandle());
    REQUIRE_THROWS_AS(material.setUniformValue("A", 0), InvalidOperation);
}

TEST_CASE("Set and get a uniform value in a material for a uniform of the same type in the shader", "[Material]")
{
    Shader shader;
    shader.addUniform(Uniform("A", UniformType::Int));

    Material material;
    material.setShader(shader.createHandle());
    material.setUniformValue("A", 1);

    const UniformValue& uniformValue = material.uniformValues()[0];
    REQUIRE(uniformValue.type() == UniformType::Int);
    REQUIRE(uniformValue.asInt() == 1);
}

TEST_CASE("Set and get a uniform value in a material for a uniform of a different type in the shader", "[Material]")
{
    Shader shader;
    shader.addUniform(Uniform("A", UniformType::Texture));

    Material material;
    material.setShader(shader.createHandle());
    REQUIRE_THROWS_AS(material.setUniformValue("A", 1), InvalidOperation);
}

TEST_CASE("Set and get a uniform value in a material for a bound uniform in the shader", "[Material]")
{
    Shader shader;
    shader.addUniform(Uniform("A", UniformBinding::CameraPosition));

    Material material;
    material.setShader(shader.createHandle());
    REQUIRE_THROWS_AS(material.setUniformValue("A", 1), InvalidOperation);
}

TEST_CASE("Clear the uniform values of a material", "[Material]")
{
    Shader shader;
    shader.addUniform(Uniform("A", UniformType::Int));

    Material material;
    material.setShader(shader.createHandle());
    material.setUniformValue("A", 1);

    REQUIRE(!material.uniformValues().empty());

    material.clearUniformValues();

    REQUIRE(material.uniformValues().empty());
}

TEST_CASE("Set the shader of a material with uniform values", "[Material]")
{
    Shader shader;
    shader.addUniform(Uniform("A", UniformType::Int));

    Material material;
    material.setShader(shader.createHandle());
    material.setUniformValue("A", 1);

    REQUIRE(!material.uniformValues().empty());

    material.setShader(Shader::Handle());

    REQUIRE(material.uniformValues().empty());
}
