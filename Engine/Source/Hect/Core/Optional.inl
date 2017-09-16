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
Optional<T>::Optional()
{
}

template <typename T>
Optional<T>::Optional(const T& value) :
    _has_value(true),
    _value(value)
{
}

template <typename T>
Optional<T>::Optional(const Optional<T>& optional) :
    _has_value(optional._has_value),
    _value(optional._value)
{
}

template <typename T>
Optional<T>::Optional(Optional<T>&& optional) :
    _has_value(optional._has_value),
    _value(std::move(optional._value))
{
}

template <typename T>
bool Optional<T>::has_value() const
{
    return _has_value;
}

template <typename T>
T& Optional<T>::value()
{
    if (!_has_value)
    {
        throw InvalidOperation("No value in optional container");
    }

    return _value;
}

template <typename T>
const T& Optional<T>::value() const
{
    return const_cast<Optional<T>*>(this)->value();
}

template <typename T>
Optional<T>::operator bool() const
{
    return has_value();
}

template <typename T>
T& Optional<T>::operator*()
{
    return value();
}

template <typename T>
const T& Optional<T>::operator*() const
{
    return value();
}

template <typename T>
T* Optional<T>::operator->()
{
    return &value();
}

template <typename T>
const T* Optional<T>::operator->() const
{
    return &value();
}

template <typename T>
Optional<T>& Optional<T>::operator=(const T& value)
{
    _has_value = true;
    _value = value;
    return *this;
}

template <typename T>
Optional<T>& Optional<T>::operator=(T&& value)
{
    _has_value = true;
    _value = std::move(value);
    return *this;
}

template <typename T>
Optional<T>& Optional<T>::operator=(const Optional<T>& optional)
{
    _has_value = optional._has_value;
    if (_has_value)
    {
        _value = optional._value;
    }
    return *this;
}

template <typename T>
Optional<T>& Optional<T>::operator=(Optional<T>&& optional)
{
    _has_value = optional._has_value;
    if (_has_value)
    {
        _value = std::move(optional._value);
    }
    return *this;
}

}
