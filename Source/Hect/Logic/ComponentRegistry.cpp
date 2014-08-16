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
#include "ComponentRegistry.h"

#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"

using namespace hect;

ComponentBase::Pointer ComponentRegistry::createComponent(std::type_index typeIndex)
{
    auto it = _typeIndexToId.find(typeIndex);
    if (it == _typeIndexToId.end())
    {
        throw Error("Unregistered component type");
    }

    return _componentConstructors[it->second]();
}

ComponentBase::Pointer ComponentRegistry::createComponent(const std::string& typeName)
{
    auto it = _typeNameToId.find(typeName);
    if (it == _typeNameToId.end())
    {
        throw Error(format("Unregistered component type '%s'", typeName.c_str()));
    }

    return _componentConstructors[it->second]();
}

ComponentPoolBase::Pointer ComponentRegistry::createPool(std::type_index typeIndex, World& world)
{
    auto it = _typeIndexToId.find(typeIndex);
    if (it == _typeIndexToId.end())
    {
        throw Error("Unregistered component type");
    }

    return _poolConstructors[it->second](world);
}

ComponentPoolBase::Pointer ComponentRegistry::createPool(const std::string& typeName, World& world)
{
    auto it = _typeNameToId.find(typeName);
    if (it == _typeNameToId.end())
    {
        throw Error(format("Unregistered component type '%s'", typeName.c_str()));
    }

    return _poolConstructors[it->second](world);
}

ComponentTypeId ComponentRegistry::_nextTypeId = 0;
std::map<std::string, ComponentTypeId> ComponentRegistry::_typeNameToId;
std::map<std::type_index, ComponentTypeId> ComponentRegistry::_typeIndexToId;
std::map<ComponentTypeId, std::function<ComponentBase::Pointer(void)>> ComponentRegistry::_componentConstructors;
std::map<ComponentTypeId, std::function<ComponentPoolBase::Pointer(World&)>> ComponentRegistry::_poolConstructors;