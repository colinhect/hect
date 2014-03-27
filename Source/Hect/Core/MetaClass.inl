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

    template <typename ClassType>
    MetaClass<ClassType>::MetaClass(const std::string& name) :
        _name(name)
    {
    }

    template <typename ClassType>
    template <typename PropertyType>
    void MetaClass<ClassType>::addProperty(const std::string& name, PropertyType (ClassType::*get)() const, void (ClassType::*set)(PropertyType))
    {
        TypedMetaProperty<ClassType, PropertyType>::Getter getter = [=] (const ClassType& instance) -> const PropertyType&
            {
                static PropertyType value = PropertyType();
                value = (instance.*get)();
                return value;
            };

        TypedMetaProperty<ClassType, PropertyType>::Setter setter = [=] (ClassType& instance, const PropertyType& value) -> void
            {
                (instance.*set)(value);
            };

        std::shared_ptr<MetaProperty<ClassType>> property(new TypedMetaProperty<ClassType, PropertyType>(name, getter, setter));
        _properties.push_back(property.get());
        _ownedProperties.push_back(property);
    }

    template <typename ClassType>
    template <typename PropertyType>
    void MetaClass<ClassType>::addProperty(const std::string& name, const PropertyType& (ClassType::*get)() const, void (ClassType::*set)(const PropertyType&))
    {
        TypedMetaProperty<ClassType, PropertyType>::Getter getter = [=] (const ClassType& instance) -> const PropertyType&
            {
                return (instance.*get)();
            };

        TypedMetaProperty<ClassType, PropertyType>::Setter setter = [=] (ClassType& instance, const PropertyType& value) -> void
            {
                (instance.*set)(value);
            };

        std::shared_ptr<MetaProperty<ClassType>> property(new TypedMetaProperty<ClassType, PropertyType>(name, getter, setter));
        _properties.push_back(property.get());
        _ownedProperties.push_back(property);
    }
    
    template <typename ClassType>
    MetaProperty<ClassType>* MetaClass<ClassType>::findProperty(const std::string& name)
    {
        for (MetaProperty<ClassType>* property : _properties)
        {
            if (property->name() == name)
            {
                return property;
            }
        }

        return nullptr;
    }

}