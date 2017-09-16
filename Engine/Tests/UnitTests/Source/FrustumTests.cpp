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
#include <Hect/Math/Frustum.h>
#include <Hect/Units/Angle.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Test an axis-aligned box behind a frustum", "[Frustum]")
{
    Frustum frustum(
        Vector3(0, 0, 0),
        Vector3(0, 0, -1),
        Vector3(0, 1, 0),
        Degrees(90),
        1,
        0.1,
        100);

    AxisAlignedBox box(Vector3(-1, -1, 10), Vector3(1, 1, 11));

    REQUIRE(FrustumTestResult::Outside == frustum.test_axis_aligned_box(box));
}

TEST_CASE("Test an axis-aligned box on the outside-left of a frustum", "[Frustum]")
{
    Frustum frustum(
        Vector3(0, 0, 0),
        Vector3(0, 0, -1),
        Vector3(0, 1, 0),
        Degrees(90),
        1,
        0.1,
        100);

    AxisAlignedBox box(Vector3(-100, -1, -11), Vector3(-101, 1, -10));

    REQUIRE(FrustumTestResult::Outside == frustum.test_axis_aligned_box(box));
}

TEST_CASE("Test an axis-aligned box on the inside of a frustum", "[Frustum]")
{
    Frustum frustum(
        Vector3(0, 0, 0),
        Vector3(0, 0, -1),
        Vector3(0, 1, 0),
        Degrees(90),
        1,
        0.1,
        100);

    AxisAlignedBox box(Vector3(-1, -1, -50), Vector3(1, 1, -51));

    REQUIRE(FrustumTestResult::Inside == frustum.test_axis_aligned_box(box));
}

TEST_CASE("Test an axis-aligned box intersecting a frustum", "[Frustum]")
{
    Frustum frustum(
        Vector3(0, 0, 0),
        Vector3(0, 0, -1),
        Vector3(0, 1, 0),
        Degrees(90),
        1,
        0.1,
        100);

    AxisAlignedBox box(Vector3(-1, -1, -200), Vector3(1, 1, -51));

    REQUIRE(FrustumTestResult::Intersect == frustum.test_axis_aligned_box(box));
}
