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

namespace hect
{

class Type
{
public:

    typedef std::function<void()> RegisterFunction;

    HECT_API Type(const std::string& name);

    HECT_API const std::string& name() const;

    template <typename T>
    static Type& create(const std::string& name);

    template <typename T>
    static const Type& get();

    template <typename T>
    static const Type& of(const T& object);

    static HECT_API void addRegisterFunction(RegisterFunction registerFunction);
    static HECT_API void registerTypes();

private:
    std::string _name;

    static HECT_API std::vector<RegisterFunction> _registerFunctions;
    static HECT_API std::map<std::type_index, std::shared_ptr<Type>> _types;
};

}

#include "Type.inl"