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

#include <functional>
#include <string>
#include <vector>
#include <memory>

namespace hect
{

template <typename ClassType, typename PropertyType>
class TypedMetaProperty;

template <typename ClassType>
class MetaProperty
{
public:
    MetaProperty(const std::string& name);
    virtual ~MetaProperty();

    const std::string& name() const;

    template <typename PropertyType>
    bool isType() const;

    template <typename PropertyType>
    const TypedMetaProperty<ClassType, PropertyType>* asType() const;

    template <typename PropertyType>
    const PropertyType& get(const ClassType& instance) const;

    template <typename PropertyType>
    void set(ClassType& instance, const PropertyType& value) const;

private:
    std::string _name;
};

template <typename ClassType, typename PropertyType>
class TypedMetaProperty :
    public MetaProperty<ClassType>
{
public:
    typedef std::function<const PropertyType&(const ClassType&)> Getter;
    typedef std::function<void(ClassType&, const PropertyType&)> Setter;

    TypedMetaProperty(const std::string& name, Getter get, Setter set);

    const PropertyType& get(const ClassType& instance) const;
    void set(ClassType& instance, const PropertyType& value) const;

private:
    Getter _get;
    Setter _set;
};

}

#include "MetaProperty.inl"