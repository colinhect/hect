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
#include <Hect/Graphics/Color.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Construct a default color", "[Color]")
{
    Color a;
    REQUIRE(a.r == 0.0);
    REQUIRE(a.g == 0.0);
    REQUIRE(a.b == 0.0);
    REQUIRE(a.a == 0.0);
}

TEST_CASE("Construct a color given its components", "[Color]")
{
    Color a(1.0, 2.0, 3.0, 4.0);
    REQUIRE(a.r == 1.0);
    REQUIRE(a.g == 2.0);
    REQUIRE(a.b == 3.0);
    REQUIRE(a.a == 4.0);
}

TEST_CASE("Copy a color", "[Color]")
{
    Color a(1.0, 2.0, 3.0, 4.0);
    Color b(a);
    REQUIRE(b.r == a.r);
    REQUIRE(b.g == a.g);
    REQUIRE(b.b == a.b);
    REQUIRE(b.a == a.a);
}

TEST_CASE("Compute the sum of two colors", "[Color]")
{
    Color a = Color(1.0, 2.0, 3.0, 4.0) + Color(2.0, 4.0, 6.0, 8.0);
    REQUIRE(a.r == 3.0);
    REQUIRE(a.g == 6.0);
    REQUIRE(a.b == 9.0);
    REQUIRE(a.a == 12.0);
}

TEST_CASE("Add a color to a color", "[Color]")
{
    Color a = Color(1.0, 2.0, 3.0, 4.0);
    a += Color(2.0, 4.0, 6.0, 8.0);
    REQUIRE(a.r == 3.0);
    REQUIRE(a.g == 6.0);
    REQUIRE(a.b == 9.0);
    REQUIRE(a.a == 12.0);
}

TEST_CASE("Compute the difference between two colors", "[Color]")
{
    Color a = Color(1.0, 2.0, 3.0, 4.0) - Color(2.0, 4.0, 6.0, 8.0);
    REQUIRE(a.r == -1.0);
    REQUIRE(a.g == -2.0);
    REQUIRE(a.b == -3.0);
    REQUIRE(a.a == -4.0);
}

TEST_CASE("Subtract a color from a color", "[Color]")
{
    Color a = Color(1.0, 2.0, 3.0, 4.0);
    a -= Color(2.0, 4.0, 6.0, 8.0);
    REQUIRE(a.r == -1.0);
    REQUIRE(a.g == -2.0);
    REQUIRE(a.b == -3.0);
    REQUIRE(a.a == -4.0);
}

TEST_CASE("Compute the product of two colors", "[Color]")
{
    Color a = Color(1.0, 2.0, 3.0, 4.0) * Color(2.0, 4.0, 6.0, 8.0);
    REQUIRE(a.r == 2.0);
    REQUIRE(a.g == 8.0);
    REQUIRE(a.b == 18.0);
    REQUIRE(a.a == 32.0);
}

TEST_CASE("Multiply a color by a color", "[Color]")
{
    Color a = Color(1.0, 2.0, 3.0, 4.0);
    a *= Color(2.0, 4.0, 6.0, 8.0);
    REQUIRE(a.r == 2.0);
    REQUIRE(a.g == 8.0);
    REQUIRE(a.b == 18.0);
    REQUIRE(a.a == 32.0);
}

TEST_CASE("Compute the product of a color and a scalar", "[Color]")
{
    Color a = Color(1.0, 2.0, 3.0, 4.0) * 4.0;
    REQUIRE(a.r == 4.0);
    REQUIRE(a.g == 8.0);
    REQUIRE(a.b == 12.0);
    REQUIRE(a.a == 16.0);
}

TEST_CASE("Multiply a color by a scalar", "[Color]")
{
    Color a = Color(1.0, 2.0, 3.0, 4.0);
    a *= 4.0;
    REQUIRE(a.r == 4.0);
    REQUIRE(a.g == 8.0);
    REQUIRE(a.b == 12.0);
    REQUIRE(a.a == 16.0);
}

TEST_CASE("Compute the quotient of two colors", "[Color]")
{
    Color a = Color(6.0, 8.0, 12.0, 16.0) / Color(2.0, 4.0, 4.0, 8.0);
    REQUIRE(a.r == 3.0);
    REQUIRE(a.g == 2.0);
    REQUIRE(a.b == 3.0);
    REQUIRE(a.a == 2.0);
}

TEST_CASE("Divide a color by a color", "[Color]")
{
    Color a = Color(6.0, 8.0, 12.0, 16.0);
    a /= Color(2.0, 4.0, 4.0, 8.0);
    REQUIRE(a.r == 3.0);
    REQUIRE(a.g == 2.0);
    REQUIRE(a.b == 3.0);
    REQUIRE(a.a == 2.0);
}

TEST_CASE("Compute the quotient of a color and a scalar", "[Color]")
{
    Color a = Color(6.0, 8.0, 16.0, 24.0) / 2.0;
    REQUIRE(a.r == 3.0);
    REQUIRE(a.g == 4.0);
    REQUIRE(a.b == 8.0);
    REQUIRE(a.a == 12.0);
}

TEST_CASE("Divide a color by a scalar", "[Color]")
{
    Color a = Color(6.0, 8.0, 16.0, 24.0);
    a /= 2.0;
    REQUIRE(a.r == 3.0);
    REQUIRE(a.g == 4.0);
    REQUIRE(a.b == 8.0);
    REQUIRE(a.a == 12.0);
}

TEST_CASE("Index the components of a color", "[Color]")
{
    Color a(1.0, 2.0, 3.0, 4.0);

    REQUIRE(a[0] == 1.0);
    REQUIRE(a[1] == 2.0);
    REQUIRE(a[2] == 3.0);
    REQUIRE(a[3] == 4.0);
}