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
#include <Hect/Math/Vector2.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Construct a default 2-dimensional vector", "[Vector2]")
{
    Vector2 a;
    REQUIRE(a.x == 0.0);
    REQUIRE(a.y == 0.0);
}

TEST_CASE("Construct a 2-dimensional vector from a scalar", "[Vector2]")
{
    Vector2 a(1.0);
    REQUIRE(a.x == 1.0);
    REQUIRE(a.y == 1.0);
}

TEST_CASE("Construct a 2-dimensional vector given its components", "[Vector2]")
{
    Vector2 a(1.0, 2.0);
    REQUIRE(a.x == 1.0);
    REQUIRE(a.y == 2.0);
}

TEST_CASE("Copy a 2-dimensional vector", "[Vector2]")
{
    Vector2 a(1.0, 2.0);
    Vector2 b(a);
    REQUIRE(b.x == a.x);
    REQUIRE(b.y == a.y);
}

TEST_CASE("Get the length of a 2-dimensional vector", "[Vector2]")
{
    Vector2 a(1.0, 2.0);
    REQUIRE(a.length() == std::sqrt(a.x * a.x + a.y * a.y));
}

TEST_CASE("Get the squared length of a 2-dimensional vector", "[Vector2]")
{
    Vector2 a(1.0, 2.0);
    REQUIRE(std::sqrt(a.lengthSquared()) == a.length());
}

TEST_CASE("Normalize a 2-dimensional vector", "[Vector2]")
{
    Vector2 a(1.0, 2.0);
    a.normalize();
    REQUIRE(std::abs(a.length() - 1.0) < 0.01);
}

TEST_CASE("Get a normalized copy of a 2-dimensional vector", "[Vector2]")
{
    Vector2 a(1.0, 2.0);
    REQUIRE(std::abs(a.normalized().length() - 1.0) < 0.01);
}

TEST_CASE("Compute the angle betwen two 2-dimensional vectors", "[Vector2]")
{
    REQUIRE(Vector2::unitX().angleFrom(Vector2::unitX()).degrees() == 0.0);
    REQUIRE(std::abs(Vector2::unitX().angleFrom(-Vector2::unitX()).degrees() - 180.0) < 0.01);

    REQUIRE(Vector2::unitY().angleFrom(Vector2::unitY()).degrees() == 0.0);
    REQUIRE(Vector2::unitY().angleFrom(-Vector2::unitY()).degrees() == 180.0);

    REQUIRE(Vector2::unitX().angleFrom(Vector2::unitY()).degrees() == 90.0);
    REQUIRE(Vector2::unitY().angleFrom(Vector2::unitX()).degrees() == 90.0);
}

TEST_CASE("Compute the sum of two 2-dimensional vectors", "[Vector2]")
{
    Vector2 a = Vector2(1.0, 2.0) + Vector2(2.0, 4.0);
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 6.0);
}

TEST_CASE("Add a 2-dimensional vector to a 2-dimensional vector", "[Vector2]")
{
    Vector2 a = Vector2(1.0, 2.0);
    a += Vector2(2.0, 4.0);
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 6.0);
}

TEST_CASE("Compute the difference between two 2-dimensional vectors", "[Vector2]")
{
    Vector2 a = Vector2(1.0, 2.0) - Vector2(2.0, 4.0);
    REQUIRE(a.x == -1.0);
    REQUIRE(a.y == -2.0);
}

TEST_CASE("Subtract a 2-dimensional vector from a 2-dimensional vector", "[Vector2]")
{
    Vector2 a = Vector2(1.0, 2.0);
    a -= Vector2(2.0, 4.0);
    REQUIRE(a.x == -1.0);
    REQUIRE(a.y == -2.0);
}

TEST_CASE("Compute the product of two 2-dimensional vectors", "[Vector2]")
{
    Vector2 a = Vector2(1.0, 2.0) * Vector2(2.0, 4.0);
    REQUIRE(a.x == 2.0);
    REQUIRE(a.y == 8.0);
}

TEST_CASE("Multiply a 2-dimensional by a 2-dimensional vector", "[Vector2]")
{
    Vector2 a = Vector2(1.0, 2.0);
    a *= Vector2(2.0, 4.0);
    REQUIRE(a.x == 2.0);
    REQUIRE(a.y == 8.0);
}

TEST_CASE("Compute the product of a 2-dimensional vector and a scalar", "[Vector2]")
{
    Vector2 a = Vector2(1.0, 2.0) * 4.0;
    REQUIRE(a.x == 4.0);
    REQUIRE(a.y == 8.0);
}

TEST_CASE("Multiply a 2-dimensional vector by a scalar", "[Vector2]")
{
    Vector2 a = Vector2(1.0, 2.0);
    a *= 4.0;
    REQUIRE(a.x == 4.0);
    REQUIRE(a.y == 8.0);
}

TEST_CASE("Compute the quotient of two 2-dimensional vectors", "[Vector2]")
{
    Vector2 a = Vector2(6.0, 8.0) / Vector2(2.0, 4.0);
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 2.0);
}

TEST_CASE("Divide a 2-dimensional vector by a 2-dimensional vector", "[Vector2]")
{
    Vector2 a = Vector2(6.0, 8.0);
    a /= Vector2(2.0, 4.0);
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 2.0);
}

TEST_CASE("Compute the quotient of a 2-dimensional vector and a scalar", "[Vector2]")
{
    Vector2 a = Vector2(6.0, 8.0) / 2.0;
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 4.0);
}

TEST_CASE("Divide a 2-dimensional vector by a scalar", "[Vector2]")
{
    Vector2 a = Vector2(6.0, 8.0);
    a /= 2.0;
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 4.0);
}

TEST_CASE("Cast a 2-dimensional to a different underlying type", "[Vector2]")
{
    Vector2 a(1.0, 2.0);
    Vector2T<float> b = a;

    REQUIRE(b.x == a.x);
    REQUIRE(b.y == a.y);
}

TEST_CASE("Index the components of a 2-dimensional vector", "[Vector2]")
{
    Vector2 a(1.0, 2.0);

    REQUIRE(a[0] == 1.0);
    REQUIRE(a[1] == 2.0);
}