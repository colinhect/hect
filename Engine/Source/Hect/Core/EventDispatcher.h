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

template <typename T>
class EventListener;

///
/// Notifies registered EventListener%s of specific events.
template <typename T>
class EventDispatcher
{
public:

    ///
    /// Unregisters all EventListener%s that are registered to the dispatcher.
    virtual ~EventDispatcher();

    ///
    /// Registers a EventListener to receive events from the dispatcher.
    ///
    /// \param listener The listener to register.
    ///
    /// \throws InvalidOperation If the listener is already registered to the
    /// dispatcher.
    void registerListener(EventListener<T>& listener);

    ///
    /// Un-registers a EventListener from receiving events from the dispatcher.
    ///
    /// \param listener The listener to un-register.
    ///
    /// \throws InvalidOperation If the listener is not registered to the
    /// dispatcher.
    void unregisterListener(EventListener<T>& listener);

    ///
    /// Dispatches an event to all registered EventListener%s.
    ///
    /// \param event The event.
    void dispatchEvent(const T& event);

private:
    std::vector<EventListener<T>*> _listeners;
};

}

#include "EventDispatcher.inl"
