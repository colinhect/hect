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
#include <Hect/Math/Vector4.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Vector4_DefaultConstructor")
{
    Vector4 a;
    REQUIRE(a.x == 0.0);
    REQUIRE(a.y == 0.0);
    REQUIRE(a.z == 0.0);
    REQUIRE(a.w == 0.0);
}

TEST_CASE("Vector4_ScalarConstructor")
{
    Vector4 a(1.0);
    REQUIRE(a.x == 1.0);
    REQUIRE(a.y == 1.0);
    REQUIRE(a.z == 1.0);
    REQUIRE(a.w == 1.0);
}

TEST_CASE("Vector4_ComponentConstructor")
{
    Vector4 a(1.0, 2.0, 3.0, 4.0);
    REQUIRE(a.x == 1.0);
    REQUIRE(a.y == 2.0);
    REQUIRE(a.z == 3.0);
    REQUIRE(a.w == 4.0);
}

TEST_CASE("Vector4_CopyConstructor")
{
    Vector4 a(1.0, 2.0, 3.0, 4.0);
    Vector4 b(a);
    REQUIRE(b.x == a.x);
    REQUIRE(b.y == a.y);
    REQUIRE(b.z == a.z);
    REQUIRE(b.w == a.w);
}

TEST_CASE("Vector4_Length")
{
    Vector4 a(1.0, 2.0, 3.0, 4.0);
    REQUIRE(a.length() == std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w));
}

TEST_CASE("Vector4_LengthSquared")
{
    Vector4 a(1.0, 2.0, 3.0, 4.0);
    REQUIRE(std::sqrt(a.lengthSquared()) == a.length());
}

TEST_CASE("Vector4_Normalize")
{
    Vector4 a(1.0, 2.0, 3.0, 4.0);
    a.normalize();
    REQUIRE(std::abs(a.length() - 1.0) < 0.01);
}

TEST_CASE("Vector4_Normalized")
{
    Vector4 a(1.0, 2.0, 3.0, 4.0);
    REQUIRE(std::abs(a.normalized().length() - 1.0) < 0.01);
}

TEST_CASE("Vector4_AngleFrom")
{
    REQUIRE(Vector4::unitX().angleFrom(Vector4::unitX()).degrees() == 0.0);
    REQUIRE(Vector4::unitX().angleFrom(-Vector4::unitX()).degrees() == 180.0);

    REQUIRE(Vector4::unitY().angleFrom(Vector4::unitY()).degrees() == 0.0);
    REQUIRE(Vector4::unitY().angleFrom(-Vector4::unitY()).degrees() == 180.0);

    REQUIRE(Vector4::unitZ().angleFrom(Vector4::unitZ()).degrees() == 0.0);
    REQUIRE(Vector4::unitZ().angleFrom(-Vector4::unitZ()).degrees() == 180.0);

    REQUIRE(Vector4::unitW().angleFrom(Vector4::unitW()).degrees() == 0.0);
    REQUIRE(Vector4::unitW().angleFrom(-Vector4::unitW()).degrees() == 180.0);

    REQUIRE(Vector4::unitX().angleFrom(Vector4::unitY()).degrees() == 90.0);
    REQUIRE(Vector4::unitX().angleFrom(Vector4::unitZ()).degrees() == 90.0);
    REQUIRE(Vector4::unitX().angleFrom(Vector4::unitW()).degrees() == 90.0);
    REQUIRE(Vector4::unitY().angleFrom(Vector4::unitX()).degrees() == 90.0);
    REQUIRE(Vector4::unitY().angleFrom(Vector4::unitZ()).degrees() == 90.0);
    REQUIRE(Vector4::unitY().angleFrom(Vector4::unitW()).degrees() == 90.0);
    REQUIRE(Vector4::unitZ().angleFrom(Vector4::unitX()).degrees() == 90.0);
    REQUIRE(Vector4::unitZ().angleFrom(Vector4::unitY()).degrees() == 90.0);
    REQUIRE(Vector4::unitZ().angleFrom(Vector4::unitW()).degrees() == 90.0);
    REQUIRE(Vector4::unitW().angleFrom(Vector4::unitX()).degrees() == 90.0);
    REQUIRE(Vector4::unitW().angleFrom(Vector4::unitY()).degrees() == 90.0);
    REQUIRE(Vector4::unitW().angleFrom(Vector4::unitZ()).degrees() == 90.0);
}

TEST_CASE("Vector4_Add")
{
    Vector4 a = Vector4(1.0, 2.0, 3.0, 4.0) + Vector4(2.0, 4.0, 6.0, 8.0);
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 6.0);
    REQUIRE(a.z == 9.0);
    REQUIRE(a.w == 12.0);
}

TEST_CASE("Vector4_AddEquals")
{
    Vector4 a = Vector4(1.0, 2.0, 3.0, 4.0);
    a += Vector4(2.0, 4.0, 6.0, 8.0);
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 6.0);
    REQUIRE(a.z == 9.0);
    REQUIRE(a.w == 12.0);
}

TEST_CASE("Vector4_Subtract")
{
    Vector4 a = Vector4(1.0, 2.0, 3.0, 4.0) - Vector4(2.0, 4.0, 6.0, 8.0);
    REQUIRE(a.x == -1.0);
    REQUIRE(a.y == -2.0);
    REQUIRE(a.z == -3.0);
    REQUIRE(a.w == -4.0);
}

TEST_CASE("Vector4_SubtractEquals")
{
    Vector4 a = Vector4(1.0, 2.0, 3.0, 4.0);
    a -= Vector4(2.0, 4.0, 6.0, 8.0);
    REQUIRE(a.x == -1.0);
    REQUIRE(a.y == -2.0);
    REQUIRE(a.z == -3.0);
    REQUIRE(a.w == -4.0);
}

TEST_CASE("Vector4_Multiply")
{
    Vector4 a = Vector4(1.0, 2.0, 3.0, 4.0) * Vector4(2.0, 4.0, 6.0, 8.0);
    REQUIRE(a.x == 2.0);
    REQUIRE(a.y == 8.0);
    REQUIRE(a.z == 18.0);
    REQUIRE(a.w == 32.0);
}

TEST_CASE("Vector4_MultiplyEquals")
{
    Vector4 a = Vector4(1.0, 2.0, 3.0, 4.0);
    a *= Vector4(2.0, 4.0, 6.0, 8.0);
    REQUIRE(a.x == 2.0);
    REQUIRE(a.y == 8.0);
    REQUIRE(a.z == 18.0);
    REQUIRE(a.w == 32.0);
}

TEST_CASE("Vector4_MultiplyScalar")
{
    Vector4 a = Vector4(1.0, 2.0, 3.0, 4.0) * 4.0;
    REQUIRE(a.x == 4.0);
    REQUIRE(a.y == 8.0);
    REQUIRE(a.z == 12.0);
    REQUIRE(a.w == 16.0);
}

TEST_CASE("Vector4_MultiplyScalarEquals")
{
    Vector4 a = Vector4(1.0, 2.0, 3.0, 4.0);
    a *= 4.0;
    REQUIRE(a.x == 4.0);
    REQUIRE(a.y == 8.0);
    REQUIRE(a.z == 12.0);
    REQUIRE(a.w == 16.0);
}

TEST_CASE("Vector4_Divide")
{
    Vector4 a = Vector4(6.0, 8.0, 12.0, 16.0) / Vector4(2.0, 4.0, 4.0, 8.0);
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 2.0);
    REQUIRE(a.z == 3.0);
    REQUIRE(a.w == 2.0);
}

TEST_CASE("Vector4_DivideEquals")
{
    Vector4 a = Vector4(6.0, 8.0, 12.0, 16.0);
    a /= Vector4(2.0, 4.0, 4.0, 8.0);
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 2.0);
    REQUIRE(a.z == 3.0);
    REQUIRE(a.w == 2.0);
}

TEST_CASE("Vector4_DivideScalar")
{
    Vector4 a = Vector4(6.0, 8.0, 16.0, 24.0) / 2.0;
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 4.0);
    REQUIRE(a.z == 8.0);
    REQUIRE(a.w == 12.0);
}

TEST_CASE("Vector4_DivideScalarEquals")
{
    Vector4 a = Vector4(6.0, 8.0, 16.0, 24.0);
    a /= 2.0;
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 4.0);
    REQUIRE(a.z == 8.0);
    REQUIRE(a.w == 12.0);
}

TEST_CASE("Vector4_Cast")
{
    double x = 1.0f;
    Vector4 a(1.0, 2.0, 3.0, 4.0);
    Vector4T<float> b = a;

    REQUIRE(b.x == a.x);
    REQUIRE(b.y == a.y);
    REQUIRE(b.z == a.z);
    REQUIRE(b.w == a.w);
}

TEST_CASE("Vector4_Index")
{
    Vector4 a(1.0, 2.0, 3.0, 4.0);

    REQUIRE(a[0] == 1.0);
    REQUIRE(a[1] == 2.0);
    REQUIRE(a[2] == 3.0);
    REQUIRE(a[3] == 4.0);
}