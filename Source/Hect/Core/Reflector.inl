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

template <typename ClassType, typename PropertyType>
void Reflector::addProperty(const std::string& name, PropertyType (ClassType::*getter)() const, void (ClassType::*setter)(PropertyType))
{
    auto propertyGetter = [=] (const void* instance) -> const PropertyType&
    {
        static PropertyType value = PropertyType();
        value = ((reinterpret_cast<const ClassType*>(instance))->*getter)();
        return value;
    };

    auto propertySetter = [=] (void* instance, const PropertyType& value) -> void
    {
        ((reinterpret_cast<ClassType*>(instance))->*setter)(value);
    };

    std::shared_ptr<Property> property(new TypedProperty<PropertyType>(name, propertyGetter, propertySetter));
    _properties.push_back(property.get());
    _ownedProperties.push_back(property);
}

template <typename ClassType, typename PropertyType>
void Reflector::addProperty(const std::string& name, const PropertyType& (ClassType::*getter)() const, void (ClassType::*setter)(const PropertyType&))
{
    auto propertyGetter = [=] (const void* instance) -> const PropertyType&
    {
        return ((reinterpret_cast<const ClassType*>(instance))->*getter)();
    };

    auto propertySetter = [=] (void* instance, const PropertyType& value) -> void
    {
        ((reinterpret_cast<ClassType*>(instance))->*setter)(value);
    };

    std::shared_ptr<Property> property(new TypedProperty<PropertyType>(name, propertyGetter, propertySetter));
    _properties.push_back(property.get());
    _ownedProperties.push_back(property);
}

}