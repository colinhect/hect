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
#include <Hect/Graphics/Material.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Construct a material without a name", "[Material]")
{
    Material material;

    REQUIRE(material.name() == "<unnamed>");
}

TEST_CASE("Construct a material with a name", "[Material]")
{
    Material material("A");

    REQUIRE(material.name() == "A");
}

TEST_CASE("Get and set the type of a material", "[Material]")
{
    Material material;

    REQUIRE(material.type() == MaterialType_None);
    material.setType(MaterialType_OpaqueGeometry);
    REQUIRE(material.type() == MaterialType_OpaqueGeometry);
}

TEST_CASE("Get and set whether blending is enabled for a material", "[Material]")
{
    Material material;

    REQUIRE(material.blend() == false);
    material.setBlend(true);
    REQUIRE(material.depthTest() == true);
}

TEST_CASE("Get and set the blend factors of a material", "[Material]")
{
    Material material;

    REQUIRE(material.sourceBlendFactor() == BlendFactor_One);
    REQUIRE(material.destinationBlendFactor() == BlendFactor_One);

    material.setBlendFactors(BlendFactor_DestAlpha, BlendFactor_DestColor);

    REQUIRE(material.sourceBlendFactor() == BlendFactor_DestAlpha);
    REQUIRE(material.destinationBlendFactor() == BlendFactor_DestColor);
}

TEST_CASE("Get and set whether depth testing is enabled for a material", "[Material]")
{
    Material material;

    REQUIRE(material.depthTest() == true);
    material.setDepthTest(false);
    REQUIRE(material.depthTest() == false);
}

TEST_CASE("Get and set whether depth writing is enabled for a material", "[Material]")
{
    Material material;

    REQUIRE(material.depthWrite() == true);
    material.setDepthWrite(false);
    REQUIRE(material.depthWrite() == false);
}

TEST_CASE("Get and set whether back face culling is enabled for a material", "[Material]")
{
    Material material;

    REQUIRE(material.cullBackFace() == true);
    material.setCullBackFace(false);
    REQUIRE(material.cullBackFace() == false);
}

TEST_CASE("Get and set the priority of a material", "[Material]")
{
    Material material;

    REQUIRE(material.priority() == 0);
    material.setPriority(100);
    REQUIRE(material.priority() == 100);
}