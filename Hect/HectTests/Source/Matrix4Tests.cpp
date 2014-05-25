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
#include <Hect.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Matrix4_MultiplyVector")
{
    Quaternion r;
    Matrix4 m;
    Vector3 v;

    r = Quaternion::fromAxisAngle(Vector3::unitY(), Angle::fromDegrees(180));
    m = Matrix4::fromRotation(r);
    v = m * Vector3::unitX();
    REQUIRE(-1.0 == v.x);
    REQUIRE(0.0 == v.y);
    REQUIRE(std::abs(v.z - 0.0) < 0.01);

    r = Quaternion::fromAxisAngle(Vector3::unitY(), Angle::fromDegrees(90));
    m = Matrix4::fromRotation(r);
    v = m * Vector3::unitX();
    REQUIRE(std::abs(v.x - 0.0) < 0.01);
    REQUIRE(0.0 == v.y);
    REQUIRE(std::abs(v.z - 1.0) < 0.01);
}

TEST_CASE("Matrix4_MultiplyVectorByIdentity")
{
    Vector3 v = Matrix4() * Vector3::unitX();
    REQUIRE(1.0 == v.x);
    REQUIRE(0.0 == v.y);
    REQUIRE(0.0 == v.z);

    v = Matrix4() * Vector3::unitY();
    REQUIRE(0.0 == v.x);
    REQUIRE(1.0 == v.y);
    REQUIRE(0.0 == v.z);

    v = Matrix4() * Vector3::unitZ();
    REQUIRE(0.0 == v.x);
    REQUIRE(0.0 == v.y);
    REQUIRE(1.0 == v.z);
}

TEST_CASE("Matrix4_Cast")
{
    Quaternion r = Quaternion::fromAxisAngle(Vector3::unitY(), Angle::fromDegrees(180));
    Matrix4 a = Matrix4::fromRotation(r);
    Matrix4T<float> b = a;

    for (size_t i = 0; i < 16; ++i)
    {
        REQUIRE(std::abs(a[i] - b[i]) < 0.01);
    }
}