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

std::shared_ptr<ComponentBase> ComponentRegistry::create(ComponentTypeId type_id)
{
    if (!is_registered_type_id(type_id))
    {
        throw InvalidOperation("Unknown component type id");
    }
    return _component_constructors[type_id]();
}

std::shared_ptr<ComponentPoolBase> ComponentRegistry::create_pool(ComponentTypeId type_id, Scene& scene)
{
    if (!is_registered_type_id(type_id))
    {
        throw InvalidOperation("Unknown component type id");
    }
    return _component_pool_constructors[type_id](scene);
}

ComponentTypeId ComponentRegistry::type_id_of(std::type_index type_index)
{
    auto it = _type_index_to_id.find(type_index);
    if (it == _type_index_to_id.end())
    {
        throw InvalidOperation("Unknown component type");
    }
    return it->second;
}

ComponentTypeId ComponentRegistry::type_id_of(Name type_name)
{
    auto it = _type_name_to_id.find(type_name);
    if (it == _type_name_to_id.end())
    {
        throw InvalidOperation(format("Unknown component type '%s'", type_name.data()));
    }
    return it->second;
}

Name ComponentRegistry::type_name_of(ComponentTypeId type_id)
{
    auto it = _type_id_to_name.find(type_id);
    if (it == _type_id_to_name.end())
    {
        throw InvalidOperation("Unknown component type id");
    }
    return _type_id_to_name[type_id];
}

bool ComponentRegistry::is_registered_type_id(ComponentTypeId type_id)
{
    return type_id < _component_constructors.size();
}

ComponentRegistry::ComponentTypeIdSequence ComponentRegistry::type_ids()
{
    return ComponentTypeIdSequence(_type_ids);
}

std::map<ComponentTypeId, Name> ComponentRegistry::_type_id_to_name;
std::map<Name, ComponentTypeId> ComponentRegistry::_type_name_to_id;
std::map<std::type_index, ComponentTypeId> ComponentRegistry::_type_index_to_id;
std::vector<ComponentRegistry::ComponentConstructor> ComponentRegistry::_component_constructors;
std::vector<ComponentRegistry::ComponentPoolConstructor> ComponentRegistry::_component_pool_constructors;
ComponentRegistry::ComponentTypeIdContainer ComponentRegistry::_type_ids;
