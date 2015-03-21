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
#include <Hect/Math/Quaternion.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Construct a default quaternion", "[Quaternion]")
{
    Quaternion a;
    REQUIRE(a.x == 0.0);
    REQUIRE(a.y == 0.0);
    REQUIRE(a.z == 0.0);
    REQUIRE(a.w == 1.0);
}

TEST_CASE("Construct a quaternion given its components", "[Quaternion]")
{
    Quaternion a(1.0, 2.0, 3.0, 4.0);
    REQUIRE(a.x == 1.0);
    REQUIRE(a.y == 2.0);
    REQUIRE(a.z == 3.0);
    REQUIRE(a.w == 4.0);
}

TEST_CASE("Construct a quaternion from a 3-dimensional vector", "[Quaternion]")
{
    Quaternion a(Vector3(1.0, 2.0, 3.0), 4.0);
    REQUIRE(a.x == 1.0);
    REQUIRE(a.y == 2.0);
    REQUIRE(a.z == 3.0);
    REQUIRE(a.w == 4.0);
}

TEST_CASE("Construct a quaternion from a 4-dimensional vector", "[Quaternion]")
{
    Quaternion a(Vector4(1.0, 2.0, 3.0, 4.0));
    REQUIRE(a.x == 1.0);
    REQUIRE(a.y == 2.0);
    REQUIRE(a.z == 3.0);
    REQUIRE(a.w == 4.0);
}

TEST_CASE("Copy a quaternion", "[Quaternion]")
{
    Quaternion a(Quaternion(1.0, 2.0, 3.0, 4.0));
    REQUIRE(a.x == 1.0);
    REQUIRE(a.y == 2.0);
    REQUIRE(a.z == 3.0);
    REQUIRE(a.w == 4.0);
}

TEST_CASE("Construct a quaternion from an axis and an angle", "[Quaternion]")
{
    Quaternion a = Quaternion::fromAxisAngle(Vector3::UnitY, Angle::fromDegrees(180));

    Vector3 axis;
    Angle angle;
    a.toAxisAngle(axis, angle);

    REQUIRE(axis.x == 0.0);
    REQUIRE(axis.y == 1.0);
    REQUIRE(axis.z == 0.0);
    REQUIRE(std::abs(angle.degrees() - 180.0) < 0.01);
}

TEST_CASE("Multiply two quaternions", "[Quaternion]")
{
    Vector3 axis;
    Angle angle;
    Quaternion a;
    Quaternion b;

    a = Quaternion::fromAxisAngle(Vector3::UnitY, Angle::fromDegrees(90));
    b = Quaternion::fromAxisAngle(Vector3::UnitY, Angle::fromDegrees(180));
    a = b * a;
    a.toAxisAngle(axis, angle);

    REQUIRE(axis.x == 0.0);
    REQUIRE(std::abs(axis.y - 1.0) < 0.01);
    REQUIRE(axis.z == 0.0);
    REQUIRE(angle.degrees() == 270);

    a = Quaternion::fromAxisAngle(Vector3::UnitY, Angle::fromDegrees(90));
    b = Quaternion::fromAxisAngle(Vector3::UnitY, Angle::fromDegrees(180));
    a *= b;
    a.toAxisAngle(axis, angle);

    REQUIRE(axis.x == 0.0);
    REQUIRE(axis.y == 1.0);
    REQUIRE(axis.z == 0.0);
    REQUIRE(angle.degrees() == 270);
}

TEST_CASE("Multply a quaternion and a vector", "[Quaternion]")
{
    Quaternion r;
    Vector3 v;

    r = Quaternion::fromAxisAngle(Vector3::UnitY, Angle::fromDegrees(180));
    v = r * Vector3::UnitX;
    REQUIRE(v.x == -1.0);
    REQUIRE(v.y == 0.0);
    REQUIRE(std::abs(v.z - 0.0) < 0.01);

    r = Quaternion::fromAxisAngle(Vector3::UnitY, Angle::fromDegrees(90));
    v = r * Vector3::UnitX;
    REQUIRE(std::abs(v.x - 0.0) < 0.01);
    REQUIRE(v.y == 0.0);
    REQUIRE(std::abs(v.z - 1.0) < 0.01);
}

TEST_CASE("Multiply a vector by an identity quaternion", "[Quaternion]")
{
    Vector3 v;

    v = Quaternion() * Vector3::UnitX;
    REQUIRE(v.x == 1.0);
    REQUIRE(v.y == 0.0);
    REQUIRE(v.z == 0.0);

    v = Quaternion() * Vector3::UnitY;
    REQUIRE(v.x == 0.0);
    REQUIRE(v.y == 1.0);
    REQUIRE(v.z == 0.0);

    v = Quaternion() * Vector3::UnitZ;
    REQUIRE(v.x == 0.0);
    REQUIRE(v.y == 0.0);
    REQUIRE(v.z == 1.0);
}

TEST_CASE("Cast a quaternion to another underlying type", "[Quaternion]")
{
    Quaternion a(1.0, 2.0, 3.0, 4.0);
    QuaternionT<float> b = a;

    REQUIRE(b.x == a.x);
    REQUIRE(b.y == a.y);
    REQUIRE(b.z == a.z);
    REQUIRE(b.w == a.w);
}

TEST_CASE("Index a quaternion", "[Quaternion]")
{
    Quaternion a(1.0, 2.0, 3.0, 4.0);

    REQUIRE(a[0] == 1.0);
    REQUIRE(a[1] == 2.0);
    REQUIRE(a[2] == 3.0);
    REQUIRE(a[3] == 4.0);
}