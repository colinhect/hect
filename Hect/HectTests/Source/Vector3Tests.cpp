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

TEST_CASE("Vector3_DefaultConstructor")
{
    Vector3 a;
    REQUIRE(a.x == 0.0);
    REQUIRE(a.y == 0.0);
    REQUIRE(a.z == 0.0);
}

TEST_CASE("Vector3_ScalarConstructor")
{
    Vector3 a(1.0);
    REQUIRE(a.x == 1.0);
    REQUIRE(a.y == 1.0);
    REQUIRE(a.z == 1.0);
}

TEST_CASE("Vector3_ComponentConstructor")
{
    Vector3 a(1.0, 2.0, 3.0);
    REQUIRE(a.x == 1.0);
    REQUIRE(a.y == 2.0);
    REQUIRE(a.z == 3.0);
}

TEST_CASE("Vector3_CopyConstructor")
{
    Vector3 a(1.0, 2.0, 3.0);
    Vector3 b(a);
    REQUIRE(b.x == a.x);
    REQUIRE(b.y == a.y);
    REQUIRE(b.z == a.z);
}

TEST_CASE("Vector3_Length")
{
    Vector3 a(1.0, 2.0, 3.0);
    REQUIRE(a.length() == std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z));
}

TEST_CASE("Vector3_LengthSquared")
{
    Vector3 a(1.0, 2.0, 3.0);
    REQUIRE(std::sqrt(a.lengthSquared()) == a.length());
}

TEST_CASE("Vector3_Normalize")
{
    Vector3 a(1.0, 2.0, 3.0);
    a.normalize();
    REQUIRE(a.length() == 1.0);
}

TEST_CASE("Vector3_Normalized")
{
    Vector3 a(1.0, 2.0, 3.0);
    REQUIRE(a.normalized().length() == 1.0);
}

TEST_CASE("Vector3_AngleFrom")
{
    REQUIRE(Vector3::unitX().angleFrom(Vector3::unitX()).degrees() == 0.0);
    REQUIRE(Vector3::unitX().angleFrom(-Vector3::unitX()).degrees() == 180.0);

    REQUIRE(Vector3::unitY().angleFrom(Vector3::unitY()).degrees() == 0.0);
    REQUIRE(Vector3::unitY().angleFrom(-Vector3::unitY()).degrees() == 180.0);

    REQUIRE(Vector3::unitZ().angleFrom(Vector3::unitZ()).degrees() == 0.0);
    REQUIRE(Vector3::unitZ().angleFrom(-Vector3::unitZ()).degrees() == 180.0);

    REQUIRE(Vector3::unitX().angleFrom(Vector3::unitY()).degrees() == 90.0);
    REQUIRE(Vector3::unitX().angleFrom(Vector3::unitZ()).degrees() == 90.0);
    REQUIRE(Vector3::unitY().angleFrom(Vector3::unitX()).degrees() == 90.0);
    REQUIRE(Vector3::unitY().angleFrom(Vector3::unitZ()).degrees() == 90.0);
    REQUIRE(Vector3::unitZ().angleFrom(Vector3::unitX()).degrees() == 90.0);
    REQUIRE(Vector3::unitZ().angleFrom(Vector3::unitY()).degrees() == 90.0);
}

TEST_CASE("Vector3_Cross")
{
    Vector3 a = Vector3::unitX().cross(Vector3::unitY());

    REQUIRE(a.x == 0.0);
    REQUIRE(a.y == 0.0);
    REQUIRE(a.z == 1.0);

    a = Vector3::unitY().cross(Vector3::unitX());

    REQUIRE(a.x == 0.0);
    REQUIRE(a.y == 0.0);
    REQUIRE(a.z == -1.0);

    a = Vector3::unitX().cross(Vector3::unitZ());

    REQUIRE(a.x == 0.0);
    REQUIRE(a.y == -1.0);
    REQUIRE(a.z == 0.0);

    a = Vector3::unitZ().cross(Vector3::unitX());

    REQUIRE(a.x == 0.0);
    REQUIRE(a.y == 1.0);
    REQUIRE(a.z == 0.0);
}

TEST_CASE("Vector3_Add")
{
    Vector3 a = Vector3(1.0, 2.0, 3.0) + Vector3(2.0, 4.0, 6.0);
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 6.0);
    REQUIRE(a.z == 9.0);
}

TEST_CASE("Vector3_AddEquals")
{
    Vector3 a = Vector3(1.0, 2.0, 3.0);
    a += Vector3(2.0, 4.0, 6.0);
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 6.0);
    REQUIRE(a.z == 9.0);
}

TEST_CASE("Vector3_Subtract")
{
    Vector3 a = Vector3(1.0, 2.0, 3.0) - Vector3(2.0, 4.0, 6.0);
    REQUIRE(a.x == -1.0);
    REQUIRE(a.y == -2.0);
    REQUIRE(a.z == -3.0);
}

TEST_CASE("Vector3_SubtractEquals")
{
    Vector3 a = Vector3(1.0, 2.0, 3.0);
    a -= Vector3(2.0, 4.0, 6.0);
    REQUIRE(a.x == -1.0);
    REQUIRE(a.y == -2.0);
    REQUIRE(a.z == -3.0);
}

TEST_CASE("Vector3_Multiply")
{
    Vector3 a = Vector3(1.0, 2.0, 3.0) * Vector3(2.0, 4.0, 6.0);
    REQUIRE(a.x == 2.0);
    REQUIRE(a.y == 8.0);
    REQUIRE(a.z == 18.0);
}

TEST_CASE("Vector3_MultiplyEquals")
{
    Vector3 a = Vector3(1.0, 2.0, 3.0);
    a *= Vector3(2.0, 4.0, 6.0);
    REQUIRE(a.x == 2.0);
    REQUIRE(a.y == 8.0);
    REQUIRE(a.z == 18.0);
}

TEST_CASE("Vector3_MultiplyScalar")
{
    Vector3 a = Vector3(1.0, 2.0, 3.0) * 4.0;
    REQUIRE(a.x == 4.0);
    REQUIRE(a.y == 8.0);
    REQUIRE(a.z == 12.0);
}

TEST_CASE("Vector3_MultiplyScalarEquals")
{
    Vector3 a = Vector3(1.0, 2.0, 3.0);
    a *= 4.0;
    REQUIRE(a.x == 4.0);
    REQUIRE(a.y == 8.0);
    REQUIRE(a.z == 12.0);
}

TEST_CASE("Vector3_Divide")
{
    Vector3 a = Vector3(6.0, 8.0, 12.0) / Vector3(2.0, 4.0, 4.0);
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 2.0);
    REQUIRE(a.z == 3.0);
}

TEST_CASE("Vector3_DivideEquals")
{
    Vector3 a = Vector3(6.0, 8.0, 12.0);
    a /= Vector3(2.0, 4.0, 4.0);
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 2.0);
    REQUIRE(a.z == 3.0);
}

TEST_CASE("Vector3_DivideScalar")
{
    Vector3 a = Vector3(6.0, 8.0, 16.0) / 2.0;
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 4.0);
    REQUIRE(a.z == 8.0);
}

TEST_CASE("Vector3_DivideScalarEquals")
{
    Vector3 a = Vector3(6.0, 8.0, 16.0);
    a /= 2.0;
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 4.0);
    REQUIRE(a.z == 8.0);
}

TEST_CASE("Vector3_Cast")
{
    Vector3 a(1.0, 2.0, 3.0);
    Vector3T<float> b = a;

    REQUIRE(b.x == a.x);
    REQUIRE(b.y == a.y);
    REQUIRE(b.z == a.z);
}

TEST_CASE("Vector3_Index")
{
    Vector3 a(1.0, 2.0, 3.0);

    REQUIRE(a[0] == 1.0);
    REQUIRE(a[1] == 2.0);
    REQUIRE(a[2] == 3.0);
}