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
#include <map>
#include <memory>
#include <vector>
#include <typeinfo>
#include <typeindex>

#include "Hect/Core/Export.h"

namespace hect
{

///
/// A kind of type
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

    Kind::Enum getKind() const;
    const std::string& getName() const;

    template <typename T>
    static Type& create(Kind::Enum kind, const std::string& name)
    {
        std::shared_ptr<Type> type(new Type(kind, name));
        _types[std::type_index(typeid(T))] = type;
        return *type;
    }

    template <typename T>
    static const Type& of(const T& object)
    {
        return *_types[std::type_index(typeid(object))];
    }

    static void registerTypes();

private:
    Type(Kind::Enum kind, const std::string& name);

    Kind::Enum _kind;
    std::string _name;

    static std::map<std::type_index, std::shared_ptr<Type>> _types;
};

}