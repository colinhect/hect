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

namespace hect
{

///
/// An event dispatcher which notifies registered listeners of specific events.
template <typename T>
class Dispatcher
{
public:

    ///
    /// Registers a listener to receive events notified from the dispatcher.
    ///
    /// \note If the listener is already registered to the dispatcher then
    /// there is no effect.
    ///
    /// \param listener The listener to register.
    void addListener(Listener<T>& listener);
    
    ///
    /// Unregisters a listener from receiving events notified from the
    /// dispatcher.
    ///
    /// \note If the listener is not registered to the dispatcher then there is
    /// no effect.
    ///
    /// \param listener The listener to unregister.
    void removeListener(Listener<T>& listener);

    ///
    /// Notifies an event to all registered listeners.
    ///
    /// \param event The event.
    void notifyEvent(const T& event);

private:
    std::vector<Listener<T>*> _listeners;
};

}

#include "Dispatcher.inl"