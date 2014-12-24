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

#include <vector>

#include "Hect/Core/Error.h"

namespace hect
{

template <typename T>
class Dispatcher;

///
/// An listener of specific event.
template <typename T>
class Listener
{
    friend class Dispatcher<T>;
public:

    ///
    /// Unregister the listener from all Dispatcher%s that the listener is
    /// registered to.
    virtual ~Listener();

    ///
    /// Notifies the listener of an event.
    ///
    /// \param event The event.
    virtual void receiveEvent(const T& event) = 0;

private:
    void addDispatcher(Dispatcher<T>& dispatcher);
    void removeDispatcher(Dispatcher<T>& dispatcher);

    std::vector<Dispatcher<T>*> _dispatchers;
};

///
/// Notifies registered Listener%s of specific events.
template <typename T>
class Dispatcher
{
public:

    ///
    /// Unregisters all Listener%s that are registered to the dispatcher.
    virtual ~Dispatcher();

    ///
    /// Registers a Listener to receive events from the dispatcher.
    ///
    /// \param listener The listener to register.
    ///
    /// \throws Error If the listener is already registered to the dispatcher.
    void addListener(Listener<T>& listener);

    ///
    /// Un-registers a Listener from receiving events from the dispatcher.
    ///
    /// \param listener The listener to un-register.
    ///
    /// \throws Error If the listener is not registered to the dispatcher.
    void removeListener(Listener<T>& listener);

    ///
    /// Dispatches an event to all registered Listener%s.
    ///
    /// \param event The event.
    void dispatchEvent(const T& event);

private:
    std::vector<Listener<T>*> _listeners;
};

}

#include "Event.inl"