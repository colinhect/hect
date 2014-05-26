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
ComponentPool<T>::ComponentPool() :
    _components(32, T()),
    _entityIdToComponentId(1024, (ComponentId)-1)
{
}

template <typename T>
T& ComponentPool<T>::add(EntityId entityId, const T& component)
{
    while (entityId >= _entityIdToComponentId.size())
    {
        _entityIdToComponentId.resize(_entityIdToComponentId.size() * 2);
    }

    ComponentId& componentId = _entityIdToComponentId[entityId];
    if (componentId == (ComponentId)-1)
    {
        componentId = _componentIdPool.create();
    }

    return _components[componentId] = component;
}

template <typename T>
bool ComponentPool<T>::remove(EntityId entityId)
{
    bool hadComponent = false;
    if (entityId < _entityIdToComponentId.size())
    {
        ComponentId& componentId = _entityIdToComponentId[entityId];
        if (componentId != (ComponentId)-1)
        {
            _componentIdPool.destroy(componentId);

            hadComponent = true;
            componentId = (ComponentId)-1;
        }
    }

    return hadComponent;
}

template <typename T>
void ComponentPool<T>::clone(EntityId sourceEntityId, EntityId destEntityId)
{
    if (sourceEntityId < _entityIdToComponentId.size())
    {
        ComponentId& componentId = _entityIdToComponentId[sourceEntityId];
        if (componentId != (ComponentId)-1)
        {
            add(destEntityId, _components[componentId]);
        }
    }
}

template <typename T>
bool ComponentPool<T>::has(EntityId entityId) const
{
    bool entityHasComponent = false;
    if (entityId < _entityIdToComponentId.size())
    {
        const ComponentId& componentId = _entityIdToComponentId[entityId];
        entityHasComponent = componentId != (ComponentId)-1;
    }

    return entityHasComponent;
}

template <typename T>
T& ComponentPool<T>::get(EntityId entityId)
{
    if (entityId < _entityIdToComponentId.size())
    {
        ComponentId& componentId = _entityIdToComponentId[entityId];
        if (componentId != (ComponentId)-1)
        {
            return _components[componentId];
        }
    }

    throw Error("Entity does not have a component of the specified type");
}

}