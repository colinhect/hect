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
ComponentPool<T>::ComponentPool(Scene& scene) :
    _scene(&scene)
{
}

template <typename T>
Dispatcher<ComponentPoolEvent>& ComponentPool<T>::dispatcher()
{
    return _dispatcher;
}

template <typename T>
Entity& ComponentPool<T>::entityForComponent(ComponentId id)
{
    if (id < _componentToEntity.size())
    {
        EntityId entityId = _componentToEntity[id];
        if (entityId != (EntityId)-1)
        {
            return _scene->entityFromId(entityId);
        }
    }
    throw Error("Component does not have an associated entity");
}

template <typename T>
const Entity& ComponentPool<T>::entityForComponent(ComponentId id) const
{
    if (id < _componentToEntity.size())
    {
        EntityId entityId = _componentToEntity[id];
        if (entityId != (EntityId)-1)
        {
            return _scene->entityFromId(entityId);
        }
    }
    throw Error("Component does not have an associated entity");
}

template <typename T>
void ComponentPool<T>::addBase(Entity& entity, const ComponentBase& component)
{
    add(entity, *(T*)&component);
}

template <typename T>
typename Component<T>::Iter ComponentPool<T>::add(Entity& entity, T component)
{
    EntityId entityId = entity.id();

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
    T& addedComponent = _components[componentId] = component;
    addedComponent.enterPool(this, componentId);

    // Dispatch the add event if the entity is activated
    if (entity.isActivated())
    {
        _dispatcher.notifyEvent(ComponentPoolEvent(ComponentPoolEventType::Add, entityId));
    }

    return Component<T>::Iter(this, componentId);
}

template <typename T>
typename Component<T>::Iter ComponentPool<T>::replace(Entity& entity, T component)
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
                _dispatcher.notifyEvent(ComponentPoolEvent(ComponentPoolEventType::Remove, entityId));
            }
            
            // Assign the new component and get a reference to it
            T& addedComponent = _components[componentId] = component;

            // If the entity is activated then dispatch the add event
            if (entity.isActivated())
            {
                _dispatcher.notifyEvent(ComponentPoolEvent(ComponentPoolEventType::Add, entityId));
            }

            return Component<T>::Iter(this, componentId);
        }
    }

    throw Error("Entity does not have a component of that type");
}

template <typename T>
void ComponentPool<T>::remove(Entity& entity)
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
                _dispatcher.notifyEvent(ComponentPoolEvent(ComponentPoolEventType::Remove, entityId));
            }

            _idPool.destroy(componentId);

            _components[componentId].exitPool();
            _componentToEntity[componentId] = (EntityId)-1;
            componentId = (ComponentId)-1;
        }
    }

    throw Error("Entity does not have a component of that type");
}

template <typename T>
void ComponentPool<T>::clone(const Entity& source, Entity& dest)
{
    EntityId sourceEntityId = source.id();
    if (sourceEntityId < _entityToComponent.size())
    {
        ComponentId& componentId = _entityToComponent[sourceEntityId];
        if (componentId != (ComponentId)-1)
        {
            add(dest, _components[componentId]);
        }
    }
}

template <typename T>
bool ComponentPool<T>::has(const Entity& entity) const
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
ComponentBase& ComponentPool<T>::getBase(const Entity& entity)
{
    auto component = get(entity);
    if (!component)
    {
        throw Error("Entity does not have a component in this pool");
    }
    return *component;
}

template <typename T>
typename Component<T>::Iter ComponentPool<T>::get(Entity& entity)
{
    EntityId entityId = entity.id();
    if (entityId < _entityToComponent.size())
    {
        ComponentId& componentId = _entityToComponent[entityId];
        if (componentId != (ComponentId)-1)
        {
            return Component<T>::Iter(this, componentId);
        }
    }

    return Component<T>::Iter(this, _components.size());
}

template <typename T>
typename Component<T>::ConstIter ComponentPool<T>::get(const Entity& entity) const
{
    EntityId entityId = entity.id();
    if (entityId < _entityToComponent.size())
    {
        ComponentId& componentId = _entityToComponent[entityId];
        if (componentId != (ComponentId)-1)
        {
            return Component<T>::ConstIter(this, componentId);
        }
    }

    return Component<T>::ConstIter(this, _components.size());
}

template <typename T>
typename Component<T>::Iter ComponentPool<T>::begin()
{
    return Component<T>::Iter(this, 0);
}

template <typename T>
typename Component<T>::ConstIter ComponentPool<T>::begin() const
{
    return Component<T>::ConstIter(this, 0);
}

template <typename T>
typename Component<T>::Iter ComponentPool<T>::end()
{
    return Component<T>::Iter(this, _components.size());
}

template <typename T>
typename Component<T>::ConstIter ComponentPool<T>::end() const
{
    return Component<T>::ConstIter(this, _components.size());
}

template <typename T, typename U>
void ComponentPool<T>::_expandVector(std::vector<U>& vector, size_t size, U value)
{
    bool expanded = false;
    while (size >= vector.size())
    {
        expanded = true;
        size_t oldSize = vector.size();
        vector.resize(std::max(oldSize * 2, (size_t)8));
    }
    return expanded;
}

}