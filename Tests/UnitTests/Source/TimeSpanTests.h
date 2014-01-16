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
SUITE(TimeSpan)
{
    TEST(FromMicroseconds)
    {
        TimeSpan t = TimeSpan::fromMicroseconds(1000);
        CHECK_EQUAL(1000, t.microseconds());
    }

    TEST(FromMilliseconds)
    {
        TimeSpan t = TimeSpan::fromMilliseconds(1000);
        CHECK_EQUAL(1000000, t.microseconds());
    }

    TEST(FromSeconds)
    {
        TimeSpan t = TimeSpan::fromSeconds(0.001);
        CHECK_EQUAL(1000, t.microseconds());
    }

    TEST(Milliseconds)
    {
        TimeSpan t = TimeSpan::fromMicroseconds(1000);
        CHECK_EQUAL(1, t.milliseconds());
    }

    TEST(Seconds)
    {
        TimeSpan t = TimeSpan::fromMicroseconds(1000);
        CHECK_CLOSE(0.001, t.seconds(), epsilon);
    }

    TEST(Add)
    {
        TimeSpan t = TimeSpan::fromMicroseconds(10) + TimeSpan::fromMicroseconds(5);
        CHECK_EQUAL(15, t.microseconds());
    }

    TEST(AddEquals)
    {
        TimeSpan t = TimeSpan::fromMicroseconds(10);
        t += TimeSpan::fromMicroseconds(5);
        CHECK_EQUAL(15, t.microseconds());
    }

    TEST(Subtract)
    {
        TimeSpan t = TimeSpan::fromMicroseconds(10) - TimeSpan::fromMicroseconds(5);
        CHECK_EQUAL(5, t.microseconds());
    }

    TEST(SubtractEquals)
    {
        TimeSpan t = TimeSpan::fromMicroseconds(10);
        t -= TimeSpan::fromMicroseconds(5);
        CHECK_EQUAL(5, t.microseconds());
    }

    TEST(Multiply)
    {
        TimeSpan t = TimeSpan::fromMicroseconds(10) * 2.5;
        CHECK_EQUAL(25, t.microseconds());
    }

    TEST(MultiplyEquals)
    {
        TimeSpan t = TimeSpan::fromMicroseconds(10);
        t *= 2.5;
        CHECK_EQUAL(25, t.microseconds());
    }

    TEST(Divide)
    {
        TimeSpan t = TimeSpan::fromMicroseconds(10) / 2.0;
        CHECK_EQUAL(5, t.microseconds());
    }

    TEST(DivideEquals)
    {
        TimeSpan t = TimeSpan::fromMicroseconds(10);
        t /= 2.0;
        CHECK_EQUAL(5, t.microseconds());
    }

    TEST(Negate)
    {
        TimeSpan t = -TimeSpan::fromMicroseconds(10);
        CHECK_EQUAL(-10, t.microseconds());
    }
}