///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2016 Colin Hill
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

#include "Hect/Core/Exception.h"
#include "Hect/Core/Format.h"

using namespace hect;

std::shared_ptr<SystemBase> SystemRegistry::create(SystemTypeId typeId, Engine& engine, Scene& scene)
{
    if (!isRegisteredTypeId(typeId))
    {
        throw InvalidOperation("Unknown system type id");
    }
    return _constructors[typeId](engine, scene);
}

SystemTypeId SystemRegistry::typeIdOf(std::type_index typeIndex)
{
    auto it = _typeIndexToId.find(typeIndex);
    if (it == _typeIndexToId.end())
    {
        throw InvalidOperation("Unknown system type index");
    }
    return it->second;
}

SystemTypeId SystemRegistry::typeIdOf(const std::string& typeName)
{
    auto it = _typeNameToId.find(typeName);
    if (it == _typeNameToId.end())
    {
        throw InvalidOperation(format("Unknown system type name '%s'", typeName.c_str()));
    }
    return it->second;
}

const std::string& SystemRegistry::typeNameOf(SystemTypeId typeId)
{
    auto it = _typeIdToName.find(typeId);
    if (it == _typeIdToName.end())
    {
        throw InvalidOperation("Unknown system type id");
    }
    return _typeIdToName[typeId];
}

bool SystemRegistry::isRegisteredTypeId(SystemTypeId typeId)
{
    return typeId < _constructors.size();
}

SystemRegistry::SystemTypeIdSequence SystemRegistry::typeIds()
{
    return SystemTypeIdSequence(_typeIds);
}

std::map<SystemTypeId, std::string> SystemRegistry::_typeIdToName;
std::map<std::string, SystemTypeId> SystemRegistry::_typeNameToId;
std::map<std::type_index, SystemTypeId> SystemRegistry::_typeIndexToId;
std::vector<SystemRegistry::SystemConstructor> SystemRegistry::_constructors;
SystemRegistry::SystemTypeIdContainer SystemRegistry::_typeIds;
