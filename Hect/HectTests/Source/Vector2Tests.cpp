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

TEST_CASE("Vector2_DefaultConstructor")
{
    Vector2 a;
    REQUIRE(a.x == 0.0);
    REQUIRE(a.y == 0.0);
}

TEST_CASE("Vector2_ScalarConstructor")
{
    Vector2 a(1.0);
    REQUIRE(a.x == 1.0);
    REQUIRE(a.y == 1.0);
}

TEST_CASE("Vector2_ComponentConstructor")
{
    Vector2 a(1.0, 2.0);
    REQUIRE(a.x == 1.0);
    REQUIRE(a.y == 2.0);
}

TEST_CASE("Vector2_CopyConstructor")
{
    Vector2 a(1.0, 2.0);
    Vector2 b(a);
    REQUIRE(b.x == a.x);
    REQUIRE(b.y == a.y);
}

TEST_CASE("Vector2_Length")
{
    Vector2 a(1.0, 2.0);
    REQUIRE(a.length() == std::sqrt(a.x * a.x + a.y * a.y));
}

TEST_CASE("Vector2_LengthSquared")
{
    Vector2 a(1.0, 2.0);
    REQUIRE(std::sqrt(a.lengthSquared()) == a.length());
}

TEST_CASE("Vector2_Normalize")
{
    Vector2 a(1.0, 2.0);
    a.normalize();
    REQUIRE(std::abs(a.length() - 1.0) < 0.01);
}

TEST_CASE("Vector2_Normalized")
{
    Vector2 a(1.0, 2.0);
    REQUIRE(std::abs(a.normalized().length() - 1.0) < 0.01);
}

TEST_CASE("Vector2_AngleFrom")
{
    REQUIRE(Vector2::unitX().angleFrom(Vector2::unitX()).degrees() == 0.0);
    REQUIRE(std::abs(Vector2::unitX().angleFrom(-Vector2::unitX()).degrees() - 180.0) < 0.01);

    REQUIRE(Vector2::unitY().angleFrom(Vector2::unitY()).degrees() == 0.0);
    REQUIRE(Vector2::unitY().angleFrom(-Vector2::unitY()).degrees() == 180.0);

    REQUIRE(Vector2::unitX().angleFrom(Vector2::unitY()).degrees() == 90.0);
    REQUIRE(Vector2::unitY().angleFrom(Vector2::unitX()).degrees() == 90.0);
}

TEST_CASE("Vector2_Add")
{
    Vector2 a = Vector2(1.0, 2.0) + Vector2(2.0, 4.0);
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 6.0);
}

TEST_CASE("Vector2_AddEquals")
{
    Vector2 a = Vector2(1.0, 2.0);
    a += Vector2(2.0, 4.0);
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 6.0);
}

TEST_CASE("Vector2_Subtract")
{
    Vector2 a = Vector2(1.0, 2.0) - Vector2(2.0, 4.0);
    REQUIRE(a.x == -1.0);
    REQUIRE(a.y == -2.0);
}

TEST_CASE("Vector2_SubtractEquals")
{
    Vector2 a = Vector2(1.0, 2.0);
    a -= Vector2(2.0, 4.0);
    REQUIRE(a.x == -1.0);
    REQUIRE(a.y == -2.0);
}

TEST_CASE("Vector2_Multiply")
{
    Vector2 a = Vector2(1.0, 2.0) * Vector2(2.0, 4.0);
    REQUIRE(a.x == 2.0);
    REQUIRE(a.y == 8.0);
}

TEST_CASE("Vector2_MultiplyEquals")
{
    Vector2 a = Vector2(1.0, 2.0);
    a *= Vector2(2.0, 4.0);
    REQUIRE(a.x == 2.0);
    REQUIRE(a.y == 8.0);
}

TEST_CASE("Vector2_MultiplyScalar")
{
    Vector2 a = Vector2(1.0, 2.0) * 4.0;
    REQUIRE(a.x == 4.0);
    REQUIRE(a.y == 8.0);
}

TEST_CASE("Vector2_MultiplyScalarEquals")
{
    Vector2 a = Vector2(1.0, 2.0);
    a *= 4.0;
    REQUIRE(a.x == 4.0);
    REQUIRE(a.y == 8.0);
}

TEST_CASE("Vector2_Divide")
{
    Vector2 a = Vector2(6.0, 8.0) / Vector2(2.0, 4.0);
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 2.0);
}

TEST_CASE("Vector2_DivideEquals")
{
    Vector2 a = Vector2(6.0, 8.0);
    a /= Vector2(2.0, 4.0);
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 2.0);
}

TEST_CASE("Vector2_DivideScalar")
{
    Vector2 a = Vector2(6.0, 8.0) / 2.0;
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 4.0);
}

TEST_CASE("Vector2_DivideScalarEquals")
{
    Vector2 a = Vector2(6.0, 8.0);
    a /= 2.0;
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 4.0);
}

TEST_CASE("Vector2_Cast")
{
    Vector2 a(1.0, 2.0);
    Vector2T<float> b = a;

    REQUIRE(b.x == a.x);
    REQUIRE(b.y == a.y);
}

TEST_CASE("Vector2_Index")
{
    Vector2 a(1.0, 2.0);

    REQUIRE(a[0] == 1.0);
    REQUIRE(a[1] == 2.0);
}