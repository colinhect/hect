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
    
template <typename ClassType>
MetaProperty<ClassType>::MetaProperty(const std::string& name) :
    _name(name)
{
}

template <typename ClassType>
MetaProperty<ClassType>::~MetaProperty()
{
}

template <typename ClassType>
const std::string& MetaProperty<ClassType>::name() const
{
    return _name;
}

template <typename ClassType>
template <typename PropertyType>
bool MetaProperty<ClassType>::isType() const
{
    return asType<PropertyType>() != nullptr;
}

template <typename ClassType>
template <typename PropertyType>
const TypedMetaProperty<ClassType, PropertyType>* MetaProperty<ClassType>::asType() const
{
    return dynamic_cast<const TypedMetaProperty<ClassType, PropertyType>*>(this);
}

template <typename ClassType>
template <typename PropertyType>
const PropertyType& MetaProperty<ClassType>::get(const ClassType& instance) const
{
    const TypedMetaProperty<ClassType, PropertyType>* typed = asType<PropertyType>();
    assert(typed);
    return typed->get(instance);
}

template <typename ClassType>
template <typename PropertyType>
void MetaProperty<ClassType>::set(ClassType& instance, const PropertyType& value) const
{
    const TypedMetaProperty<ClassType, PropertyType>* typed = asType<PropertyType>();
    assert(typed);
    typed->set(instance, value);
}

template <typename ClassType, typename PropertyType>
TypedMetaProperty<ClassType, PropertyType>::TypedMetaProperty(const std::string& name, Getter get, Setter set) :
    MetaProperty(name),
    _get(get),
    _set(set)
{
}

template <typename ClassType, typename PropertyType>
const PropertyType& TypedMetaProperty<ClassType, PropertyType>::get(const ClassType& instance) const
{
    return _get(instance);
}

template <typename ClassType, typename PropertyType>
void TypedMetaProperty<ClassType, PropertyType>::set(ClassType& instance, const PropertyType& value) const
{
    _set(instance, value);
}

}