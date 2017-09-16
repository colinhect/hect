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
    if (!iterator || !iterator->entity().is_activated())
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
    if (!iterator || !iterator->entity().is_activated())
    {
        ++iterator;
    }
    return iterator;
}

template <typename T>
typename Component<T>::Iterator ComponentPool<T>::end()
{
    return typename Component<T>::Iterator(*this, std::max(max_id(), static_cast<ComponentId>(1)));
}

template <typename T>
typename Component<T>::ConstIterator ComponentPool<T>::end() const
{
    return typename Component<T>::ConstIterator(*this, std::max(max_id(), static_cast<ComponentId>(1)));
}

template <typename T>
template <typename U>
typename Component<T>::Handle ComponentPool<T>::find_first(U&& predicate) const
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
T& ComponentPool<T>::with_id(ComponentId id)
{
    const T& component = const_cast<const ComponentPool<T>*>(this)->with_id(id);
    return const_cast<T&>(component);
}

template <typename T>
const T& ComponentPool<T>::with_id(ComponentId id) const
{
    if (id < max_id())
    {
        const T& component = look_up_component(id);
        if (component.in_pool())
        {
            return component;
        }
    }

    throw InvalidOperation("Invalid component");
}

template <typename T>
void ComponentPool<T>::dispatch_event(ComponentEventType type, Entity& entity)
{
    ComponentEvent<T> event;
    event.type = type;
    event.entity = entity.handle();

    EventDispatcher<ComponentEvent<T>>::dispatch_event(event);
}

template <typename T>
void ComponentPool<T>::add_base(Entity& entity, const ComponentBase& component)
{
    add(entity, *const_cast<T*>(reinterpret_cast<const T*>(&component)));
}

template <typename T>
ComponentBase& ComponentPool<T>::get_base(Entity& entity)
{
    return get(entity);
}

template <typename T>
const ComponentBase& ComponentPool<T>::get_base(const Entity& entity) const
{
    return get(entity);
}

template <typename T>
void ComponentPool<T>::remove(Entity& entity)
{
    EntityId entity_id = entity.id();

    ComponentId id;
    if (entity_id_to_component_id(entity_id, id))
    {
        // If the entity is activated then dispatch the remove event
        if (entity.is_activated())
        {
            dispatch_event(ComponentEventType::Remove, entity);
        }

        // Destory the component id to be re-used
        _id_pool.destroy(id);

        // Remove the component from the pool
        T& component = look_up_component(id);
        component.exit_pool();

        // Clear the mapping from entity to component and component to
        // entity
        _component_to_entity[id] = EntityId(-1);
        _entity_to_component[entity_id] = ComponentId(-1);
    }
    else
    {
        Name type_name = Type::get<T>().name();
        throw InvalidOperation(format("Entity does not have component of type '%s'", type_name.data()));
    }
}

template <typename T>
void ComponentPool<T>::clone(const Entity& source, Entity& dest)
{
    EntityId source_entity_id = source.id();

    ComponentId id;
    if (entity_id_to_component_id(source_entity_id, id))
    {
        // Add the component of the source entity to the destination entity
        const T& component = look_up_component(id);
        add(dest, component);
    }
}

template <typename T>
bool ComponentPool<T>::has(const Entity& entity) const
{
    EntityId entity_id = entity.id();

    ComponentId id;
    return entity_id_to_component_id(entity_id, id);
}

template <typename T>
T& ComponentPool<T>::add(Entity& entity, const T& component)
{
    EntityId entity_id = entity.id();

    // The component being added might be a reference to a component in the
    // component vector which might be resized; so it needs to be copied first
    T copied_component = component;

    // Expand the entity-to-component vector if needed
    expand_vector(_entity_to_component, entity_id, ComponentId(-1));

    // Get the component id for the entity
    ComponentId id = _entity_to_component[entity_id];

    // Ensure that the entity does not already have a component of this type
    if (id != ComponentId(-1))
    {
        const Name type_name = Type::get<T>().name();
        throw InvalidOperation(format("Entity already has component of type '%s'", type_name.data()));
    }

    // Create the new component id and expand the components deque if needed
    id = _id_pool.create();
    while (id >= max_id())
    {
        _components.emplace_back(T());
    }

    // Remember which component this entity has
    _entity_to_component[entity_id] = id;

    // Expand the component-to-entity vector if needed
    expand_vector(_component_to_entity, id, EntityId(-1));

    // Remember which entity this component belongs to
    _component_to_entity[id] = entity_id;

    // Copy the added component into the pool
    T& added_component = look_up_component(id);
    added_component = copied_component;

    // Include the component in the pool
    added_component.enter_pool(*this, id);

    // Dispatch the add event if the entity is activated
    if (entity.is_activated())
    {
        dispatch_event(ComponentEventType::Add, entity);
    }

    return added_component;
}

template <typename T>
T& ComponentPool<T>::replace(Entity& entity, const T& component)
{
    EntityId entity_id = entity.id();

    ComponentId id;
    if (entity_id_to_component_id(entity_id, id))
    {
        // If the entity is activated then dispatch the remove event
        if (entity.is_activated())
        {
            dispatch_event(ComponentEventType::Remove, entity);
        }

        // Get the old component
        T& added_component = look_up_component(id);

        // Remove the old component from the pool
        added_component.exit_pool();

        // Replace the old component with the new component
        added_component = component;

        // Include the new component in the pool
        added_component.enter_pool(*this, id);

        // If the entity is activated then dispatch the add event
        if (entity.is_activated())
        {
            dispatch_event(ComponentEventType::Add, entity);
        }

        return added_component;
    }
    else
    {
        Name type_name = Type::get<T>().name();
        throw InvalidOperation(format("Entity does not have component of type '%s'", type_name.data()));
    }
}

template <typename T>
T& ComponentPool<T>::get(Entity& entity)
{
    EntityId entity_id = entity.id();

    ComponentId id;
    if (entity_id_to_component_id(entity_id, id))
    {
        return look_up_component(id);
    }
    else
    {
        Name type_name = Type::get<T>().name();
        throw InvalidOperation(format("Entity does not have component of type '%s'", type_name.data()));
    }
}

template <typename T>
const T& ComponentPool<T>::get(const Entity& entity) const
{
    EntityId entity_id = entity.id();

    ComponentId id;
    if (entity_id_to_component_id(entity_id, id))
    {
        return look_up_component(id);
    }
    else
    {
        Name type_name = Type::get<T>().name();
        throw InvalidOperation(format("Entity does not have component of type '%s'", type_name.data()));
    }
}

template <typename T>
ComponentId ComponentPool<T>::max_id() const
{
    return static_cast<ComponentId>(_components.size());
}

template <typename T>
bool ComponentPool<T>::component_has_entity(ComponentId id) const
{
    if (id < _component_to_entity.size())
    {
        if (_component_to_entity[id] != EntityId(-1))
        {
            return true;
        }
    }
    return false;
}

template <typename T>
Entity& ComponentPool<T>::entity_for_component(ComponentId id)
{
    const Entity& entity = const_cast<const ComponentPool<T>*>(this)->entity_for_component(id);
    return const_cast<Entity&>(entity);
}

template <typename T>
const Entity& ComponentPool<T>::entity_for_component(ComponentId id) const
{
    EntityId entity_id = component_id_to_entity_id(id);
    return _scene.entities().entity_with_id(entity_id);
}

template <typename T>
EntityId ComponentPool<T>::component_id_to_entity_id(ComponentId id) const
{
    if (id < _component_to_entity.size())
    {
        EntityId entity_id = _component_to_entity[id];
        if (entity_id != EntityId(-1))
        {
            return entity_id;
        }
    }
    throw InvalidOperation("Component does not have an associated entity");
}

template <typename T>
bool ComponentPool<T>::entity_id_to_component_id(EntityId entity_id, ComponentId& id) const
{
    // If the entity id is within range for the pool
    if (entity_id < _entity_to_component.size())
    {
        id = _entity_to_component[entity_id];
    }
    else
    {
        id = ComponentId(-1);
    }

    return id != ComponentId(-1);
}

template <typename T>
T& ComponentPool<T>::look_up_component(ComponentId id)
{
    return _components[id];
}

template <typename T>
const T& ComponentPool<T>::look_up_component(ComponentId id) const
{
    return _components[id];
}

template <typename T>
template <typename U>
bool ComponentPool<T>::expand_vector(std::vector<U>& vector, size_t size, U value)
{
    bool expanded = false;
    while (size >= vector.size())
    {
        expanded = true;
        size_t old_size = vector.size();
        vector.resize(std::max(old_size * 2, size_t(8)), value);
    }
    return expanded;
}

}
