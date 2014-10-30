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
#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include "Hect/Event/Dispatcher.h"
#include "Hect/Logic/Component.h"
#include "Hect/Logic/ComponentEvent.h"
#include "Hect/Logic/IdPool.h"

namespace hect
{

class Scene;

class ComponentPoolBase
{
    friend class Scene;
public:

    typedef std::shared_ptr<ComponentPoolBase> Pointer;

protected:
    virtual void dispatchEvent(ComponentEventType type, Entity& entity) = 0;

    virtual void addBase(Entity& entity, const ComponentBase& component) = 0;
    virtual const ComponentBase& getBase(const Entity& entity) const = 0;

    virtual void remove(Entity& entity) = 0;
    virtual void clone(const Entity& source, Entity& dest) = 0;

    virtual bool has(const Entity& entity) const = 0;
};

///
/// A pool of components of a specific type within a Scene.
template <typename T>
class ComponentPool :
    public ComponentPoolBase,
    public Dispatcher<ComponentEvent<T>>
{
    friend class Scene;
    friend class Entity;
    friend class Component<T>;
    friend class Component<T>::IteratorBase;
public:
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
    /// Returns an iterator to the first component matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An iterator to the first matching component; invalid if there
    /// was no matching component.
    typename Component<T>::Iterator findFirst(typename Component<T>::Predicate predicate);

    ///
    /// Returns an iterator to the first component matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An iterator to the first matching component; invalid if there
    /// was no matching component.
    typename Component<T>::ConstIterator findFirst(typename Component<T>::Predicate predicate) const;

    ///
    /// Returns iterators to all components matching the given predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns A vector of iterators to the matching components.
    typename Component<T>::Iterator::Vector find(typename Component<T>::Predicate predicate);

    ///
    /// Returns iterators to all components matching the given predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns A vector of iterators to the matching components.
    typename Component<T>::ConstIterator::Vector find(typename Component<T>::Predicate predicate) const;

private:
    void dispatchEvent(ComponentEventType type, Entity& entity);

    void addBase(Entity& entity, const ComponentBase& component);
    const ComponentBase& getBase(const Entity& entity) const;

    void remove(Entity& entity);
    void clone(const Entity& source, Entity& dest);

    bool has(const Entity& entity) const;

    typename Component<T>::Iterator add(Entity& entity, const T& component);
    typename Component<T>::Iterator replace(Entity& entity, const T& component);

    typename Component<T>::Iterator get(Entity& entity);
    typename Component<T>::ConstIterator get(const Entity& entity) const;

    ComponentId maxId() const;

    bool componentHasEntity(ComponentId id) const;

    Entity& entityForComponent(ComponentId id);
    const Entity& entityForComponent(ComponentId id) const;

    T& componentWithId(ComponentId id);
    const T& componentWithId(ComponentId id) const;

    bool entityIdToComponentId(EntityId entityId, ComponentId& id) const;

    template <typename U>
    bool expandVector(std::vector<U>& vector, size_t size, U value = U());

    Scene* _scene;
    IdPool<ComponentId> _idPool;
    std::vector<T> _components;
    std::vector<ComponentId> _entityToComponent;
    std::vector<EntityId> _componentToEntity;
};

}
