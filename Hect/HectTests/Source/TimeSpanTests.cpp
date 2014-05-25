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

TEST_CASE("TimeSpan_FromMicroseconds")
{
    TimeSpan t = TimeSpan::fromMicroseconds(1000);
    REQUIRE(t.microseconds() == 1000);
}

TEST_CASE("TimeSpan_FromMilliseconds")
{
    TimeSpan t = TimeSpan::fromMilliseconds(1000);
    REQUIRE(t.microseconds() == 1000000);
}

TEST_CASE("TimeSpan_FromSeconds")
{
    TimeSpan t = TimeSpan::fromSeconds((Real)0.001);
    REQUIRE(t.microseconds() == 1000);
}

TEST_CASE("TimeSpan_Milliseconds")
{
    TimeSpan t = TimeSpan::fromMicroseconds(1000);
    REQUIRE(t.milliseconds() == 1);
}

TEST_CASE("TimeSpan_Seconds")
{
    TimeSpan t = TimeSpan::fromMicroseconds(1000);
    REQUIRE(std::abs(t.seconds() - 0.001) < 0.01);
}

TEST_CASE("TimeSpan_Add")
{
    TimeSpan t = TimeSpan::fromMicroseconds(10) + TimeSpan::fromMicroseconds(5);
    REQUIRE(t.microseconds() == 15);
}

TEST_CASE("TimeSpan_AddEquals")
{
    TimeSpan t = TimeSpan::fromMicroseconds(10);
    t += TimeSpan::fromMicroseconds(5);
    REQUIRE(t.microseconds() == 15);
}

TEST_CASE("TimeSpan_Subtract")
{
    TimeSpan t = TimeSpan::fromMicroseconds(10) - TimeSpan::fromMicroseconds(5);
    REQUIRE(t.microseconds() == 5);
}

TEST_CASE("TimeSpan_SubtractEquals")
{
    TimeSpan t = TimeSpan::fromMicroseconds(10);
    t -= TimeSpan::fromMicroseconds(5);
    REQUIRE(t.microseconds() == 5);
}

TEST_CASE("TimeSpan_Multiply")
{
    TimeSpan t = TimeSpan::fromMicroseconds(10) * 2.5;
    REQUIRE(t.microseconds() == 25);
}

TEST_CASE("TimeSpan_MultiplyEquals")
{
    TimeSpan t = TimeSpan::fromMicroseconds(10);
    t *= 2.5;
    REQUIRE(t.microseconds() == 25);
}

TEST_CASE("TimeSpan_Divide")
{
    TimeSpan t = TimeSpan::fromMicroseconds(10) / 2.0;
    REQUIRE(t.microseconds() == 5);
}

TEST_CASE("TimeSpan_DivideEquals")
{
    TimeSpan t = TimeSpan::fromMicroseconds(10);
    t /= 2.0;
    REQUIRE(t.microseconds() == 5);
}

TEST_CASE("TimeSpan_Negate")
{
    TimeSpan t = -TimeSpan::fromMicroseconds(10);
    REQUIRE(t.microseconds() == -10);
}