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
#include <Hect/Core/EventDispatcher.h>
#include <Hect/Core/EventListener.h>
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
    public EventListener<TestEventA>,
    public EventListener<TestEventB>
{
public:

    void receive_event(const TestEventA& event) override
    {
        last_test_event_a = event;
    }

    void receive_event(const TestEventB& event) override
    {
        last_test_event_b = event;
    }

    TestEventA last_test_event_a;
    TestEventB last_test_event_b;
};

TEST_CASE("Register a listener, dispatch event, and unregister listener", "[Event]")
{
    EventDispatcher<TestEventA> dispatcher;

    TestListener listener;

    dispatcher.register_listener(listener);
    dispatcher.dispatch_event(TestEventA::A);
    REQUIRE(TestEventA::A == listener.last_test_event_a);
    dispatcher.dispatch_event(TestEventA::B);
    REQUIRE(TestEventA::B == listener.last_test_event_a);

    dispatcher.unregister_listener(listener);
    dispatcher.dispatch_event(TestEventA::A);

    REQUIRE(TestEventA::B == listener.last_test_event_a);
}

TEST_CASE("Register listener to multiple dispatcher types", "[Event]")
{
    EventDispatcher<TestEventA> dispatcher_a;
    EventDispatcher<TestEventB> dispatcher_b;

    TestListener listener;

    dispatcher_a.register_listener(listener);
    dispatcher_b.register_listener(listener);

    dispatcher_a.dispatch_event(TestEventA::A);
    REQUIRE(TestEventA::A == listener.last_test_event_a);

    dispatcher_b.dispatch_event(TestEventB::A);
    REQUIRE(TestEventB::A == listener.last_test_event_b);

    dispatcher_b.dispatch_event(TestEventB::B);
    REQUIRE(TestEventB::B == listener.last_test_event_b);
}

TEST_CASE("Register a listener that is already registered", "[Event]")
{
    EventDispatcher<TestEventA> dispatcher;

    TestListener listener;

    dispatcher.register_listener(listener);
    REQUIRE_THROWS_AS(dispatcher.register_listener(listener), InvalidOperation);

    dispatcher.dispatch_event(TestEventA::A);
    REQUIRE(TestEventA::A == listener.last_test_event_a);
}

TEST_CASE("Unregister a listener that is not registered", "[Event]")
{
    EventDispatcher<TestEventA> dispatcher;

    TestListener listener;
    REQUIRE_THROWS_AS(dispatcher.unregister_listener(listener), InvalidOperation);
}
