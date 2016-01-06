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
#include "ComponentRegistry.h"

#include "Hect/Core/Exception.h"
#include "Hect/Core/Format.h"

using namespace hect;

std::shared_ptr<ComponentBase> ComponentRegistry::create(ComponentTypeId typeId)
{
    if (!isRegisteredTypeId(typeId))
    {
        throw InvalidOperation("Unknown component type id");
    }
    return _componentConstructors[typeId]();
}

std::shared_ptr<ComponentPoolBase> ComponentRegistry::createPool(ComponentTypeId typeId, Scene& scene)
{
    if (!isRegisteredTypeId(typeId))
    {
        throw InvalidOperation("Unknown component type id");
    }
    return _componentPoolConstructors[typeId](scene);
}

ComponentTypeId ComponentRegistry::typeIdOf(std::type_index typeIndex)
{
    auto it = _typeIndexToId.find(typeIndex);
    if (it == _typeIndexToId.end())
    {
        throw InvalidOperation("Unknown component type");
    }
    return it->second;
}

ComponentTypeId ComponentRegistry::typeIdOf(const std::string& typeName)
{
    auto it = _typeNameToId.find(typeName);
    if (it == _typeNameToId.end())
    {
        throw InvalidOperation(format("Unknown component type '%s'", typeName.c_str()));
    }
    return it->second;
}

const std::string& ComponentRegistry::typeNameOf(ComponentTypeId typeId)
{
    auto it = _typeIdToName.find(typeId);
    if (it == _typeIdToName.end())
    {
        throw InvalidOperation("Unknown component type id");
    }
    return _typeIdToName[typeId];
}

bool ComponentRegistry::isRegisteredTypeId(ComponentTypeId typeId)
{
    return typeId < _componentConstructors.size();
}

std::map<ComponentTypeId, std::string> ComponentRegistry::_typeIdToName;
std::map<std::string, ComponentTypeId> ComponentRegistry::_typeNameToId;
std::map<std::type_index, ComponentTypeId> ComponentRegistry::_typeIndexToId;
std::vector<ComponentRegistry::ComponentConstructor> ComponentRegistry::_componentConstructors;
std::vector<ComponentRegistry::ComponentPoolConstructor> ComponentRegistry::_componentPoolConstructors;