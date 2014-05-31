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
bool ComponentPool<T>::Iterator::isValid() const
{
    return _components
        && _index < _components->size()
        && (*_components)[_index].entity().exists();
}

template <typename T>
T& ComponentPool<T>::Iterator::operator*() const
{
    if (!_components || _index >= _components->size())
    {
        throw Error("Cannot dereference an invalid component iterator");
    }
    return (*_components)[_index];
}

template <typename T>
T* ComponentPool<T>::Iterator::operator->() const
{
    return &(**this);
}

template <typename T>
typename ComponentPool<T>::Iterator& ComponentPool<T>::Iterator::operator++()
{
    // Move to the next component
    size_t size = _components->size();
    do
    {
        ++_index;
    } while (_index < size && !(*_components)[_index].entity().exists());

    return *this;
}

template <typename T>
bool ComponentPool<T>::Iterator::operator==(const Iterator& other) const
{
    return _components == other._components && _index == other._index;
}

template <typename T>
bool ComponentPool<T>::Iterator::operator!=(const Iterator& other) const
{
    return _components != other._components || _index != other._index;
}

template <typename T>
ComponentPool<T>::Iterator::operator bool() const
{
    return isValid();
}

template <typename T>
ComponentPool<T>::Iterator::Iterator(std::vector<T>* components, size_t index) :
    _components(components),
    _index(index)
{
    if (_components)
    {
        // Move to the first component
        size_t size = _components->size();
        while (_index < size && !(*_components)[_index].entity().exists())
        {
            ++_index;
        }
    }
}

template <typename T>
bool ComponentPool<T>::ConstIterator::isValid() const
{
    return _components
        && _index < _components->size()
        && (*_components)[_index].entity().exists();
}

template <typename T>
const T& ComponentPool<T>::ConstIterator::operator*() const
{
    if (!_components || _index >= _components->size())
    {
        throw Error("Cannot dereference an invalid component iterator");
    }
    return (*_components)[_index];
}

template <typename T>
const T* ComponentPool<T>::ConstIterator::operator->() const
{
    return &(**this);
}

template <typename T>
typename ComponentPool<T>::ConstIterator& ComponentPool<T>::ConstIterator::operator++()
{
    // Move to the next component
    size_t size = _components->size();
    do
    {
        ++_index;
    } while (_index < size && !(*_components)[_index].entity().exists());

    return *this;
}

template <typename T>
bool ComponentPool<T>::ConstIterator::operator==(const ConstIterator& other) const
{
    return _components == other._components && _index == other._index;
}

template <typename T>
bool ComponentPool<T>::ConstIterator::operator!=(const ConstIterator& other) const
{
    return _components != other._components || _index != other._index;
}

template <typename T>
ComponentPool<T>::ConstIterator::operator bool() const
{
    return isValid();
}

template <typename T>
ComponentPool<T>::ConstIterator::ConstIterator(const std::vector<T>* components, size_t index) :
    _components(components),
    _index(index)
{
    if (_components)
    {
        // Move to the first component
        size_t size = _components->size();
        while (_index < size && !(*_components)[_index].entity().exists())
        {
            ++_index;
        }
    }
}

template <typename T>
ComponentPool<T>::ComponentPool(Scene* scene) :
    _scene(scene)
{
}

template <typename T>
Dispatcher<ComponentPoolEvent>& ComponentPool<T>::dispatcher()
{
    return _dispatcher;
}

template <typename T>
void ComponentPool<T>::add(EntityId entityId, const ComponentBase& component)
{
    add(entityId, *(T*)&component);
}

template <typename T>
typename ComponentPool<T>::Iterator ComponentPool<T>::add(EntityId entityId, const T& component)
{
    // Expand the entity to component vector if needed
    while (entityId >= _entityIdToComponentId.size())
    {
        size_t size = _entityIdToComponentId.size();
        _entityIdToComponentId.resize(std::max(size * 2, (size_t)8), (ComponentId)-1);
    }

    // Get the component id for the entity
    ComponentId& componentId = _entityIdToComponentId[entityId];
    if (componentId == (ComponentId)-1)
    {
        // The entity did not have a component in the pool so create a new one
        componentId = _componentIdPool.create();
    }
    else
    {
        // The entity already had a component so dispatch a remove event first
        _dispatcher.notifyEvent(ComponentPoolEvent(ComponentPoolEventType::Remove, entityId));
    }

    // Expand the components vector if needed
    while (componentId >= _components.size())
    {
        size_t size = _components.size();
        _components.resize(std::max(size * 2, (size_t)8));
    }

    // Assign the new component and get a reference to it
    T& addedComponent = _components[componentId] = component;
    addedComponent._entity = Entity(*_scene, entityId);

    // Dispatch the add event
    _dispatcher.notifyEvent(ComponentPoolEvent(ComponentPoolEventType::Add, entityId));

    return Iterator(&_components, componentId);
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
            // Dispatch the add event
            _dispatcher.notifyEvent(ComponentPoolEvent(ComponentPoolEventType::Remove, entityId));
            
            _componentIdPool.destroy(componentId);

            hadComponent = true;
            _components[componentId]._entity = Entity();
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
typename ComponentPool<T>::Iterator ComponentPool<T>::get(EntityId entityId)
{
    if (entityId < _entityIdToComponentId.size())
    {
        ComponentId& componentId = _entityIdToComponentId[entityId];
        if (componentId != (ComponentId)-1)
        {
            return Iterator(&_components, componentId);
        }
    }

    return Iterator(nullptr, 0);
}

template <typename T>
typename ComponentPool<T>::Iterator ComponentPool<T>::begin()
{
    return Iterator(&_components, 0);
}

template <typename T>
typename ComponentPool<T>::ConstIterator ComponentPool<T>::begin() const
{
    return ConstIterator(&_components, 0);
}

template <typename T>
typename ComponentPool<T>::Iterator ComponentPool<T>::end()
{
    return Iterator(&_components, _components.size());
}

template <typename T>
typename ComponentPool<T>::ConstIterator ComponentPool<T>::end() const
{
    return ConstIterator(&_components, _components.size());
}

}