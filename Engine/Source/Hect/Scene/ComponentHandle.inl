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
#include "Hect/Core/Exception.h"

namespace hect
{

template <typename T>
ComponentHandle<T>::ComponentHandle()
{
}

template <typename T>
T& ComponentHandle<T>::operator*()
{
    ensureValid();
    return *_component;
}

template <typename T>
const T& ComponentHandle<T>::operator*() const
{
    ensureValid();
    return *_component;
}

template <typename T>
T* ComponentHandle<T>::operator->()
{
    ensureValid();
    return _component;
}

template <typename T>
const T* ComponentHandle<T>::operator->() const
{
    ensureValid();
    return _component;
}

template <typename T>
bool ComponentHandle<T>::operator==(const ComponentHandle& other) const
{
    return _component == other._component;
}

template <typename T>
bool ComponentHandle<T>::operator!=(const ComponentHandle& other) const
{
    return _component != other._component;
}

template <typename T>
ComponentHandle<T>::operator bool() const
{
    return isValid();
}

template <typename T>
ComponentHandle<T>::ComponentHandle(T& component) :
    _component(&component),
    _valid(std::make_shared<std::atomic_bool>(true))
{
}

template <typename T>
bool ComponentHandle<T>::isValid() const
{
    return _component && _valid && *_valid;
}

template <typename T>
void ComponentHandle<T>::ensureValid() const
{
    if (!isValid())
    {
        throw InvalidOperation("Invalid component handle");
    }
}

template <typename T>
void ComponentHandle<T>::invalidate()
{
    ensureValid();
    _valid->store(false);
}

}
