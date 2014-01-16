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
SUITE(Path)
{
    TEST(DefaultConstructor)
    {
        Path path;

        CHECK_EQUAL("", path.toString());
    }

    TEST(ConstructorWithOnePart)
    {
        Path path("Data");

        CHECK_EQUAL("Data", path.toString());
    }

    TEST(ConstructorWithMultipleParts)
    {
        Path path("Data/Internal/Fail.log");

        CHECK_EQUAL("Data/Internal/Fail.log", path.toString());
    }

    TEST(ConstructorWithLeadingSlash)
    {
        Path path("/Data/Internal/Fail.log");

        CHECK_EQUAL("Data/Internal/Fail.log", path.toString());
    }

    TEST(ConstructorWithTrailingSlash)
    {
        Path path("Data/Internal/");

        CHECK_EQUAL("Data/Internal", path.toString());
    }

    TEST(ConstructorWithLeadingAndTrailingSlash)
    {
        Path path("/Data/Internal/");

        CHECK_EQUAL("Data/Internal", path.toString());
    }

    TEST(Extension)
    {
        Path path("Data/Internal/Fail.log");

        CHECK_EQUAL("log", path.extension());
    }

    TEST(NoExtension)
    {
        Path path("Data/Internal/Fail");

        CHECK_EQUAL("", path.extension());
    }

    TEST(Add)
    {
        Path path = Path("Data") + Path("Internal/Fail.log");

        CHECK_EQUAL("Data/Internal/Fail.log", path.toString());
    }

    TEST(AddWithEmptyFirst)
    {
        Path path = Path() + Path("Internal/Fail.log");

        CHECK_EQUAL("Internal/Fail.log", path.toString());
    }

    TEST(AddWithEmptySecond)
    {
        Path path = Path("Data/Internal") + Path();

        CHECK_EQUAL("Data/Internal", path.toString());
    }

    TEST(AddEquals)
    {
        Path path("Data");
        path += "Internal/Fail.log";

        CHECK_EQUAL("Data/Internal/Fail.log", path.toString());
    }

    TEST(AddEqualsFromEmpty)
    {
        Path path;
        path += "Data";
        path += "Internal/Fail.log";

        CHECK_EQUAL("Data/Internal/Fail.log", path.toString());
    }

    TEST(StreamOutput)
    {
        Path path("/Data/Internal/Fail.log");

        std::stringstream ss;
        ss << path;

        CHECK_EQUAL("Data/Internal/Fail.log", ss.str());
    }
}