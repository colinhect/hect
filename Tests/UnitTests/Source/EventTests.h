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
SUITE(Event)
{
    enum class EventA
    {
        A = 1,
        B = 3
    };

    enum class EventB
    {
        A = 2,
        B = 4
    };

    class TestListener :
        public Listener<EventA>,
        public Listener<EventB>
    {
    public:

        void receiveEvent(const EventA& event)
        {
            lastEventA = event;
        }

        void receiveEvent(const EventB& event)
        {
            lastEventB = event;
        }

        EventA lastEventA;
        EventB lastEventB;
    };

    TEST(AddNotifyAndRemove)
    {
        Dispatcher<EventA> dispatcher;

        TestListener listener;

        dispatcher.addListener(listener);
        dispatcher.notifyEvent(EventA::A);
        CHECK(EventA::A == listener.lastEventA);
        dispatcher.notifyEvent(EventA::B);
        CHECK(EventA::B == listener.lastEventA);
        
        dispatcher.removeListener(listener);
        dispatcher.notifyEvent(EventA::A);

        CHECK(EventA::B == listener.lastEventA);
    }

    TEST(MultipleListenerTypes)
    {
        Dispatcher<EventA> dispatcherA;
        Dispatcher<EventB> dispatcherB;

        TestListener listener;
        
        dispatcherA.addListener(listener);
        dispatcherB.addListener(listener);
        
        dispatcherA.notifyEvent(EventA::A);
        CHECK(EventA::A == listener.lastEventA);
        
        dispatcherB.notifyEvent(EventB::A);
        CHECK(EventB::A == listener.lastEventB);

        dispatcherB.notifyEvent(EventB::B);
        CHECK(EventB::B == listener.lastEventB);
    }

    TEST(AddRegisteredListener)
    {
        Dispatcher<EventA> dispatcher;

        TestListener listener;
        
        dispatcher.addListener(listener);
        dispatcher.addListener(listener);

        dispatcher.notifyEvent(EventA::A);
        CHECK(EventA::A == listener.lastEventA);
    }

    TEST(RemoveUnregisteredListener)
    {
        Dispatcher<EventA> dispatcher;

        TestListener listener;
        dispatcher.removeListener(listener);
    }
}
