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
#include <algorithm>
#include <cassert>

namespace hect
{

template <typename T>
Listener<T>::~Listener()
{
    // Copy the vector so it will remain valid when items are removed from it
    auto dispatchers = _dispatchers;

    // Unregister the listener from all dispatchers
    for (Dispatcher<T>* dispatcher : dispatchers)
    {
        dispatcher->unregisterListener(*this);
    }
}

template <typename T>
void Listener<T>::addDispatcher(Dispatcher<T>& dispatcher)
{
    auto it = std::find(_dispatchers.begin(), _dispatchers.end(), &dispatcher);
    if (it != _dispatchers.end())
    {
        throw InvalidOperation("The listener is already registered to this dispatcher");
    }
    else
    {
        _dispatchers.push_back(&dispatcher);
    }
}

template <typename T>
void Listener<T>::removeDispatcher(Dispatcher<T>& dispatcher)
{
    auto it = std::find(_dispatchers.begin(), _dispatchers.end(), &dispatcher);
    if (it == _dispatchers.end())
    {
        throw InvalidOperation("The listener is not registered to this dispatcher");
    }
    else
    {
        _dispatchers.erase(it);
    }
}

template <typename T>
Dispatcher<T>::~Dispatcher()
{
    // Copy the vector so it will remain valid when items are removed from it
    auto listeners = _listeners;

    // Unregister all listeners from the dispatcher
    for (Listener<T>* listener : listeners)
    {
        unregisterListener(*listener);
    }
}

template <typename T>
void Dispatcher<T>::registerListener(Listener<T>& listener)
{
    auto it = std::find(_listeners.begin(), _listeners.end(), &listener);
    if (it != _listeners.end())
    {
        throw InvalidOperation("The listener is already registered to this dispatcher");
    }
    else
    {
        _listeners.push_back(&listener);
        listener.addDispatcher(*this);
    }
}

template <typename T>
void Dispatcher<T>::unregisterListener(Listener<T>& listener)
{
    auto it = std::find(_listeners.begin(), _listeners.end(), &listener);
    if (it == _listeners.end())
    {
        throw InvalidOperation("The listener is not registered to this dispatcher");
    }
    else
    {
        _listeners.erase(it);
        listener.removeDispatcher(*this);
    }
}

template <typename T>
void Dispatcher<T>::dispatchEvent(const T& event)
{
    for (Listener<T>* listener : _listeners)
    {
        listener->receiveEvent(event);
    }
}

}
