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

TEST_CASE("Construct a default 4-dimensional vector", "[Vector4]")
{
    Vector4 a;
    REQUIRE(a.x == 0.0);
    REQUIRE(a.y == 0.0);
    REQUIRE(a.z == 0.0);
    REQUIRE(a.w == 0.0);
}

TEST_CASE("Construct a 4-dimensional vector from a scalar", "[Vector4]")
{
    Vector4 a(1.0);
    REQUIRE(a.x == 1.0);
    REQUIRE(a.y == 1.0);
    REQUIRE(a.z == 1.0);
    REQUIRE(a.w == 1.0);
}

TEST_CASE("Construct a 4-dimensional vector given its components", "[Vector4]")
{
    Vector4 a(1.0, 2.0, 3.0, 4.0);
    REQUIRE(a.x == 1.0);
    REQUIRE(a.y == 2.0);
    REQUIRE(a.z == 3.0);
    REQUIRE(a.w == 4.0);
}

TEST_CASE("Copy a 4-dimensional vector", "[Vector4]")
{
    Vector4 a(1.0, 2.0, 3.0, 4.0);
    Vector4 b(a);
    REQUIRE(b.x == a.x);
    REQUIRE(b.y == a.y);
    REQUIRE(b.z == a.z);
    REQUIRE(b.w == a.w);
}

TEST_CASE("Get the length of a 4-dimensional vector", "[Vector4]")
{
    Vector4 a(1.0, 2.0, 3.0, 4.0);
    REQUIRE(a.length() == std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w));
}

TEST_CASE("Get the squared length of a 4-dimensional vector", "[Vector4]")
{
    Vector4 a(1.0, 2.0, 3.0, 4.0);
    REQUIRE(std::sqrt(a.lengthSquared()) == a.length());
}

TEST_CASE("Normalize a 4-dimensional vector", "[Vector4]")
{
    Vector4 a(1.0, 2.0, 3.0, 4.0);
    a.normalize();
    REQUIRE(std::abs(a.length() - 1.0) < 0.01);
}

TEST_CASE("Get a normalized copy of a 4-dimensional vector", "[Vector4]")
{
    Vector4 a(1.0, 2.0, 3.0, 4.0);
    REQUIRE(std::abs(a.normalized().length() - 1.0) < 0.01);
}

TEST_CASE("Compute the angle betwen two 4-dimensional vectors", "[Vector4]")
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

TEST_CASE("Compute the sum of two 4-dimensional vectors", "[Vector4]")
{
    Vector4 a = Vector4(1.0, 2.0, 3.0, 4.0) + Vector4(2.0, 4.0, 6.0, 8.0);
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 6.0);
    REQUIRE(a.z == 9.0);
    REQUIRE(a.w == 12.0);
}

TEST_CASE("Add a 4-dimensional vector to a 4-dimensional vector", "[Vector4]")
{
    Vector4 a = Vector4(1.0, 2.0, 3.0, 4.0);
    a += Vector4(2.0, 4.0, 6.0, 8.0);
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 6.0);
    REQUIRE(a.z == 9.0);
    REQUIRE(a.w == 12.0);
}

TEST_CASE("Compute the difference between two 4-dimensional vectors", "[Vector4]")
{
    Vector4 a = Vector4(1.0, 2.0, 3.0, 4.0) - Vector4(2.0, 4.0, 6.0, 8.0);
    REQUIRE(a.x == -1.0);
    REQUIRE(a.y == -2.0);
    REQUIRE(a.z == -3.0);
    REQUIRE(a.w == -4.0);
}

TEST_CASE("Subtract a 4-dimensional vector from a 4-dimensional vector", "[Vector4]")
{
    Vector4 a = Vector4(1.0, 2.0, 3.0, 4.0);
    a -= Vector4(2.0, 4.0, 6.0, 8.0);
    REQUIRE(a.x == -1.0);
    REQUIRE(a.y == -2.0);
    REQUIRE(a.z == -3.0);
    REQUIRE(a.w == -4.0);
}

TEST_CASE("Compute the product of two 4-dimensional vectors", "[Vector4]")
{
    Vector4 a = Vector4(1.0, 2.0, 3.0, 4.0) * Vector4(2.0, 4.0, 6.0, 8.0);
    REQUIRE(a.x == 2.0);
    REQUIRE(a.y == 8.0);
    REQUIRE(a.z == 18.0);
    REQUIRE(a.w == 32.0);
}

TEST_CASE("Multiply a 4-dimensional by a 4-dimensional vector", "[Vector4]")
{
    Vector4 a = Vector4(1.0, 2.0, 3.0, 4.0);
    a *= Vector4(2.0, 4.0, 6.0, 8.0);
    REQUIRE(a.x == 2.0);
    REQUIRE(a.y == 8.0);
    REQUIRE(a.z == 18.0);
    REQUIRE(a.w == 32.0);
}

TEST_CASE("Compute the product of a 4-dimensional vector and a scalar", "[Vector4]")
{
    Vector4 a = Vector4(1.0, 2.0, 3.0, 4.0) * 4.0;
    REQUIRE(a.x == 4.0);
    REQUIRE(a.y == 8.0);
    REQUIRE(a.z == 12.0);
    REQUIRE(a.w == 16.0);
}

TEST_CASE("Multiply a 4-dimensional vector by a scalar", "[Vector4]")
{
    Vector4 a = Vector4(1.0, 2.0, 3.0, 4.0);
    a *= 4.0;
    REQUIRE(a.x == 4.0);
    REQUIRE(a.y == 8.0);
    REQUIRE(a.z == 12.0);
    REQUIRE(a.w == 16.0);
}

TEST_CASE("Compute the quotient of two 4-dimensional vectors", "[Vector4]")
{
    Vector4 a = Vector4(6.0, 8.0, 12.0, 16.0) / Vector4(2.0, 4.0, 4.0, 8.0);
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 2.0);
    REQUIRE(a.z == 3.0);
    REQUIRE(a.w == 2.0);
}

TEST_CASE("Divide a 4-dimensional vector by a 4-dimensional vector", "[Vector4]")
{
    Vector4 a = Vector4(6.0, 8.0, 12.0, 16.0);
    a /= Vector4(2.0, 4.0, 4.0, 8.0);
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 2.0);
    REQUIRE(a.z == 3.0);
    REQUIRE(a.w == 2.0);
}

TEST_CASE("Compute the quotient of a 4-dimensional vector and a scalar", "[Vector4]")
{
    Vector4 a = Vector4(6.0, 8.0, 16.0, 24.0) / 2.0;
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 4.0);
    REQUIRE(a.z == 8.0);
    REQUIRE(a.w == 12.0);
}

TEST_CASE("Divide a 4-dimensional vector by a scalar", "[Vector4]")
{
    Vector4 a = Vector4(6.0, 8.0, 16.0, 24.0);
    a /= 2.0;
    REQUIRE(a.x == 3.0);
    REQUIRE(a.y == 4.0);
    REQUIRE(a.z == 8.0);
    REQUIRE(a.w == 12.0);
}

TEST_CASE("Cast a 4-dimensional to a different underlying type", "[Vector4]")
{
    Vector4 a(1.0, 2.0, 3.0, 4.0);
    Vector4T<float> b = a;

    REQUIRE(b.x == a.x);
    REQUIRE(b.y == a.y);
    REQUIRE(b.z == a.z);
    REQUIRE(b.w == a.w);
}

TEST_CASE("Index the components of a 4-dimensional vector", "[Vector4]")
{
    Vector4 a(1.0, 2.0, 3.0, 4.0);

    REQUIRE(a[0] == 1.0);
    REQUIRE(a[1] == 2.0);
    REQUIRE(a[2] == 3.0);
    REQUIRE(a[3] == 4.0);
}