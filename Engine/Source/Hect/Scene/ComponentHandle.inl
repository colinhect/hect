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

template <typename ComponentType>
ComponentHandle<ComponentType>::ComponentHandle()
{
}

template <typename ComponentType>
ComponentType& ComponentHandle<ComponentType>::operator*()
{
    ensure_valid();
    return *_component;
}

template <typename ComponentType>
const ComponentType& ComponentHandle<ComponentType>::operator*() const
{
    ensure_valid();
    return *_component;
}

template <typename ComponentType>
ComponentType* ComponentHandle<ComponentType>::operator->()
{
    ensure_valid();
    return _component;
}

template <typename ComponentType>
const ComponentType* ComponentHandle<ComponentType>::operator->() const
{
    ensure_valid();
    return _component;
}

template <typename ComponentType>
bool ComponentHandle<ComponentType>::operator==(const ComponentHandle& other) const
{
    return _component == other._component;
}

template <typename ComponentType>
bool ComponentHandle<ComponentType>::operator!=(const ComponentHandle& other) const
{
    return _component != other._component;
}

template <typename ComponentType>
ComponentHandle<ComponentType>::operator bool() const
{
    return is_valid();
}

template <typename ComponentType>
ComponentHandle<ComponentType>::ComponentHandle(ComponentType& component) :
    _component(&component),
    _valid(std::make_shared<std::atomic_bool>(true))
{
}

template <typename ComponentType>
bool ComponentHandle<ComponentType>::is_valid() const
{
    return _component && _valid && *_valid;
}

template <typename ComponentType>
void ComponentHandle<ComponentType>::ensure_valid() const
{
    if (!is_valid())
    {
        throw InvalidOperation("Invalid component handle");
    }
}

template <typename ComponentType>
void ComponentHandle<ComponentType>::invalidate()
{
    ensure_valid();
    _valid->store(false);
}

}
