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
ComponentPool<T>::ComponentPool(World& world, const std::string& componentTypeName) :
    _world(&world),
    _componentTypeName(componentTypeName)
{
}

template <typename T>
typename Component<T>::Iterator ComponentPool<T>::begin()
{
    Component<T>::Iterator iterator(*this, 0);

    // Move to the first component with activated entity
    if (!iterator || !iterator->entity().isActivated())
    {
        ++iterator;
    }
    return iterator;
}

template <typename T>
typename Component<T>::ConstIterator ComponentPool<T>::begin() const
{
    Component<T>::ConstIterator iterator(*this, 0);

    // Move to the first component with activated entity
    if (!iterator || !iterator->entity().isActivated())
    {
        ++iterator;
    }
    return iterator;
}

template <typename T>
typename Component<T>::Iterator ComponentPool<T>::end()
{
    return Component<T>::Iterator(*this, std::max(maxId(), (ComponentId)1));
}

template <typename T>
typename Component<T>::ConstIterator ComponentPool<T>::end() const
{
    return Component<T>::ConstIterator(*this, std::max(maxId(), (ComponentId)1));
}

template <typename T>
typename Component<T>::Iterator ComponentPool<T>::findFirst(typename Component<T>::Predicate predicate)
{
    for (auto iterator = begin(); iterator != end(); ++iterator)
    {
        if (predicate(*iterator))
        {
            return iterator;
        }
    }
    return end();
}

template <typename T>
typename Component<T>::ConstIterator ComponentPool<T>::findFirst(typename Component<T>::Predicate predicate) const
{
    for (auto iterator = begin(); iterator != end(); ++iterator)
    {
        if (predicate(*iterator))
        {
            return iterator;
        }
    }
    return end();
}

template <typename T>
typename Component<T>::Iterator::Vector ComponentPool<T>::find(typename Component<T>::Predicate predicate)
{
    Component<T>::Iterator::Vector results;
    for (auto iterator = begin(); iterator != end(); ++iterator)
    {
        if (predicate(*iterator))
        {
            results.push_back(iterator);
        }
    }
    return results;
}

template <typename T>
typename Component<T>::ConstIterator::Vector ComponentPool<T>::find(typename Component<T>::Predicate predicate) const
{
    Component<T>::ConstIterator::Vector results;
    for (auto iterator = begin(); iterator != end(); ++iterator)
    {
        if (predicate(*iterator))
        {
            results.push_back(iterator);
        }
    }
    return results;
}

template <typename T>
void ComponentPool<T>::dispatchEvent(ComponentEventType type, Entity& entity)
{
    ComponentEvent<T> event(type, entity);
    Dispatcher<ComponentEvent<T>>::dispatchEvent(event);
}

template <typename T>
void ComponentPool<T>::addBase(Entity& entity, const ComponentBase& component)
{
    add(entity, *(T*)&component);
}

template <typename T>
const ComponentBase& ComponentPool<T>::getBase(const Entity& entity) const
{
    auto component = get(entity);
    assert(component);
    return *component;
}

template <typename T>
void ComponentPool<T>::remove(Entity& entity)
{
    EntityId entityId = entity.id();

    ComponentId id;
    if (entityIdToComponentId(entityId, id))
    {
        // If the entity is activated then dispatch the remove event
        if (entity.isActivated())
        {
            dispatchEvent(ComponentEventType_Remove, entity);
        }

        // Destory the component id to be re-used
        _idPool.destroy(id);

        // Remove the component from the pool
        _components[id].exitPool();

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
void ComponentPool<T>::clone(const Entity& source, Entity& dest)
{
    EntityId sourceEntityId = source.id();

    ComponentId id;
    if (entityIdToComponentId(sourceEntityId, id))
    {
        // Add the component of the source entity to the destination entity
        add(dest, _components[id]);
    }
}

template <typename T>
bool ComponentPool<T>::has(const Entity& entity) const
{
    EntityId entityId = entity.id();

    ComponentId id;
    return entityIdToComponentId(entityId, id);
}

template <typename T>
typename Component<T>::Iterator ComponentPool<T>::add(Entity& entity, const T& component)
{
    EntityId entityId = entity.id();

    // The component being added might be a reference to a component in the
    // component vector which might be resized; so it needs to be copied first
    T copiedComponent = component;

    // Expand the entity-to-component vector if needed
    expandVector(_entityToComponent, entityId, (ComponentId)-1);

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
    if (expandVector(_components, id))
    {
        // The component vector was resize, so each valid component needs to be
        // re-added to the pool
        for (ComponentId id = 0; id < _components.size(); ++id)
        {
            if (componentHasEntity(id))
            {
                _components[id].enterPool(*this, id);
            }
        }
    }

    // Expand the component-to-entity vector if needed
    expandVector(_componentToEntity, id, (EntityId)-1);

    // Remember which entity this component belongs to
    _componentToEntity[id] = entityId;

    // Assign the new component and get a reference to it
    T& addedComponent = _components[id] = copiedComponent;

    // Include the component in the pool
    addedComponent.enterPool(*this, id);

    // Dispatch the add event if the entity is activated
    if (entity.isActivated())
    {
        dispatchEvent(ComponentEventType_Add, entity);
    }

    return Component<T>::Iterator(*this, id);
}

template <typename T>
typename Component<T>::Iterator ComponentPool<T>::replace(Entity& entity, const T& component)
{
    EntityId entityId = entity.id();

    ComponentId id;
    if (entityIdToComponentId(entityId, id))
    {
        // If the entity is activated then dispatch the remove event
        if (entity.isActivated())
        {
            dispatchEvent(ComponentEventType_Remove, entity);
        }

        // Get the old component
        T& addedComponent = _components[id];

        // Remove the old component from the pool
        addedComponent.exitPool();

        // Replace the old component with the new component
        addedComponent = component;

        // Include the new component in the pool
        addedComponent.enterPool(*this, id);

        // If the entity is activated then dispatch the add event
        if (entity.isActivated())
        {
            dispatchEvent(ComponentEventType_Add, entity);
        }

        return Component<T>::Iterator(*this, id);
    }
    else
    {
        throw Error(format("Entity does not have component of type '%s'", _componentTypeName.c_str()));
    }
}

template <typename T>
typename Component<T>::Iterator ComponentPool<T>::get(Entity& entity)
{
    EntityId entityId = entity.id();

    ComponentId id;
    if (entityIdToComponentId(entityId, id))
    {
        return Component<T>::Iterator(*this, id);
    }
    else
    {
        return end();
    }
}

template <typename T>
typename Component<T>::ConstIterator ComponentPool<T>::get(const Entity& entity) const
{
    EntityId entityId = entity.id();

    ComponentId id;
    if (entityIdToComponentId(entityId, id))
    {
        return Component<T>::ConstIterator(*this, id);
    }
    else
    {
        return end();
    }
}

template <typename T>
ComponentId ComponentPool<T>::maxId() const
{
    return (ComponentId)_components.size();
}

template <typename T>
bool ComponentPool<T>::componentHasEntity(ComponentId id) const
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
Entity& ComponentPool<T>::entityForComponent(ComponentId id)
{
    const Entity& entity = const_cast<const ComponentPool<T>*>(this)->entityForComponent(id);
    return const_cast<Entity&>(entity);
}

template <typename T>
const Entity& ComponentPool<T>::entityForComponent(ComponentId id) const
{
    if (id < _componentToEntity.size())
    {
        EntityId entityId = _componentToEntity[id];
        if (entityId != (EntityId)-1)
        {
            return _world->entities().entityWithId(entityId);
        }
    }
    throw Error("Component does not have an associated entity");
}

template <typename T>
T& ComponentPool<T>::componentWithId(ComponentId id)
{
    const T& component = const_cast<const ComponentPool<T>*>(this)->componentWithId(id);
    return const_cast<T&>(component);
}

template <typename T>
const T& ComponentPool<T>::componentWithId(ComponentId id) const
{
    if (id < _components.size())
    {
        const T& component = _components[id];
        if (component.inPool())
        {
            return component;
        }
    }
    
    throw Error("Invalid component");
}

template <typename T>
bool ComponentPool<T>::entityIdToComponentId(EntityId entityId, ComponentId& id) const
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
bool ComponentPool<T>::expandVector(std::vector<U>& vector, size_t size, U value)
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