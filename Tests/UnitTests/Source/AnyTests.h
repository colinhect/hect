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
SUITE(Any)
{
    TEST(HasValue)
    {
        Any a;
        CHECK(!a.hasValue());

        Any b(5);
        CHECK(b.hasValue());
    }

    TEST(IsType)
    {
        Any a;
        CHECK(!a.isType<int>());

        Any b(5);
        CHECK(b.isType<int>());
    }

    TEST(As)
    {
        Any a(5);
        CHECK_EQUAL(5, a.as<int>());
    }

    TEST(AsError)
    {
        Any a(5);

        bool errorThrown = false;
        try
        {
            a.as<double>();
        }
        catch (Error&)
        {
            errorThrown = true;
        }

        CHECK(errorThrown);
    }

    TEST(Copy)
    {
        Any a(5);
        Any b(a);

        CHECK_EQUAL(5, a.as<int>());
        CHECK_EQUAL(5, b.as<int>());
    }

    TEST(Assign)
    {
        Any a(5);
        CHECK_EQUAL(5, a.as<int>());

        a = std::string("Testing");
        CHECK_EQUAL("Testing", a.as<std::string>());
    }
}