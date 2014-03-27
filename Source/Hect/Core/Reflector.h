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

#include <string>
#include <vector>
#include <memory>

#include "Hect/Core/Export.h"
#include "Hect/Core/Property.h"

namespace hect
{

///
/// Reflects the properties of a class.
class Reflector
{
public:

    ///
    /// Constructs a reflector.
    ///
    /// \param className The name of the reflected class.
    HECT_API Reflector(const std::string& className);

    ///
    /// Returns the name of the reflected class.
    HECT_API const std::string& className() const;

    ///
    /// Returns the property of the given name.
    ///
    /// \param name The name of the property to get.
    ///
    /// \throws Error If no property with the given name exists.
    HECT_API const Property& propertyWithName(const std::string& name) const;

    ///
    /// Returns all of the exposed properties in the reflected class.
    HECT_API const Property::Array& properties() const;

    ///
    /// Adds a property to the reflector.
    ///
    /// \param name The name of the property.
    /// \param getter A pointer to the getter member function.
    /// \param setter A pointer to the setter member function.
    template <typename ClassType, typename PropertyType>
    void addProperty(const std::string& name, PropertyType (ClassType::*getter)() const, void (ClassType::*setter)(PropertyType));
    
    ///
    /// Adds a property to the reflector.
    ///
    /// \param name The name of the property.
    /// \param getter A pointer to the getter member function.
    /// \param setter A pointer to the setter member function.
    template <typename ClassType, typename PropertyType>
    void addProperty(const std::string& name, const PropertyType& (ClassType::*getter)() const, void (ClassType::*setter)(const PropertyType&));

private:
    std::string _className;
    Property::Array _properties;
    std::vector<std::shared_ptr<Property>> _ownedProperties;
};

}

#include "Reflector.inl"