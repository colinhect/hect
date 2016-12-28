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
#include <Hect/Math/Constants.h>
#include <Hect/Units/Angle.h>
#include <Hect/Units/Distance.h>
#include <Hect/Units/Time.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Convert and compare equivalent angle units", "[Unit]")
{
    RadiansNew radians(Pi);
    DegreesNew degrees(180.0);

    REQUIRE(radians == RadiansNew(degrees));
    REQUIRE(degrees == DegreesNew(radians));
}

TEST_CASE("Perform basic arithmetic on angle units", "[Unit]")
{
    RadiansNew radians(Pi);
    REQUIRE(radians * 2.0 == radians + radians);
}

TEST_CASE("Convert and compare equivalent time units", "[Unit]")
{
    Hours hours(1.0);
    Minutes minutes(1.0 * 60.0);
    Seconds seconds(1.0 * 60.0 * 60.0);

    REQUIRE(seconds == Seconds(minutes));
    REQUIRE(seconds == Seconds(hours));
    REQUIRE(minutes == Minutes(seconds));
    REQUIRE(minutes == Minutes(hours));
    REQUIRE(hours == Hours(seconds));
    REQUIRE(hours == Hours(minutes));
}

TEST_CASE("Perform basic arithmetic on time units", "[Unit]")
{
    Seconds seconds(60.0);
    REQUIRE(seconds * 2.0 == seconds + seconds);
}
