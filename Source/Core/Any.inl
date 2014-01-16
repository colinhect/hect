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
namespace hect
{

template <typename T>
Any::Any(const T& value) :
    _container(new ContainerValue<T>(value))
{
}

template <typename T>
Any& Any::operator=(const T& value)
{
    if (_container)
    {
        delete _container;
    }

    _container = new ContainerValue<T>(value);

    return *this;
}

template <typename T>
bool Any::isType() const
{
    if (_container)
    {
        return dynamic_cast<ContainerValue<T>*>(_container) != nullptr;
    }
    else
    {
        return false;
    }
}

template <typename T>
T& Any::as() const
{
    auto containerValue = dynamic_cast<ContainerValue<T>*>(_container);

    if (!containerValue)
    {
        throw Error("Invalid type");
    }

    return containerValue->held;
}

template <typename T>
Any::ContainerValue<T>::ContainerValue(const T& value) :
    held(value)
{
}

template <typename T>
Any::Container* Any::ContainerValue<T>::clone() const
{
    return new ContainerValue(held);
}

}