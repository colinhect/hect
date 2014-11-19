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
#include "SystemRegistry.h"

#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"

using namespace hect;

std::shared_ptr<System> SystemRegistry::create(SystemTypeId typeId, Scene& scene)
{
    return _constructors[typeId](scene);
}

SystemTypeId SystemRegistry::typeIdOf(std::type_index typeIndex)
{
    auto it = _typeIndexToId.find(typeIndex);
    if (it == _typeIndexToId.end())
    {
        throw Error("Unknown system type");
    }
    return it->second;
}

SystemTypeId SystemRegistry::typeIdOf(const std::string& typeName)
{
    auto it = _typeNameToId.find(typeName);
    if (it == _typeNameToId.end())
    {
        throw Error(format("Unknown system type '%s'", typeName.c_str()));
    }

    return it->second;
}

std::map<std::string, SystemTypeId> SystemRegistry::_typeNameToId;
std::map<std::type_index, SystemTypeId> SystemRegistry::_typeIndexToId;
std::vector<SystemRegistry::SystemConstructor> SystemRegistry::_constructors;
