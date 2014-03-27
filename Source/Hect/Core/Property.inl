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
#include <assert.h>

namespace hect
{

template <typename PropertyType>
bool Property::isType() const
{
    return asType<PropertyType>() != nullptr;
}

template <typename PropertyType>
const TypedProperty<PropertyType>* Property::asType() const
{
    return dynamic_cast<const TypedProperty<PropertyType>*>(this);
}

template <typename PropertyType, typename ClassType>
const PropertyType& Property::get(const ClassType& instance) const
{
    const TypedProperty<PropertyType>* typed = asType<PropertyType>();
    assert(typed);
    return typed->get(instance);
}

template <typename PropertyType, typename ClassType>
void Property::set(ClassType& instance, const PropertyType& value) const
{
    const TypedProperty<PropertyType>* typed = asType<PropertyType>();
    assert(typed);
    typed->set(instance, value);
}

template <typename PropertyType>
TypedProperty<PropertyType>::TypedProperty(const std::string& name, Getter getter, Setter setter) :
    Property(name),
    _getter(getter),
    _setter(setter)
{
}

template <typename PropertyType>
template <typename ClassType>
const PropertyType& TypedProperty<PropertyType>::get(const ClassType& instance) const
{
    return _getter(reinterpret_cast<const void*>(&instance));
}

template <typename PropertyType>
template <typename ClassType>
void TypedProperty<PropertyType>::set(ClassType& instance, const PropertyType& value) const
{
    _setter(reinterpret_cast<void*>(&instance), value);
}

}