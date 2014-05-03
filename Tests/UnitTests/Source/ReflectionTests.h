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

SUITE(Reflection)
{
    class Person
    {
    public:

        Person(const std::string& name, int age) :
            _name(name),
            _age(age)
        {
        }

        const std::string& name() const
        {
            return _name;
        }

        void setName(const std::string& name)
        {
            _name = name;
        }

        int age() const
        {
            return _age;
        }

        void setAge(int age)
        {
            _age = age;
        }

    private:
        std::string _name;
        int _age;
    };

    TEST(GetAndSetProperty)
    {
        Person frank("Frank", 37);

        Type& type = Type::create<Person>(Kind::Class, "Person");
        type.addProperty<Person, std::string>("name", &Person::name, &Person::setName);
        type.addProperty<Person, int>("age", &Person::age, &Person::setAge);

        const Property& nameProperty = type.propertyWithName("name");
        CHECK(nameProperty.isType<std::string>());
        CHECK(!nameProperty.isType<int>());

        const Property& ageProperty = type.propertyWithName("age");
        CHECK(!ageProperty.isType<std::string>());
        CHECK(ageProperty.isType<int>());

        // Get and set the name
        CHECK_EQUAL("Frank", nameProperty.get<std::string>(frank));
        nameProperty.set<std::string>(frank, "Joe");
        CHECK_EQUAL("Joe", nameProperty.get<std::string>(frank));

        // Get and set the age
        CHECK_EQUAL(37, ageProperty.get<int>(frank));
        ageProperty.set<int>(frank, 42);
        CHECK_EQUAL(42, ageProperty.get<int>(frank));
        
        // Iterate
        size_t i = 0;
        for (Property* property : type.properties())
        {
            if (i == 0)
            {
                CHECK_EQUAL("name", property->name());
                CHECK(property->isType<std::string>());
                CHECK_EQUAL("Joe", property->get<std::string>(frank));
            }
            else if (i == 1)
            {
                CHECK_EQUAL("age", property->name());
                CHECK(property->isType<int>());
                CHECK_EQUAL(37, property->get<int>(frank));
            }
            ++i;
        }
    }
}