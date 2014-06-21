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
ComponentEvent<T>::ComponentEvent(ComponentEventType::Enum type, Entity& entity) :
    _type(type),
    _entity(&entity)
{
}

template <typename T>
ComponentEventType::Enum ComponentEvent<T>::type() const
{
    return _type;
}

template <typename T>
Entity& ComponentEvent<T>::entity() const
{
    assert(_entity);
    return *_entity;
}

template <typename T>
ComponentPool<T>::ComponentPool(Scene& scene) :
    _scene(&scene)
{
}

template <typename T>
Dispatcher<ComponentEvent<T>>& ComponentPool<T>::dispatcher()
{
    return _dispatcher;
}

template <typename T>
typename Component<T>::Iter ComponentPool<T>::begin()
{
    Component<T>::Iter iter(*this, 0);

    // Move to the first component with activated entity
    if (!iter || !iter->entity().isActivated())
    {
        ++iter;
    }
    return iter;
}

template <typename T>
typename Component<T>::ConstIter ComponentPool<T>::begin() const
{
    Component<T>::ConstIter iter(*this, 0);

    // Move to the first component with activated entity
    if (!iter || !iter->entity().isActivated())
    {
        ++iter;
    }
    return iter;
}

template <typename T>
typename Component<T>::Iter ComponentPool<T>::end()
{
    return Component<T>::Iter(*this, std::max(_maxId(), (ComponentId)1));
}

template <typename T>
typename Component<T>::ConstIter ComponentPool<T>::end() const
{
    return Component<T>::ConstIter(*this, std::max(_maxId(), (ComponentId)1));
}

template <typename T>
typename Component<T>::Iter ComponentPool<T>::findFirst(typename ComponentPool<T>::Predicate predicate)
{
    for (auto iter = begin(); iter != end(); ++iter)
    {
        if (predicate(*iter))
        {
            return iter;
        }
    }
    return end();
}

template <typename T>
typename Component<T>::ConstIter ComponentPool<T>::findFirst(typename ComponentPool<T>::Predicate predicate) const
{
    for (auto iter = begin(); iter != end(); ++iter)
    {
        if (predicate(*iter))
        {
            return iter;
        }
    }
    return end();
}

template <typename T>
typename Component<T>::Iter::Array ComponentPool<T>::find(typename ComponentPool<T>::Predicate predicate)
{
    Component<T>::Iter::Array results;
    for (auto iter = begin(); iter != end(); ++iter)
    {
        if (predicate(*iter))
        {
            results.push_back(iter);
        }
    }
    return results;
}

template <typename T>
typename Component<T>::ConstIter::Array ComponentPool<T>::find(typename ComponentPool<T>::Predicate predicate) const
{
    Component<T>::Iter::Array results;
    for (auto iter = begin(); iter != end(); ++iter)
    {
        if (predicate(*iter))
        {
            results.push_back(iter);
        }
    }
    return results;
}

template <typename T>
void ComponentPool<T>::_notifyEvent(ComponentEventType::Enum type, Entity& entity)
{
    _dispatcher.notifyEvent(ComponentEvent<T>(type, entity));
}

template <typename T>
void ComponentPool<T>::_addBase(Entity& entity, const ComponentBase& component)
{
    _add(entity, *(T*)&component);
}

template <typename T>
void ComponentPool<T>::_remove(Entity& entity)
{
    EntityId entityId = entity.id();
    if (entityId < _entityToComponent.size())
    {
        // Get the component id for the entity
        ComponentId& componentId = _entityToComponent[entityId];
        if (componentId != (ComponentId)-1)
        {
            // If the entity is activated then dispatch the remove event
            if (entity.isActivated())
            {
                _dispatcher.notifyEvent(ComponentEvent<T>(ComponentEventType::Remove, entity));
            }

            _idPool.destroy(componentId);

            _components[componentId]._exitPool();
            _componentToEntity[componentId] = (EntityId)-1;
            componentId = (ComponentId)-1;

            return;
        }
    }

    throw Error("Entity does not have a component of that type");
}

template <typename T>
void ComponentPool<T>::_clone(const Entity& source, Entity& dest)
{
    EntityId sourceEntityId = source.id();
    if (sourceEntityId < _entityToComponent.size())
    {
        ComponentId& componentId = _entityToComponent[sourceEntityId];
        if (componentId != (ComponentId)-1)
        {
            _add(dest, _components[componentId]);
        }
    }
}

template <typename T>
bool ComponentPool<T>::_has(const Entity& entity) const
{
    EntityId entityId = entity.id();
    if (entityId < _entityToComponent.size())
    {
        const ComponentId& componentId = _entityToComponent[entityId];
        if (componentId != (ComponentId)-1)
        {
            return true;
        }
    }

    return false;
}

template <typename T>
ComponentBase& ComponentPool<T>::_getBase(Entity& entity)
{
    auto component = _get(entity);
    if (!component)
    {
        throw Error("Entity does not have a component in this pool");
    }
    return *component;
}

template <typename T>
const ComponentBase& ComponentPool<T>::_getBase(const Entity& entity) const
{
    auto component = _get(entity);
    if (!component)
    {
        throw Error("Entity does not have a component in this pool");
    }
    return *component;
}


template <typename T>
typename Component<T>::Iter ComponentPool<T>::_add(Entity& entity, const T& component)
{
    EntityId entityId = entity.id();

    // The component being added might be a reference to a component in the
    // component vector which might be resized; so it needs to be copied first
    T copiedComponent = component;

    // Expand the entity-to-component vector if needed
    _expandVector(_entityToComponent, entityId, (ComponentId)-1);

    // Get the component id for the entity
    ComponentId& componentId = _entityToComponent[entityId];

    // Ensure that the entity does not already have a component of this type
    if (componentId != (ComponentId)-1)
    {
        throw Error("Entity already has a component of that type");
    }

    // Create the new component ID
    componentId = _idPool.create();

    // Expand the component vector if needed
    _expandVector(_components, componentId);

    // Expand the component-to-entity vector if needed
    _expandVector(_componentToEntity, componentId, (EntityId)-1);

    // Remember which entity this component belongs to
    _componentToEntity[componentId] = entityId;

    // Assign the new component and get a reference to it
    T& addedComponent = _components[componentId] = copiedComponent;
    addedComponent._enterPool(*this, componentId);

    // Dispatch the add event if the entity is activated
    if (entity.isActivated())
    {
        _dispatcher.notifyEvent(ComponentEvent<T>(ComponentEventType::Add, entity));
    }

    return Component<T>::Iter(*this, componentId);
}

template <typename T>
typename Component<T>::Iter ComponentPool<T>::_replace(Entity& entity, const T& component)
{
    EntityId entityId = entity.id();
    if (entityId < _entityToComponent.size())
    {
        // Get the component id for the entity
        ComponentId& componentId = _entityToComponent[entityId];
        if (componentId != (ComponentId)-1)
        {
            // If the entity is activated then dispatch the remove event
            if (entity.isActivated())
            {
                _dispatcher.notifyEvent(ComponentEvent<T>(ComponentEventType::Remove, entity));
            }

            T& addedComponent = _components[componentId];
            addedComponent._exitPool();
            addedComponent = component;
            addedComponent._enterPool(*this, componentId);

            // If the entity is activated then dispatch the add event
            if (entity.isActivated())
            {
                _dispatcher.notifyEvent(ComponentEvent<T>(ComponentEventType::Add, entity));
            }

            return Component<T>::Iter(*this, componentId);
        }
    }

    throw Error("Entity does not have a component of that type");
}

template <typename T>
typename Component<T>::Iter ComponentPool<T>::_get(Entity& entity)
{
    EntityId entityId = entity.id();
    if (entityId < _entityToComponent.size())
    {
        ComponentId componentId = _entityToComponent[entityId];
        if (componentId != (ComponentId)-1)
        {
            return Component<T>::Iter(*this, componentId);
        }
    }

    return end();
}

template <typename T>
typename Component<T>::ConstIter ComponentPool<T>::_get(const Entity& entity) const
{
    EntityId entityId = entity.id();
    if (entityId < _entityToComponent.size())
    {
        ComponentId componentId = _entityToComponent[entityId];
        if (componentId != (ComponentId)-1)
        {
            return Component<T>::ConstIter(*this, componentId);
        }
    }

    return end();
}

template <typename T>
ComponentId ComponentPool<T>::_maxId() const
{
    return (ComponentId)_components.size();
}

template <typename T>
bool ComponentPool<T>::_componentHasEntity(ComponentId id) const
{
    if (id < _componentToEntity.size())
    {
        if (_componentToEntity[id] != (EntityId)-1)
        {
            return true;
        }
    }
    return false;
}

template <typename T>
Entity& ComponentPool<T>::_entityForComponent(ComponentId id)
{
    if (id < _componentToEntity.size())
    {
        EntityId entityId = _componentToEntity[id];
        if (entityId != (EntityId)-1)
        {
            return _scene->entities().entityWithId(entityId);
        }
    }
    throw Error("Component does not have an associated entity");
}

template <typename T>
const Entity& ComponentPool<T>::_entityForComponent(ComponentId id) const
{
    if (id < _componentToEntity.size())
    {
        EntityId entityId = _componentToEntity[id];
        if (entityId != (EntityId)-1)
        {
            return _scene->entityWithId(entityId);
        }
    }
    throw Error("Component does not have an associated entity");
}

template <typename T>
T& ComponentPool<T>::_componentWithId(ComponentId id)
{
    if (id < _components.size())
    {
        T& component = _components[id];
        if (component._inPool())
        {
            return component;
        }
    }
    throw Error("Invalid component");
}

template <typename T>
const T& ComponentPool<T>::_componentWithId(ComponentId id) const
{
    if (id < _components.size())
    {
        T& component = _components[id];
        if (component._inPool())
        {
            return component;
        }
    }
    throw Error("Invalid component");
}

template <typename T>
template <typename U>
bool ComponentPool<T>::_expandVector(std::vector<U>& vector, size_t size, U value)
{
    bool expanded = false;
    while (size >= vector.size())
    {
        expanded = true;
        size_t oldSize = vector.size();
        vector.resize(std::max(oldSize * 2, (size_t)8), value);
    }
    return expanded;
}

}