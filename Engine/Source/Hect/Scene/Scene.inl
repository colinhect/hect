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
namespace hect
{

template <typename ...ComponentTypes>
Entity& Scene::create_entity_with(Name name)
{
    Entity& entity = create_entity(name);
    int _[] = { 0, (entity.add_component<ComponentTypes>(), 0)... };
    (void)_;
    return entity;
}


template <typename ComponentType>
ComponentPool<ComponentType>& Scene::components()
{
    const ComponentTypeId type_id = ComponentRegistry::type_id_of<ComponentType>();
    if (type_id >= _component_pools.size() || !_component_pools[type_id])
    {
        const Name component_type_name = Type::get<ComponentType>().name();
        throw InvalidOperation(format("Scene does not support component type '%s'", component_type_name.data()));
    }
    return static_cast<ComponentPool<ComponentType>&>(*_component_pools[type_id]);
}

template <typename ComponentType>
const ComponentPool<ComponentType>& Scene::components() const
{
    return const_cast<Scene*>(this)->components<ComponentType>();
}

}
