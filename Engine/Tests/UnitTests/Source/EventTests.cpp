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
#include <Hect/Core/Event.h>
using namespace hect;

#include <catch.hpp>

enum class TestEventA
{
    A = 1,
    B = 3
};

enum class TestEventB
{
    A = 2,
    B = 4
};

class TestListener :
    public Listener<TestEventA>,
    public Listener<TestEventB>
{
public:

    void receiveEvent(const TestEventA& event)
    {
        lastTestEventA = event;
    }

    void receiveEvent(const TestEventB& event)
    {
        lastTestEventB = event;
    }

    TestEventA lastTestEventA;
    TestEventB lastTestEventB;
};

TEST_CASE("Register a listener, dispatch event, and unregister listener", "[Event]")
{
    Dispatcher<TestEventA> dispatcher;

    TestListener listener;

    dispatcher.registerListener(listener);
    dispatcher.dispatchEvent(TestEventA::A);
    REQUIRE(TestEventA::A == listener.lastTestEventA);
    dispatcher.dispatchEvent(TestEventA::B);
    REQUIRE(TestEventA::B == listener.lastTestEventA);

    dispatcher.unregisterListener(listener);
    dispatcher.dispatchEvent(TestEventA::A);

    REQUIRE(TestEventA::B == listener.lastTestEventA);
}

TEST_CASE("Register listener to multiple dispatcher types", "[Event]")
{
    Dispatcher<TestEventA> dispatcherA;
    Dispatcher<TestEventB> dispatcherB;

    TestListener listener;

    dispatcherA.registerListener(listener);
    dispatcherB.registerListener(listener);

    dispatcherA.dispatchEvent(TestEventA::A);
    REQUIRE(TestEventA::A == listener.lastTestEventA);

    dispatcherB.dispatchEvent(TestEventB::A);
    REQUIRE(TestEventB::A == listener.lastTestEventB);

    dispatcherB.dispatchEvent(TestEventB::B);
    REQUIRE(TestEventB::B == listener.lastTestEventB);
}

TEST_CASE("Register a listener that is already registered", "[Event]")
{
    Dispatcher<TestEventA> dispatcher;

    TestListener listener;

    dispatcher.registerListener(listener);
    REQUIRE_THROWS_AS(dispatcher.registerListener(listener), InvalidOperation);

    dispatcher.dispatchEvent(TestEventA::A);
    REQUIRE(TestEventA::A == listener.lastTestEventA);
}

TEST_CASE("Unregister a listener that is not registered", "[Event]")
{
    Dispatcher<TestEventA> dispatcher;

    TestListener listener;
    REQUIRE_THROWS_AS(dispatcher.unregisterListener(listener), InvalidOperation);
}