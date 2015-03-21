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

TEST_CASE("Get the floor of a 4-dimensional vector", "[Vector4]")
{
    Vector4 a(1.8, 2.1, 3.5, 4.1);
    Vector4 b(a.floor());
    REQUIRE(b.x == 1.0);
    REQUIRE(b.y == 2.0);
    REQUIRE(b.z == 3.0);
    REQUIRE(b.w == 4.0);
}

TEST_CASE("Get the ceil of a 4-dimensional vector", "[Vector4]")
{
    Vector4 a(1.8, 2.1, 3.5, 4.1);
    Vector4 b(a.ceil());
    REQUIRE(b.x == 2.0);
    REQUIRE(b.y == 3.0);
    REQUIRE(b.z == 4.0);
    REQUIRE(b.w == 5.0);
}

TEST_CASE("Get the minimum of two 4-dimensional vectors", "[Vector4]")
{
    Vector4 a(1.0, 3.0, 3.0, 5.0);
    Vector4 b(2.0, 2.0, 4.0, 4.0);
    Vector4 c(a.min(b));
    REQUIRE(c.x == 1.0);
    REQUIRE(c.y == 2.0);
    REQUIRE(c.z == 3.0);
    REQUIRE(c.w == 4.0);
}

TEST_CASE("Get the maximum of two 4-dimensional vectors", "[Vector4]")
{
    Vector4 a(1.0, 3.0, 3.0, 5.0);
    Vector4 b(2.0, 2.0, 4.0, 4.0);
    Vector4 c(a.max(b));
    REQUIRE(c.x == 2.0);
    REQUIRE(c.y == 3.0);
    REQUIRE(c.z == 4.0);
    REQUIRE(c.w == 5.0);
}

TEST_CASE("Get the absolute value of a 4-dimensional vector", "[Vector4]")
{
    Vector4 a(-1.0, -2.0, -3.0, -4.0);
    a = a.abs();
    REQUIRE(a.x == 1.0);
    REQUIRE(a.y == 2.0);
    REQUIRE(a.z == 3.0);
    REQUIRE(a.w == 4.0);
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
    REQUIRE(Vector4::UnitX.angleFrom(Vector4::UnitX).degrees() == 0.0);
    REQUIRE(Vector4::UnitX.angleFrom(-Vector4::UnitX).degrees() == 180.0);

    REQUIRE(Vector4::UnitY.angleFrom(Vector4::UnitY).degrees() == 0.0);
    REQUIRE(Vector4::UnitY.angleFrom(-Vector4::UnitY).degrees() == 180.0);

    REQUIRE(Vector4::UnitZ.angleFrom(Vector4::UnitZ).degrees() == 0.0);
    REQUIRE(Vector4::UnitZ.angleFrom(-Vector4::UnitZ).degrees() == 180.0);

    REQUIRE(Vector4::UnitW.angleFrom(Vector4::UnitW).degrees() == 0.0);
    REQUIRE(Vector4::UnitW.angleFrom(-Vector4::UnitW).degrees() == 180.0);

    REQUIRE(Vector4::UnitX.angleFrom(Vector4::UnitY).degrees() == 90.0);
    REQUIRE(Vector4::UnitX.angleFrom(Vector4::UnitZ).degrees() == 90.0);
    REQUIRE(Vector4::UnitX.angleFrom(Vector4::UnitW).degrees() == 90.0);
    REQUIRE(Vector4::UnitY.angleFrom(Vector4::UnitX).degrees() == 90.0);
    REQUIRE(Vector4::UnitY.angleFrom(Vector4::UnitZ).degrees() == 90.0);
    REQUIRE(Vector4::UnitY.angleFrom(Vector4::UnitW).degrees() == 90.0);
    REQUIRE(Vector4::UnitZ.angleFrom(Vector4::UnitX).degrees() == 90.0);
    REQUIRE(Vector4::UnitZ.angleFrom(Vector4::UnitY).degrees() == 90.0);
    REQUIRE(Vector4::UnitZ.angleFrom(Vector4::UnitW).degrees() == 90.0);
    REQUIRE(Vector4::UnitW.angleFrom(Vector4::UnitX).degrees() == 90.0);
    REQUIRE(Vector4::UnitW.angleFrom(Vector4::UnitY).degrees() == 90.0);
    REQUIRE(Vector4::UnitW.angleFrom(Vector4::UnitZ).degrees() == 90.0);
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