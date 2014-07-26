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

#include <map>
#include <typeindex>

#include "Hect/Reflection/EnumMetaData.h"

namespace hect
{

enum Kind
{
    Kind_None,
    Kind_Namespace,
    Kind_Class,
    Kind_Enum
};

class Type
{
public:

    template <typename T>
    static void registerTypes();

    template <typename T>
    static Type& create(Kind kind, const std::string& name);
    
    template <typename T>
    static const Type& get();

    template <typename T>
    static const Type& of(T& value);

    Type();
    
    Kind kind() const;
    const std::string& name() const;
    
    EnumMetaData& enumMetaData();
    const EnumMetaData& enumMetaData() const;

private:
    Type(Kind kind, const std::string& name);

    Kind _kind;
    std::string _name;
    EnumMetaData _enumMetaData;

    static std::map<std::type_index, Type> _registeredTypes;
};

}

#include "Type.inl"