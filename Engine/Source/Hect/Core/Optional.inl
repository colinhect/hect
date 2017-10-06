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

template <typename Type>
Optional<Type>::Optional()
{
}

template <typename Type>
Optional<Type>::Optional(const Type& value) :
    _has_value(true),
    _value(value)
{
}

template <typename Type>
Optional<Type>::Optional(const Optional<Type>& optional) :
    _has_value(optional._has_value),
    _value(optional._value)
{
}

template <typename Type>
Optional<Type>::Optional(Optional<Type>&& optional) :
    _has_value(optional._has_value),
    _value(std::move(optional._value))
{
}

template <typename Type>
bool Optional<Type>::has_value() const
{
    return _has_value;
}

template <typename Type>
Type& Optional<Type>::value()
{
    if (!_has_value)
    {
        throw InvalidOperation("No value in optional container");
    }

    return _value;
}

template <typename Type>
const Type& Optional<Type>::value() const
{
    return const_cast<Optional<Type>*>(this)->value();
}

template <typename Type>
Optional<Type>::operator bool() const
{
    return has_value();
}

template <typename Type>
Type& Optional<Type>::operator*()
{
    return value();
}

template <typename Type>
const Type& Optional<Type>::operator*() const
{
    return value();
}

template <typename Type>
Type* Optional<Type>::operator->()
{
    return &value();
}

template <typename Type>
const Type* Optional<Type>::operator->() const
{
    return &value();
}

template <typename Type>
Optional<Type>& Optional<Type>::operator=(const Type& value)
{
    _has_value = true;
    _value = value;
    return *this;
}

template <typename Type>
Optional<Type>& Optional<Type>::operator=(Type&& value)
{
    _has_value = true;
    _value = std::move(value);
    return *this;
}

template <typename Type>
Optional<Type>& Optional<Type>::operator=(const Optional<Type>& optional)
{
    _has_value = optional._has_value;
    if (_has_value)
    {
        _value = optional._value;
    }
    return *this;
}

template <typename Type>
Optional<Type>& Optional<Type>::operator=(Optional<Type>&& optional)
{
    _has_value = optional._has_value;
    if (_has_value)
    {
        _value = std::move(optional._value);
    }
    return *this;
}

}
