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
#include <Hect/Core/StringMap.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Get unhashed value from a string map", "[StringMap]")
{
    StringMap<int> map;
    map.set(std::string("A"), 1);

    SECTION("Not constant")
    {
        StringMap<int>::Iterator it = map.get(std::string("A"));
        REQUIRE(it);
        REQUIRE(*it == 1);
    }

    SECTION("Constant")
    {
        const StringMap<int>& constMap = map;
        StringMap<int>::ConstIterator it = constMap.get(std::string("A"));
        REQUIRE(it);
        REQUIRE(*it == 1);
    }
}

TEST_CASE("Get unhashed non-existing value from a string map", "[StringMap]")
{
    StringMap<int> map;

    SECTION("Not constant")
    {
        StringMap<int>::Iterator it = map.get(std::string("B"));
        REQUIRE(!it);
    }

    SECTION("Not constant")
    {
        const StringMap<int>& constMap = map;
        StringMap<int>::ConstIterator it = constMap.get(std::string("B"));
        REQUIRE(!it);
    }
}

TEST_CASE("Get hashed value from a string map", "[StringMap]")
{
    StringMap<int> map;
    map.set(std::string("A"), 1);

    SECTION("Not constant")
    {
        for (int i = 0; i < 10; ++i)
        {
            StringMap<int>::Iterator it = map.get("A");
            REQUIRE(it);
            REQUIRE(*it == 1);
        }
    }

    SECTION("Constant")
    {
        for (int i = 0; i < 10; ++i)
        {
            const StringMap<int>& constMap = map;
            StringMap<int>::ConstIterator it = constMap.get("A");
            REQUIRE(it);
            REQUIRE(*it == 1);
        }
    }
}

TEST_CASE("Get hashed non-existing value from a string map", "[StringMap]")
{
    StringMap<int> map;

    SECTION("Not constant")
    {
        StringMap<int>::Iterator it = map.get("B");
        REQUIRE(!it);
    }

    SECTION("Constant")
    {
        const StringMap<int>& constMap = map;
        StringMap<int>::ConstIterator it = constMap.get("B");
        REQUIRE(!it);
    }
}