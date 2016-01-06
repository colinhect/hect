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
#include <Hect/Math/Rectangle.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Construct a default rectangle", "[Rectangle]")
{
    Rectangle a;

    REQUIRE(a.minimum() == Vector2::Zero);
    REQUIRE(a.maximum() == Vector2::Zero);
    REQUIRE(a.center() == Vector2::Zero);
    REQUIRE(a.size() == Vector2::Zero);
}

TEST_CASE("Construct a rectangle from minimum and maximum vector components", "[Rectangle]")
{
    Rectangle a(4.0, 8.0, 16.0, 32.0);

    REQUIRE(a.minimum() == Vector2(4.0, 8.0));
    REQUIRE(a.maximum() == Vector2(16.0, 32.0));
    REQUIRE(a.size() == Vector2(12.0, 24.0));
}

TEST_CASE("Construct a rectangle from minimum and maximum vectors", "[Rectangle]")
{
    Vector2 minimum(4.0, 8.0);
    Vector2 maximum(16.0, 32.0);

    Rectangle a(minimum, maximum);

    REQUIRE(a.minimum() == minimum);
    REQUIRE(a.maximum() == maximum);
    REQUIRE(a.size() == maximum - minimum);
}

TEST_CASE("Intersect disjoint rectangles", "[Rectangle]")
{
    Rectangle a(0.0, 0.0, 16.0, 32.0);
    Rectangle b(16.0, 32.0, 32.0, 48.0);

    Rectangle c = a.intersect(b);

    REQUIRE(c.size() == Vector2::Zero);
}

TEST_CASE("Intersect overlapping rectangles", "[Rectangle]")
{
    Rectangle a(0.0, 0.0, 8.0, 16.0);
    Rectangle b(2.0, 4.0, 8.0, 16.0);

    Rectangle c = a.intersect(b);

    REQUIRE(c.minimum() == Vector2(2.0, 4.0));
    REQUIRE(c.maximum() == Vector2(8.0, 16.0));
    REQUIRE(c.size() == Vector2(6.0, 12.0));
}

TEST_CASE("Intersect overlapping rectangles is semetric", "[Rectangle]")
{
    Rectangle a(0.0, 0.0, 8.0, 16.0);
    Rectangle b(2.0, 4.0, 8.0, 16.0);

    Rectangle c = a.intersect(b);
    Rectangle d = b.intersect(a);

    REQUIRE(c == d);
}
