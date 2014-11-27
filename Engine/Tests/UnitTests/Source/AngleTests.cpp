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
#include <Hect/Math/Angle.h>
#include <Hect/Math/Constants.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Construct a default angle", "[Angle]")
{
    Angle a;

    REQUIRE(a.degrees() == 0);
    REQUIRE(a.radians() == 0);
}

TEST_CASE("Construct an angle from degrees", "[Angle]")
{
    Angle a = Angle::fromDegrees(180);

    REQUIRE(a.degrees() == 180);
    REQUIRE(a.radians() == pi);
}

TEST_CASE("Construct an angle from radians", "[Angle]")
{
    Angle a = Angle::fromRadians(pi);

    REQUIRE(a.degrees() == 180);
    REQUIRE(a.radians() == pi);
}

TEST_CASE("Negate an angle", "[Angle]")
{
    Angle a = Angle::fromRadians(pi);
    a = -a;

    REQUIRE(a.degrees() == -180);
    REQUIRE(a.radians() == -pi);
}

TEST_CASE("Compute the sum of two angles", "[Angle]")
{
    SECTION("Constructed from the same unit units")
    {
        Angle a = Angle::fromDegrees(45) + Angle::fromDegrees(135);

        REQUIRE(a.degrees() == 180);
        REQUIRE(a.radians() == pi);
    }

    SECTION("Constructed from different units")
    {
        Angle a = Angle::fromRadians(pi / 4) + Angle::fromDegrees(135);

        REQUIRE(a.degrees() == 180);
        REQUIRE(a.radians() == pi);
    }
}

TEST_CASE("Add an angle to another angle", "[Angle]")
{
    Angle a = Angle::fromDegrees(45);
    a += Angle::fromDegrees(135);

    REQUIRE(a.degrees() == 180);
    REQUIRE(a.radians() == pi);
}

TEST_CASE("Compute the difference between two angles", "[Angle]")
{
    Angle a = Angle::fromDegrees(270) - Angle::fromDegrees(90);

    REQUIRE(a.degrees() == 180);
    REQUIRE(a.radians() == pi);
}

TEST_CASE("Subtract an angle from another angle", "[Angle]")
{
    Angle a = Angle::fromDegrees(270);
    a -= Angle::fromDegrees(90);

    REQUIRE(a.degrees() == 180);
    REQUIRE(a.radians() == pi);
}

TEST_CASE("Compute the product of an angle and a scalar", "[Angle]")
{
    Angle a = Angle::fromDegrees(90) * 2;

    REQUIRE(a.degrees() == 180);
    REQUIRE(a.radians() == pi);
}

TEST_CASE("Multiple an angle by a scalar", "[Angle]")
{
    Angle a = Angle::fromDegrees(90);
    a *= 2;
    REQUIRE(a.degrees() == 180);
    REQUIRE(a.radians() == pi);
}

TEST_CASE("Compute the quotient of an angle and a scalar", "[Angle]")
{
    Angle a = Angle::fromDegrees(360) / 2;

    REQUIRE(a.degrees() == 180);
    REQUIRE(a.radians() == pi);
}

TEST_CASE("Divide an angle by a scalar", "[Angle]")
{
    Angle a = Angle::fromDegrees(360);
    a /= 2;

    REQUIRE(a.degrees() == 180);
    REQUIRE(a.radians() == pi);
}