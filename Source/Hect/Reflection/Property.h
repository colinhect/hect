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

#include "Hect/Core/Export.h"

namespace hect
{

template <typename PropertyType>
class TypedProperty;

///
/// A property exposed by a reflector.
class Property
{
public:

    ///
    /// An array of properties.
    typedef std::vector<Property*> Array;

    ///
    /// Constructs a property given a name.
    ///
    /// \param name The name of the property.
    HECT_API Property(const std::string& name);

    HECT_API virtual ~Property();

    ///
    /// Returns the name of the property.
    HECT_API const std::string& name() const;

    ///
    /// Returns whether the property is of the given type.
    template <typename PropertyType>
    bool isType() const;

    ///
    /// Returns a typed property based on the given type.
    template <typename PropertyType>
    const TypedProperty<PropertyType>* asType() const;

    ///
    /// Returns the value of the property for a specific instance of the
    /// reflected class.
    ///
    /// \warning Calling this function with an object which is not an instance
    /// of the reflected class results in undefined behavior.
    ///
    /// \param instance The instance of the reflected class to get the
    /// property value for.
    template <typename PropertyType, typename ClassType>
    const PropertyType& get(const ClassType& instance) const;

    ///
    /// Sets the value of the property for a specific instance of the
    /// reflected class.
    ///
    /// \warning Calling this function with an object which is not an instance
    /// of the reflected class results in undefined behavior.
    ///
    /// \param instance The instance of the reflected class to set the
    /// property value for.
    /// \param value The new value for the property.
    template <typename PropertyType, typename ClassType>
    void set(ClassType& instance, const PropertyType& value) const;

private:
    std::string _name;
};

///
/// A property exposed by a reflector for a specific type.
template <typename PropertyType>
class TypedProperty :
    public Property
{
public:

    ///
    /// A getter function for a typed property.
    typedef std::function<const PropertyType&(const void*)> Getter;

    ///
    /// A setter function for a typed property.
    typedef std::function<void(void*, const PropertyType&)> Setter;

    ///
    /// Constructs a typed property given the name, getter, and setter.
    ///
    /// \param name The name of the property.
    /// \param getter The getter function.
    /// \param setter The setter function.
    TypedProperty(const std::string& name, Getter getter, Setter setter);

    ///
    /// Returns the value of the property for a specific instance of the
    /// reflected class.
    ///
    /// \warning Calling this function with an object which is not an instance
    /// of the reflected class results in undefined behavior.
    ///
    /// \param instance The instance of the reflected class to get the
    /// property value for.
    template <typename ClassType>
    const PropertyType& get(const ClassType& instance) const;

    ///
    /// Sets the value of the property for a specific instance of the
    /// reflected class.
    ///
    /// \warning Calling this function with an object which is not an instance
    /// of the reflected class results in undefined behavior.
    ///
    /// \param instance The instance of the reflected class to set the
    /// property value for.
    /// \param value The new value for the property.
    template <typename ClassType>
    void set(ClassType& instance, const PropertyType& value) const;

private:
    Getter _getter;
    Setter _setter;
};

}

#include "Property.inl"