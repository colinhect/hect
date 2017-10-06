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
Any::Any(const Type& value) :
    _container(new ContainerValue<Type>(value))
{
}

template <typename Type>
Any& Any::operator=(const Type& value)
{
    if (_container)
    {
        delete _container;
    }

    _container = new ContainerValue<Type>(value);

    return *this;
}

template <typename Type>
bool Any::is_type() const
{
    if (_container)
    {
        return dynamic_cast<ContainerValue<Type>*>(_container) != nullptr;
    }
    else
    {
        return false;
    }
}

template <typename Type>
Type& Any::as() const
{
    auto container_value = dynamic_cast<ContainerValue<Type>*>(_container);

    if (!container_value)
    {
        throw InvalidOperation("Invalid type");
    }

    return container_value->held;
}

template <typename Type>
Any::ContainerValue<Type>::ContainerValue(const Type& value) :
    held(value)
{
}

template <typename Type>
Any::Container* Any::ContainerValue<Type>::clone() const
{
    return new ContainerValue(held);
}

}