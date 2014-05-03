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
#include <map>
#include <memory>
#include <vector>
#include <typeinfo>
#include <typeindex>

#include "Hect/Core/Export.h"
#include "Hect/Reflection/Property.h"

namespace hect
{

///
/// A kind of type.
namespace Kind
{
    enum Enum
    {
        Primitive,
        Structure,
        Class,
        Enumeration
    };
}

class Type
{
public:

    typedef std::function<void()> RegisterFunction;

    HECT_API Type(Kind::Enum kind, const std::string& name);

    HECT_API Kind::Enum kind() const;
    HECT_API const std::string& name() const;

    template <typename T>
    static Type& create(Kind::Enum kind, const std::string& name);
    
    template <typename T>
    static const Type& get();

    template <typename T>
    static const Type& of(const T& object);

    ///
    /// Returns the property of the given name.
    ///
    /// \param name The name of the property to get.
    ///
    /// \throws Error If no property with the given name exists.
    HECT_API const Property& propertyWithName(const std::string& name) const;

    ///
    /// Returns all of the exposed properties in the type.
    HECT_API const Property::Array& properties() const;

    ///
    /// Adds a property to the type.
    ///
    /// \param name The name of the property.
    /// \param getter A pointer to the getter member function.
    /// \param setter A pointer to the setter member function.
    template <typename ClassType, typename PropertyType>
    void addProperty(const std::string& name, PropertyType (ClassType::*getter)() const, void (ClassType::*setter)(PropertyType));
    
    ///
    /// Adds a property to the type.
    ///
    /// \param name The name of the property.
    /// \param getter A pointer to the getter member function.
    /// \param setter A pointer to the setter member function.
    template <typename ClassType, typename PropertyType>
    void addProperty(const std::string& name, const PropertyType& (ClassType::*getter)() const, void (ClassType::*setter)(const PropertyType&));

    static HECT_API void addRegisterFunction(RegisterFunction registerFunction);
    static HECT_API void registerTypes();

private:
    Kind::Enum _kind;
    std::string _name;

    Property::Array _properties;
    std::vector<std::shared_ptr<Property>> _ownedProperties;

    static HECT_API std::vector<RegisterFunction> _registerFunctions;
    static HECT_API std::map<std::type_index, std::shared_ptr<Type>> _types;
};

class Enum
{
public:
    template <typename T>
    static T fromString(const std::string& string);

    template <typename T>
    static const std::string& toString(T value);

    template <typename T>
    static void add(const std::string& string, T value);

private:
    static HECT_API std::map<std::type_index, std::map<std::string, int>> _stringToValue;
    static HECT_API std::map<std::type_index, std::map<int, std::string>> _valueToString;
};

}

#include "Type.inl"