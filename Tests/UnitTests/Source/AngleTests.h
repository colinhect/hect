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
#pragma once

SUITE(Angle)
{
    TEST(DefaultConstructor)
    {
        Angle a;
        CHECK_CLOSE(0, a.degrees(), epsilon);
        CHECK_CLOSE(0, a.radians(), epsilon);
    }

    TEST(FromAndToDegrees)
    {
        Angle a = Angle::fromDegrees(180);
        CHECK_CLOSE(180, a.degrees(), epsilon);
        CHECK_CLOSE(pi, a.radians(), epsilon);
    }

    TEST(FromAndToRadians)
    {
        Angle a = Angle::fromRadians(pi);
        CHECK_CLOSE(180, a.degrees(), epsilon);
        CHECK_CLOSE(pi, a.radians(), epsilon);
    }

    TEST(Negate)
    {
        Angle a = Angle::fromRadians(pi);
        a = -a;
        CHECK_CLOSE(-180, a.degrees(), epsilon);
        CHECK_CLOSE(-pi, a.radians(), epsilon);
    }

    TEST(Add)
    {
        Angle a = Angle::fromDegrees(45) + Angle::fromDegrees(135);
        CHECK_CLOSE(180, a.degrees(), epsilon);
        CHECK_CLOSE(pi, a.radians(), epsilon);
    }

    TEST(AddEquals)
    {
        Angle a = Angle::fromDegrees(45);
        a += Angle::fromDegrees(135);
        CHECK_CLOSE(180, a.degrees(), epsilon);
        CHECK_CLOSE(pi, a.radians(), epsilon);
    }

    TEST(Subtract)
    {
        Angle a = Angle::fromDegrees(270) - Angle::fromDegrees(90);
        CHECK_CLOSE(180, a.degrees(), epsilon);
        CHECK_CLOSE(pi, a.radians(), epsilon);
    }

    TEST(SubtractEquals)
    {
        Angle a = Angle::fromDegrees(270);
        a -= Angle::fromDegrees(90);
        CHECK_CLOSE(180, a.degrees(), epsilon);
        CHECK_CLOSE(pi, a.radians(), epsilon);
    }

    TEST(Multiply)
    {
        Angle a = Angle::fromDegrees(90) * 2;
        CHECK_CLOSE(180, a.degrees(), epsilon);
        CHECK_CLOSE(pi, a.radians(), epsilon);
    }

    TEST(MultiplyEquals)
    {
        Angle a = Angle::fromDegrees(90);
        a *= 2;
        CHECK_CLOSE(180, a.degrees(), epsilon);
        CHECK_CLOSE(pi, a.radians(), epsilon);
    }

    TEST(Divide)
    {
        Angle a = Angle::fromDegrees(360) / 2;
        CHECK_CLOSE(180, a.degrees(), epsilon);
        CHECK_CLOSE(pi, a.radians(), epsilon);
    }

    TEST(DivideEquals)
    {
        Angle a = Angle::fromDegrees(360);
        a /= 2;
        CHECK_CLOSE(180, a.degrees(), epsilon);
        CHECK_CLOSE(pi, a.radians(), epsilon);
    }
}