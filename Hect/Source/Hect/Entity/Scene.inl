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
    return _componentPool<T>().add(entityId, component);
}

template <typename T>
bool Scene::removeComponent(EntityId entityId)
{
    return _componentPool<T>().remove(entityId);
}

template <typename T>
bool Scene::entityHasComponent(EntityId entityId) const
{
    return _componentPool<T>().has(entityId);
}

template <typename T>
T& Scene::entityComponent(EntityId entityId)
{
    return _componentPool<T>().get(entityId);
}

template <typename T>
ComponentPool<T>& Scene::_componentPool() const
{
    std::type_index typeIndex(typeid(T));
    auto it = _componentPools.find(typeIndex);
    if (it != _componentPools.end())
    {
        return *((ComponentPool<T>*)it->second.get());
    }
    else
    {
        std::shared_ptr<ComponentPoolBase> componentPool(new ComponentPool<T>());
        _componentPools[typeIndex] = componentPool;
        return *((ComponentPool<T>*)componentPool.get());
    }
}

}