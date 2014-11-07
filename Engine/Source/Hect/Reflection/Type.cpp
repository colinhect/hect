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

#include "Hect/Core/Configuration.h"
#include "Hect/Reflection/Enum.h"

using namespace hect;

Type::Type() :
    _kind(Kind_None)
{
}

Kind Type::kind() const
{
    return _kind;
}

const std::string& Type::name() const
{
    return _name;
}

Enum& Type::asEnum()
{
    if (!_enum)
    {
        throw Error("Type is not an enum");
    }
    return *_enum;
}

const Enum& Type::asEnum() const
{
    if (!_enum)
    {
        throw Error("Type is not an enum");
    }
    return *_enum;
}

Type::Type(Kind kind, const std::string& name) :
    _kind(kind),
    _name(name),
    _enum(new Enum(name))
{
}

const Type& Type::fromTypeInfo(const std::type_info& typeInfo)
{
    std::type_index typeIndex(typeInfo);

    auto it = _registeredTypes.find(typeIndex);
    if (it != _registeredTypes.end())
    {
        return it->second;
    }

    // If no type was found then create a placeholder which has just the name
    // of the type
    else
    {
        std::string typeName = typeInfo.name();

#ifdef HECT_WINDOWS_BUILD
        // Strip all characters before the unscoped type name
        size_t i = typeName.size() - 1;
        while (i > 0 && (typeName[i] != ':' && typeName[i] != ' '))
        {
            --i;
        }

        typeName = std::string(&typeName[i + 1]);
#endif

        // Create the placeholder type
        return _registeredTypes[typeIndex] = Type(Kind_None, typeName);
    }
}

std::map<std::type_index, Type> Type::_registeredTypes;
