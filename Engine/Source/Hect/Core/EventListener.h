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
#pragma once

#include <vector>

#include "Hect/Core/Exception.h"

namespace hect
{

template <typename Type>
class EventDispatcher;

///
/// An listener of specific event.
template <typename Type>
class EventListener
{
    friend class EventDispatcher<Type>;
public:

    ///
    /// Unregister the listener from all EventDispatcher%s that the listener is
    /// registered to.
    virtual ~EventListener();

    ///
    /// Notifies the listener of an event.
    ///
    /// \param event The event.
    virtual void receive_event(const Type& event) = 0;

private:
    void add_dispatcher(EventDispatcher<Type>& dispatcher);
    void remove_dispatcher(EventDispatcher<Type>& dispatcher);

    std::vector<EventDispatcher<Type>*> _dispatchers;
};

}

#include "EventListener.inl"
