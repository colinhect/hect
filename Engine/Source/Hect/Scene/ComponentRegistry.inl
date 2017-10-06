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
#include "Hect/Reflection/Type.h"

#include "Hect/Core/Format.h"
#include "Hect/Core/Logging.h"

namespace hect
{

template <typename ComponentType>
void ComponentRegistry::register_type()
{
    std::type_index type_index(typeid(ComponentType));

    if (_type_index_to_id.find(type_index) == _type_index_to_id.end())
    {
        Name type_name = Type::get<ComponentType>().name();

        ComponentTypeId type_id = static_cast<ComponentTypeId>(_component_constructors.size());

        _component_constructors.push_back([]()
        {
            return std::shared_ptr<ComponentBase>(new ComponentType());
        });

        _component_pool_constructors.push_back([](Scene& scene)
        {
            return std::shared_ptr<ComponentPoolBase>(new ComponentPool<ComponentType>(scene));
        });

        _type_index_to_id[type_index] = type_id;
        _type_name_to_id[type_name] = type_id;
        _type_id_to_name[type_id] = type_name;
        _type_ids.push_back(type_id);

        HECT_DEBUG(format("Registered component type '%s' (type id: %d)", type_name.data(), type_id));
    }
}

template <typename ComponentType>
ComponentTypeId ComponentRegistry::type_id_of()
{
    static ComponentTypeId id = ComponentTypeId(-1);
    if (id == ComponentTypeId(-1))
    {
        std::type_index type_index(typeid(ComponentType));
        id = type_id_of(type_index);
    }
    return id;
}

}
