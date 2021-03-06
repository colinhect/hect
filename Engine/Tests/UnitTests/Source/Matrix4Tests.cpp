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
#include <Hect/Math/Matrix4.h>
#include <Hect/Units/Angle.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Multiply a 3-dimensional vector by a 4x4 matrix", "[Matrix]")
{
    Quaternion r = Quaternion::from_axis_angle(Vector3::UnitY, Degrees(180));
    Matrix4 m = Matrix4::from_rotation(r);
    Vector3 v = m * Vector3::UnitX;
    REQUIRE(-1.0 == v.x);
    REQUIRE(0.0 == v.y);
    REQUIRE(std::abs(v.z - 0.0) < 0.01);

    r = Quaternion::from_axis_angle(Vector3::UnitY, Degrees(90));
    m = Matrix4::from_rotation(r);
    v = m * Vector3::UnitX;
    REQUIRE(std::abs(v.x - 0.0) < 0.01);
    REQUIRE(0.0 == v.y);
    REQUIRE(std::abs(v.z - 1.0) < 0.01);
}

TEST_CASE("Multiply a 3-dimensional vector by the identity 4x4 matrix", "[Matrix]")
{
    Vector3 v = Matrix4() * Vector3::UnitX;
    REQUIRE(1.0 == v.x);
    REQUIRE(0.0 == v.y);
    REQUIRE(0.0 == v.z);

    v = Matrix4() * Vector3::UnitY;
    REQUIRE(0.0 == v.x);
    REQUIRE(1.0 == v.y);
    REQUIRE(0.0 == v.z);

    v = Matrix4() * Vector3::UnitZ;
    REQUIRE(0.0 == v.x);
    REQUIRE(0.0 == v.y);
    REQUIRE(1.0 == v.z);
}

TEST_CASE("Cast a 4x4 matrix from double to float", "[Matrix]")
{
    Quaternion r = Quaternion::from_axis_angle(Vector3::UnitY, Degrees(180));
    Matrix4 a = Matrix4::from_rotation(r);
    Matrix4T<float> b = a;

    for (size_t i = 0; i < 16; ++i)
    {
        REQUIRE(std::abs(a[i] - b[i]) < 0.01);
    }
}
