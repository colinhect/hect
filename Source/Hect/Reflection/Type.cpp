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
#include "Type.h"

#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"

using namespace hect;

Type::Type(const std::string& name) :
    _name(name)
{
}

const std::string& Type::name() const
{
    return _name;
}

void Type::addRegisterFunction(RegisterFunction registerFunction)
{
    _registerFunctions.push_back(registerFunction);
}

void Type::registerTypes()
{
    for (const RegisterFunction& registerFunction : _registerFunctions)
    {
        registerFunction();
    }
    _registerFunctions.clear();
}

std::vector<Type::RegisterFunction> Type::_registerFunctions;
std::map<std::type_index, std::shared_ptr<Type>> Type::_types;

std::map<std::type_index, std::map<std::string, int>> Enum::_stringToValue;
std::map<std::type_index, std::map<int, std::string>> Enum::_valueToString;