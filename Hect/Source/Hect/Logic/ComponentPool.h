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
#include <functional>
#include <cstdint>
#include <vector>

#include "Hect/Core/IdPool.h"
#include "Hect/Core/Dispatcher.h"
#include "Hect/Logic/Component.h"

namespace hect
{

namespace ComponentEventType
{
enum Enum
{
    ///
    /// A component was added.
    Add,

    ///
    /// A component was removed.
    Remove
};
}

template <typename T>
class ComponentEvent
{
public:
    ComponentEvent(ComponentEventType::Enum type, Entity& entity);

    ComponentEventType::Enum type() const;
    Entity& entity() const;

private:
    ComponentEventType::Enum _type;
    mutable Entity* _entity;
};

class ComponentPoolBase
{
    friend class Scene;
protected:
    virtual void _notifyEvent(ComponentEventType::Enum type, Entity& entity) = 0;

    virtual void _addBase(Entity& entity, const ComponentBase& component) = 0;
    virtual void _remove(Entity& entity) = 0;
    virtual void _clone(const Entity& source, Entity& dest) = 0;

    virtual bool _has(const Entity& entity) const = 0;

    virtual ComponentBase& _getBase(Entity& entity) = 0;
    virtual const ComponentBase& _getBase(const Entity& entity) const = 0;
};

template <typename T>
class ComponentPool :
    public ComponentPoolBase
{
    friend class Scene;
    friend class Entity;
    friend class Component<T>;
    friend class Component<T>::IterBase;
public:
    typedef std::function<bool(const T&)> Predicate;

    ComponentPool(Scene& scene);

    Dispatcher<ComponentEvent<T>>& dispatcher();

    typename Component<T>::Iter begin();
    typename Component<T>::ConstIter begin() const;
    typename Component<T>::Iter end();
    typename Component<T>::ConstIter end() const;

    typename Component<T>::Iter findFirst(Predicate predicate);
    typename Component<T>::ConstIter findFirst(Predicate predicate) const;

    typename Component<T>::Iter::Array find(Predicate predicate);
    typename Component<T>::ConstIter::Array find(Predicate predicate) const;

private:
    void _notifyEvent(ComponentEventType::Enum type, Entity& entity);

    void _addBase(Entity& entity, const ComponentBase& component);
    void _remove(Entity& entity);
    void _clone(const Entity& source, Entity& dest);

    bool _has(const Entity& entity) const;

    ComponentBase& _getBase(Entity& entity);
    const ComponentBase& _getBase(const Entity& entity) const;

    typename Component<T>::Iter _add(Entity& entity, const T& component);
    typename Component<T>::Iter _replace(Entity& entity, const T& component);

    typename Component<T>::Iter _get(Entity& entity);
    typename Component<T>::ConstIter _get(const Entity& entity) const;

    ComponentId _maxId() const;

    bool _componentHasEntity(ComponentId id) const;

    Entity& _entityForComponent(ComponentId id);
    const Entity& _entityForComponent(ComponentId id) const;

    T& _componentWithId(ComponentId id);
    const T& _componentWithId(ComponentId id) const;
    
    template <typename U>
    bool _expandVector(std::vector<U>& vector, size_t size, U value = U());

    Scene* _scene;
    Dispatcher<ComponentEvent<T>> _dispatcher;
    IdPool<ComponentId> _idPool;
    std::vector<T> _components;
    std::vector<ComponentId> _entityToComponent;
    std::vector<EntityId> _componentToEntity;
};

}

#include "ComponentPool.inl"