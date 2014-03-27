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

SUITE(MetaClass)
{
    class Person
    {
    public:

        static MetaClass<Person>& metaClass()
        {
            static std::unique_ptr<MetaClass<Person>> _metaClass;
            if (!_metaClass)
            {
                _metaClass.reset(new MetaClass<Person>("Person"));
                _metaClass->addProperty<std::string>("name", &Person::name, &Person::setName);
                _metaClass->addProperty<int>("age", &Person::age, &Person::setAge);
            }
            return *_metaClass;
        }

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
        MetaClass<Person>& metaClass = Person::metaClass();

        MetaProperty<Person>* nameProperty = metaClass.findProperty("name");
        CHECK(nameProperty);
        CHECK(nameProperty->isType<std::string>());
        CHECK(!nameProperty->isType<int>());

        MetaProperty<Person>* ageProperty = metaClass.findProperty("age");
        CHECK(ageProperty);
        CHECK(!ageProperty->isType<std::string>());
        CHECK(ageProperty->isType<int>());

        Person frank("Frank", 37);

        // Get and set the name
        CHECK_EQUAL("Frank", nameProperty->get<std::string>(frank));
        nameProperty->set<std::string>(frank, "Joe");
        CHECK_EQUAL("Joe", nameProperty->get<std::string>(frank));

        // Get and set the age
        CHECK_EQUAL(37, ageProperty->get<int>(frank));
        ageProperty->set<int>(frank, 42);
        CHECK_EQUAL(42, ageProperty->get<int>(frank));
    }
}