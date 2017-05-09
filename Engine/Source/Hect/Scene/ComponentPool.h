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
#pragma once

#include <deque>
#include <vector>

#include "Hect/Core/EventDispatcher.h"
#include "Hect/Core/Export.h"
#include "Hect/Scene/Component.h"
#include "Hect/Scene/ComponentEvent.h"
#include "Hect/Scene/ComponentIterator.h"
#include "Hect/Scene/EntityIterator.h"
#include "Hect/Scene/IdPool.h"

namespace hect
{

class Scene;

///
/// Abstract base for ComponentPool.
class HECT_EXPORT ComponentPoolBase
{
    friend class Scene;
public:
    virtual ~ComponentPoolBase() = default;

protected:
    virtual void dispatchEvent(ComponentEventType type, Entity& entity) = 0;

    virtual void addBase(Entity& entity, const ComponentBase& component) = 0;
    virtual ComponentBase& getBase(Entity& entity) = 0;
    virtual const ComponentBase& getBase(const Entity& entity) const = 0;

    virtual void remove(Entity& entity) = 0;
    virtual void clone(const Entity& source, Entity& dest) = 0;

    virtual bool has(const Entity& entity) const = 0;
};

///
/// A pool of Component%s of a specific type within a Scene.
template <typename T>
class ComponentPool :
    public ComponentPoolBase,
    public EventDispatcher<ComponentEvent<T>>
{
    friend class Scene;
    friend class Component<T>;
    friend class ComponentIteratorBase<T>;
    friend class Entity;
public:

    ///
    /// Constructs a component pool
    ///
    /// \param scene The scene.
    ComponentPool(Scene& scene);

    ///
    /// Returns an iterator to the beginning of the pool.
    typename Component<T>::Iterator begin();

    ///
    /// Returns an iterator to the beginning of the pool.
    typename Component<T>::ConstIterator begin() const;

    ///
    /// Returns an iterator to the end of the pool.
    typename Component<T>::Iterator end();

    ///
    /// Returns an iterator to the end of the pool.
    typename Component<T>::ConstIterator end() const;

    ///
    /// Returns a handle to the first Component matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search; must be callable
    /// as a function accepting a reference to an Entity, returning whether
    /// the entity matches the predicate.
    ///
    /// \returns A hande to the first matching component; invalid if there
    /// was no matching component.
    template <typename U>
    typename Component<T>::Handle findFirst(U&& predicate) const;

    ///
    /// Returns handles to all Component%s matching the given predicate.
    ///
    /// \param predicate The predicate to use in the search; must be callable
    /// as a function accepting a reference to an Entity, returning whether
    /// the entity matches the predicate.
    ///
    /// \returns A vector of handles to the matching components.
    template <typename U>
    std::vector<typename Component<T>::Handle> find(U&& predicate) const;

    ///
    /// Returns the Component with the given id.
    ///
    /// \param id The id of the component to get.
    ///
    /// \throws InvalidOperation If no component with the given id exists in
    /// the pool.
    T& withId(ComponentId id);

    ///
    /// \copydoc hect::ComponentPool::withId()
    const T& withId(ComponentId id) const;

private:
    void dispatchEvent(ComponentEventType type, Entity& entity) override;

    void addBase(Entity& entity, const ComponentBase& component) override;
    ComponentBase& getBase(Entity& entity) override;
    const ComponentBase& getBase(const Entity& entity) const override;

    void remove(Entity& entity) override;
    void clone(const Entity& source, Entity& dest) override;

    bool has(const Entity& entity) const override;

    T& add(Entity& entity, const T& component);
    T& replace(Entity& entity, const T& component);

    T& get(Entity& entity);
    const T& get(const Entity& entity) const;

    ComponentId maxId() const;

    bool componentHasEntity(ComponentId id) const;

    Entity& entityForComponent(ComponentId id);
    const Entity& entityForComponent(ComponentId id) const;

    EntityId componentIdToEntityId(ComponentId id) const;

    bool entityIdToComponentId(EntityId entityId, ComponentId& id) const;

    T& lookUpComponent(ComponentId id);
    const T& lookUpComponent(ComponentId id) const;

    template <typename U>
    bool expandVector(std::vector<U>& vector, size_t size, U value = U());

    Scene& _scene;
    IdPool<ComponentId> _idPool;

    std::deque<T> _components;

    std::vector<ComponentId> _entityToComponent;
    std::vector<EntityId> _componentToEntity;
};

}
