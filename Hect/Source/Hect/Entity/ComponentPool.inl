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

template<typename T>
T& ComponentIterator<T>::operator*() const
{
    return _componentPool->_components[_index];
}

template<typename T>
typename ComponentIterator<T>& ComponentIterator<T>::operator++()
{
    auto& components = _componentPool->_components;
    do
    {
        ++_index;
    } while (_index < components.size() && components[_index].entityId() == (EntityId)-1);

    return *this;
}

template<typename T>
bool ComponentIterator<T>::operator==(const ComponentIterator<T>& other) const
{
    return _componentPool == other._componentPool && _index == other._index;
}

template<typename T>
bool ComponentIterator<T>::operator!=(const ComponentIterator<T>& other) const
{
    return _componentPool != other._componentPool || _index != other._index;
}

template<typename T>
ComponentIterator<T>::ComponentIterator(ComponentPool<T>* componentPool, size_t index) :
    _componentPool(componentPool),
    _index(index)
{
}

template<typename T>
const T& ConstComponentIterator<T>::operator*() const
{
    return _componentPool->_components[_index];
}

template<typename T>
typename ConstComponentIterator<T>& ConstComponentIterator<T>::operator++()
{
    auto& components = _componentPool->_components;
    do
    {
        ++_index;
    } while (_index < components.size() && components[_index].entityId() == (EntityId)-1);

    return *this;
}

template<typename T>
bool ConstComponentIterator<T>::operator==(const ConstComponentIterator<T>& other) const
{
    return _componentPool == other._componentPool && _index == other._index;
}

template<typename T>
bool ConstComponentIterator<T>::operator!=(const ConstComponentIterator<T>& other) const
{
    return _componentPool != other._componentPool || _index != other._index;
}

template<typename T>
ConstComponentIterator<T>::ConstComponentIterator(const ComponentPool<T>* componentPool, size_t index) :
    _componentPool(componentPool),
    _index(index)
{
}

template <typename T>
typename ComponentIterator<T> ComponentPool<T>::begin()
{
    return ComponentIterator<T>(this, 0);
}

template <typename T>
typename ConstComponentIterator<T> ComponentPool<T>::begin() const
{
    return ConstComponentIterator<T>(this, 0);
}

template <typename T>
typename ComponentIterator<T> ComponentPool<T>::end()
{
    return ComponentIterator<T>(this, _components.size());
}

template <typename T>
typename ConstComponentIterator<T> ComponentPool<T>::end() const
{
    return ConstComponentIterator<T>(this, _components.size());
}

template <typename T>
void ComponentPool<T>::_add(EntityId entityId, const ComponentBase& component)
{
    _add(entityId, *(T*)&component);
}

template <typename T>
T& ComponentPool<T>::_add(EntityId entityId, const T& component)
{
    while (entityId >= _entityIdToComponentId.size())
    {
        _entityIdToComponentId.resize(std::max(_entityIdToComponentId.size() * 2, (size_t)8), (ComponentId)-1);
    }

    ComponentId& componentId = _entityIdToComponentId[entityId];
    if (componentId == (ComponentId)-1)
    {
        componentId = _componentIdPool.create();
    }

    while (componentId >= _components.size())
    {
        _components.resize(std::max(_components.size() * 2, (size_t)8), T());
    }

    T& addedComponent = _components[componentId] = component;
    addedComponent._entityId = entityId;
    return addedComponent;
}

template <typename T>
bool ComponentPool<T>::_remove(EntityId entityId)
{
    bool hadComponent = false;
    if (entityId < _entityIdToComponentId.size())
    {
        ComponentId& componentId = _entityIdToComponentId[entityId];
        if (componentId != (ComponentId)-1)
        {
            _componentIdPool.destroy(componentId);

            hadComponent = true;
            _components[componentId]._entityId = (EntityId)-1;
            componentId = (ComponentId)-1;
        }
    }

    return hadComponent;
}

template <typename T>
void ComponentPool<T>::_clone(EntityId sourceEntityId, EntityId destEntityId)
{
    if (sourceEntityId < _entityIdToComponentId.size())
    {
        ComponentId& componentId = _entityIdToComponentId[sourceEntityId];
        if (componentId != (ComponentId)-1)
        {
            _add(destEntityId, _components[componentId]);
        }
    }
}

template <typename T>
bool ComponentPool<T>::_has(EntityId entityId) const
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
T& ComponentPool<T>::_get(EntityId entityId)
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