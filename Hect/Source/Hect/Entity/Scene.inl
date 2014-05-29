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
namespace hect
{

template <typename T>
T& Scene::addEntityComponent(EntityId entityId, const T& component)
{
    return componentPool<T>().add(entityId, component);
}

template <typename T>
bool Scene::removeEntityComponent(EntityId entityId)
{
    return componentPool<T>().remove(entityId);
}

template <typename T>
bool Scene::entityHasComponent(EntityId entityId) const
{
    return componentPool<T>().has(entityId);
}

template <typename T>
T& Scene::entityComponent(EntityId entityId)
{
    return componentPool<T>().get(entityId);
}

template <typename T>
void Scene::registerComponent(const std::string& componentName)
{
    std::type_index typeIndex(typeid(T));

    // Create a component pool for this type of component
    _componentPools[typeIndex] = std::shared_ptr<ComponentPoolBase>(new ComponentPool<T>());

    // Create a component adder for this type of component
    _componentAdders[componentName] = [](Scene& scene, EntityId entityId)
    {
        return &scene.addEntityComponent(entityId, T());
    };
}

template <typename T>
ComponentPool<T>& Scene::componentPool()
{
    std::type_index typeIndex(typeid(T));
    auto it = _componentPools.find(typeIndex);
    if (it != _componentPools.end())
    {
        return *((ComponentPool<T>*)it->second.get());
    }
    else
    {
        throw Error("Unknown component type");
    }
}

template <typename T>
const ComponentPool<T>& Scene::componentPool() const
{
    std::type_index typeIndex(typeid(T));
    auto it = _componentPools.find(typeIndex);
    if (it != _componentPools.end())
    {
        return *((ComponentPool<T>*)it->second.get());
    }
    else
    {
        throw Error("Unknown component type");
    }
}

}