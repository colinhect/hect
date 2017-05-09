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

template <typename T>
ComponentPool<T>::ComponentPool(Scene& scene) :
    _scene(scene)
{
}

template <typename T>
typename Component<T>::Iterator ComponentPool<T>::begin()
{
    typename Component<T>::Iterator iterator(*this, 0);

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
    typename Component<T>::ConstIterator iterator(*this, 0);

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
    return typename Component<T>::Iterator(*this, std::max(maxId(), static_cast<ComponentId>(1)));
}

template <typename T>
typename Component<T>::ConstIterator ComponentPool<T>::end() const
{
    return typename Component<T>::ConstIterator(*this, std::max(maxId(), static_cast<ComponentId>(1)));
}

template <typename T>
template <typename U>
typename Component<T>::Handle ComponentPool<T>::findFirst(U&& predicate) const
{
    for (auto iterator = begin(); iterator != end(); ++iterator)
    {
        if (predicate(*iterator))
        {
            return iterator->handle();
        }
    }
    return typename Component<T>::Handle();
}

template <typename T>
template <typename U>
std::vector<typename Component<T>::Handle> ComponentPool<T>::find(U&& predicate) const
{
    std::vector<typename Component<T>::Handle> results;
    for (auto iterator = begin(); iterator != end(); ++iterator)
    {
        if (predicate(*iterator))
        {
            results.push_back(iterator->handle());
        }
    }
    return results;
}

template <typename T>
T& ComponentPool<T>::withId(ComponentId id)
{
    const T& component = const_cast<const ComponentPool<T>*>(this)->withId(id);
    return const_cast<T&>(component);
}

template <typename T>
const T& ComponentPool<T>::withId(ComponentId id) const
{
    if (id < maxId())
    {
        const T& component = lookUpComponent(id);
        if (component.inPool())
        {
            return component;
        }
    }

    throw InvalidOperation("Invalid component");
}

template <typename T>
void ComponentPool<T>::dispatchEvent(ComponentEventType type, Entity& entity)
{
    ComponentEvent<T> event;
    event.type = type;
    event.entity = entity.handle();

    EventDispatcher<ComponentEvent<T>>::dispatchEvent(event);
}

template <typename T>
void ComponentPool<T>::addBase(Entity& entity, const ComponentBase& component)
{
    add(entity, *const_cast<T*>(reinterpret_cast<const T*>(&component)));
}

template <typename T>
ComponentBase& ComponentPool<T>::getBase(Entity& entity)
{
    return get(entity);
}

template <typename T>
const ComponentBase& ComponentPool<T>::getBase(const Entity& entity) const
{
    return get(entity);
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
            dispatchEvent(ComponentEventType::Remove, entity);
        }

        // Destory the component id to be re-used
        _idPool.destroy(id);

        // Remove the component from the pool
        T& component = lookUpComponent(id);
        component.exitPool();

        // Clear the mapping from entity to component and component to
        // entity
        _componentToEntity[id] = EntityId(-1);
        _entityToComponent[entityId] = ComponentId(-1);
    }
    else
    {
        Name typeName = Type::get<T>().name();
        throw InvalidOperation(format("Entity does not have component of type '%s'", typeName.data()));
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
        const T& component = lookUpComponent(id);
        add(dest, component);
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
T& ComponentPool<T>::add(Entity& entity, const T& component)
{
    EntityId entityId = entity.id();

    // The component being added might be a reference to a component in the
    // component vector which might be resized; so it needs to be copied first
    T copiedComponent = component;

    // Expand the entity-to-component vector if needed
    expandVector(_entityToComponent, entityId, ComponentId(-1));

    // Get the component id for the entity
    ComponentId id = _entityToComponent[entityId];

    // Ensure that the entity does not already have a component of this type
    if (id != ComponentId(-1))
    {
        const Name typeName = Type::get<T>().name();
        throw InvalidOperation(format("Entity already has component of type '%s'", typeName.data()));
    }

    // Create the new component id and expand the components deque if needed
    id = _idPool.create();
    while (id >= maxId())
    {
        _components.emplace_back(T());
    }

    // Remember which component this entity has
    _entityToComponent[entityId] = id;

    // Expand the component-to-entity vector if needed
    expandVector(_componentToEntity, id, EntityId(-1));

    // Remember which entity this component belongs to
    _componentToEntity[id] = entityId;

    // Copy the added component into the pool
    T& addedComponent = lookUpComponent(id);
    addedComponent = copiedComponent;

    // Include the component in the pool
    addedComponent.enterPool(*this, id);

    // Dispatch the add event if the entity is activated
    if (entity.isActivated())
    {
        dispatchEvent(ComponentEventType::Add, entity);
    }

    return addedComponent;
}

template <typename T>
T& ComponentPool<T>::replace(Entity& entity, const T& component)
{
    EntityId entityId = entity.id();

    ComponentId id;
    if (entityIdToComponentId(entityId, id))
    {
        // If the entity is activated then dispatch the remove event
        if (entity.isActivated())
        {
            dispatchEvent(ComponentEventType::Remove, entity);
        }

        // Get the old component
        T& addedComponent = lookUpComponent(id);

        // Remove the old component from the pool
        addedComponent.exitPool();

        // Replace the old component with the new component
        addedComponent = component;

        // Include the new component in the pool
        addedComponent.enterPool(*this, id);

        // If the entity is activated then dispatch the add event
        if (entity.isActivated())
        {
            dispatchEvent(ComponentEventType::Add, entity);
        }

        return addedComponent;
    }
    else
    {
        Name typeName = Type::get<T>().name();
        throw InvalidOperation(format("Entity does not have component of type '%s'", typeName.data()));
    }
}

template <typename T>
T& ComponentPool<T>::get(Entity& entity)
{
    EntityId entityId = entity.id();

    ComponentId id;
    if (entityIdToComponentId(entityId, id))
    {
        return lookUpComponent(id);
    }
    else
    {
        Name typeName = Type::get<T>().name();
        throw InvalidOperation(format("Entity does not have component of type '%s'", typeName.data()));
    }
}

template <typename T>
const T& ComponentPool<T>::get(const Entity& entity) const
{
    EntityId entityId = entity.id();

    ComponentId id;
    if (entityIdToComponentId(entityId, id))
    {
        return lookUpComponent(id);
    }
    else
    {
        Name typeName = Type::get<T>().name();
        throw InvalidOperation(format("Entity does not have component of type '%s'", typeName.data()));
    }
}

template <typename T>
ComponentId ComponentPool<T>::maxId() const
{
    return static_cast<ComponentId>(_components.size());
}

template <typename T>
bool ComponentPool<T>::componentHasEntity(ComponentId id) const
{
    if (id < _componentToEntity.size())
    {
        if (_componentToEntity[id] != EntityId(-1))
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
    EntityId entityId = componentIdToEntityId(id);
    return _scene.entities().entityWithId(entityId);
}

template <typename T>
EntityId ComponentPool<T>::componentIdToEntityId(ComponentId id) const
{
    if (id < _componentToEntity.size())
    {
        EntityId entityId = _componentToEntity[id];
        if (entityId != EntityId(-1))
        {
            return entityId;
        }
    }
    throw InvalidOperation("Component does not have an associated entity");
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
        id = ComponentId(-1);
    }

    return id != ComponentId(-1);
}

template <typename T>
T& ComponentPool<T>::lookUpComponent(ComponentId id)
{
    return _components[id];
}

template <typename T>
const T& ComponentPool<T>::lookUpComponent(ComponentId id) const
{
    return _components[id];
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
        vector.resize(std::max(oldSize * 2, size_t(8)), value);
    }
    return expanded;
}

}
