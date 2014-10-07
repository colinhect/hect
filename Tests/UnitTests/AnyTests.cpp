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
#include <Hect/Core/Any.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Any_HasValue")
{
    Any a;
    REQUIRE(!a.hasValue());

    Any b(5);
    REQUIRE(b.hasValue());
}

TEST_CASE("Any_IsType")
{
    Any a;
    REQUIRE(!a.isType<int>());

    Any b(5);
    REQUIRE(b.isType<int>());
}

TEST_CASE("Any_As")
{
    Any a(5);
    REQUIRE(a.as<int>() == 5);
}

TEST_CASE("Any_AsError")
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

    REQUIRE(errorThrown);
}

TEST_CASE("Any_Copy")
{
    Any a(5);
    Any b(a);

    REQUIRE(a.as<int>() == 5);
    REQUIRE(b.as<int>() == 5);
}

TEST_CASE("Any_Assign")
{
    Any a(5);
    REQUIRE(a.as<int>() == 5);

    a = std::string { "Testing" };
    REQUIRE(a.as<std::string>() == "Testing");
}