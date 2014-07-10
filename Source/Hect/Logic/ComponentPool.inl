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
ComponentPool<T>::ComponentPool(Scene& scene, const std::string& componentTypeName) :
    _scene(&scene),
    _componentTypeName(componentTypeName)
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
typename Component<T>::Iter ComponentPool<T>::findFirst(typename Component<T>::Predicate predicate)
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
typename Component<T>::ConstIter ComponentPool<T>::findFirst(typename Component<T>::Predicate predicate) const
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
typename Component<T>::Iter::Array ComponentPool<T>::find(typename Component<T>::Predicate predicate)
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
typename Component<T>::ConstIter::Array ComponentPool<T>::find(typename Component<T>::Predicate predicate) const
{
    Component<T>::ConstIter::Array results;
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
void ComponentPool<T>::_notifyEvent(ComponentEventType type, Entity& entity)
{
    ComponentEvent<T> event(type, entity);
    _dispatcher.dispatchEvent(event);
}

template <typename T>
void ComponentPool<T>::_addBase(Entity& entity, const ComponentBase& component)
{
    _add(entity, *(T*)&component);
}

template <typename T>
const ComponentBase& ComponentPool<T>::_getBase(const Entity& entity) const
{
    auto component = _get(entity);
    assert(component);
    return *component;
}

template <typename T>
void ComponentPool<T>::_remove(Entity& entity)
{
    EntityId entityId = entity.id();

    ComponentId id;
    if (_entityIdToComponentId(entityId, id))
    {
        // If the entity is activated then dispatch the remove event
        if (entity.isActivated())
        {
            _notifyEvent(ComponentEventType_Remove, entity);
        }

        // Destory the component id to be re-used
        _idPool.destroy(id);

        // Remove the component from the pool
        _components[id]._exitPool();

        // Clear the mapping from entity tocomponent and component to
        // entity
        _componentToEntity[id] = (EntityId)-1;
        _entityToComponent[entityId] = (ComponentId)-1;
    }
    else
    {
        throw Error(format("Entity does not have component of type '%s'", _componentTypeName.c_str()));
    }
}

template <typename T>
void ComponentPool<T>::_clone(const Entity& source, Entity& dest)
{
    EntityId sourceEntityId = source.id();

    ComponentId id;
    if (_entityIdToComponentId(sourceEntityId, id))
    {
        // Add the component of the source entity to the destination entity
        _add(dest, _components[id]);
    }
}

template <typename T>
bool ComponentPool<T>::_has(const Entity& entity) const
{
    EntityId entityId = entity.id();

    ComponentId id;
    return _entityIdToComponentId(entityId, id);
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
    ComponentId id = _entityToComponent[entityId];

    // Ensure that the entity does not already have a component of this type
    if (id != (ComponentId)-1)
    {
        throw Error(format("Entity already has component of type '%s'", _componentTypeName.c_str()));
    }

    // Create the new component id
    id = _idPool.create();
    _entityToComponent[entityId] = id;

    // Expand the component vector if needed
    if (_expandVector(_components, id))
    {
        // The component vector was resize, so each valid component needs to be
        // re-added to the pool
        for (ComponentId id = 0; id < _components.size(); ++id)
        {
            if (_componentHasEntity(id))
            {
                _components[id]._enterPool(*this, id);
            }
        }
    }

    // Expand the component-to-entity vector if needed
    _expandVector(_componentToEntity, id, (EntityId)-1);

    // Remember which entity this component belongs to
    _componentToEntity[id] = entityId;

    // Assign the new component and get a reference to it
    T& addedComponent = _components[id] = copiedComponent;

    // Include the component in the pool
    addedComponent._enterPool(*this, id);

    // Dispatch the add event if the entity is activated
    if (entity.isActivated())
    {
        _notifyEvent(ComponentEventType_Add, entity);
    }

    return Component<T>::Iter(*this, id);
}

template <typename T>
typename Component<T>::Iter ComponentPool<T>::_replace(Entity& entity, const T& component)
{
    EntityId entityId = entity.id();

    ComponentId id;
    if (_entityIdToComponentId(entityId, id))
    {
        // If the entity is activated then dispatch the remove event
        if (entity.isActivated())
        {
            _notifyEvent(ComponentEventType_Remove, entity);
        }

        // Get the old component
        T& addedComponent = _components[id];

        // Remove the old component from the pool
        addedComponent._exitPool();

        // Replace the old component with the new component
        addedComponent = component;

        // Include the new component in the pool
        addedComponent._enterPool(*this, id);

        // If the entity is activated then dispatch the add event
        if (entity.isActivated())
        {
            _notifyEvent(ComponentEventType_Add, entity);
        }

        return Component<T>::Iter(*this, id);
    }
    else
    {
        throw Error(format("Entity does not have component of type '%s'", _componentTypeName.c_str()));
    }
}

template <typename T>
typename Component<T>::Iter ComponentPool<T>::_get(Entity& entity)
{
    EntityId entityId = entity.id();

    ComponentId id;
    if (_entityIdToComponentId(entityId, id))
    {
        return Component<T>::Iter(*this, id);
    }
    else
    {
        return end();
    }
}

template <typename T>
typename Component<T>::ConstIter ComponentPool<T>::_get(const Entity& entity) const
{
    EntityId entityId = entity.id();

    ComponentId id;
    if (_entityIdToComponentId(entityId, id))
    {
        return Component<T>::ConstIter(*this, id);
    }
    else
    {
        return end();
    }
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
    const Entity& entity = const_cast<const ComponentPool<T>*>(this)->_entityForComponent(id);
    return const_cast<Entity&>(entity);
}

template <typename T>
const Entity& ComponentPool<T>::_entityForComponent(ComponentId id) const
{
    if (id < _componentToEntity.size())
    {
        EntityId entityId = _componentToEntity[id];
        if (entityId != (EntityId)-1)
        {
            return _scene->entities()._entityWithId(entityId);
        }
    }
    throw Error("Component does not have an associated entity");
}

template <typename T>
T& ComponentPool<T>::_componentWithId(ComponentId id)
{
    const T& component = const_cast<const ComponentPool<T>*>(this)->_componentWithId(id);
    return const_cast<T&>(component);
}

template <typename T>
const T& ComponentPool<T>::_componentWithId(ComponentId id) const
{
    if (id < _components.size())
    {
        const T& component = _components[id];
        if (component._inPool())
        {
            return component;
        }
    }
    
    throw Error("Invalid component");
}

template <typename T>
bool ComponentPool<T>::_entityIdToComponentId(EntityId entityId, ComponentId& id) const
{
    // If the entity id is within range for the pool
    if (entityId < _entityToComponent.size())
    {
        id = _entityToComponent[entityId];
    }
    else
    {
        id = (ComponentId)-1;
    }

    return id != (ComponentId)-1;
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