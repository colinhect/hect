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
#include <cstdint>
#include <vector>

#include "Hect/Core/IdPool.h"
#include "Hect/Core/Dispatcher.h"
#include "Hect/Logic/Component.h"

#pragma warning (disable : 4505)

namespace hect
{

///
/// A component pool event type.
namespace ComponentPoolEventType
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

///
/// An event dispatched from a ComponentPool.
class ComponentPoolEvent
{
public:

    ///
    /// Constructs a component pool event.
    ///
    /// \param type The event type.
    /// \param entity The id of the entity that the event pertains to.
    ComponentPoolEvent(ComponentPoolEventType::Enum type, EntityId entityId);

    ///
    /// The event type.
    ComponentPoolEventType::Enum type;

    ///
    /// The id of the entity that the event pertains to.
    EntityId entityId;
};

///
/// Abstract interface to a ComponentPool of any component type.
class ComponentPoolBase
{
public:

    ///
    /// Returns the dispatcher of component pool events.
    virtual Dispatcher<ComponentPoolEvent>& dispatcher() = 0;

    virtual void addBase(Entity& entity, const ComponentBase& component) = 0;

    virtual bool remove(Entity& entity) = 0;
    virtual void clone(const Entity& source, Entity& dest) = 0;

    virtual bool has(const Entity& entity) const = 0;

    virtual ComponentBase& getBase(Entity& entity) = 0;
};

class Scene;

template <typename T>
class ComponentPool :
    public ComponentPoolBase
{
public:
    ComponentPool(Scene& scene);

    ///
    /// Returns the dispatcher of component pool events.
    Dispatcher<ComponentPoolEvent>& dispatcher();

    Entity& entityForComponent(ComponentId id);
    const Entity& entityForComponent(ComponentId id) const;

    void addBase(Entity& entity, const ComponentBase& component);

    Component<T>::Iter add(Entity& entity, T component);
    Component<T>::Iter replace(Entity& entity, T component);
    void remove(Entity& entity);

    void clone(const Entity& source, Entity& dest);

    bool has(const Entity& entity) const;

    ComponentBase& getBase(Entity& entity);

    Component<T>::Iter get(Entity& entity);
    Component<T>::ConstIter get(const Entity& entity) const;

    Component<T>::Iter begin();
    Component<T>::ConstIter begin() const;
    Component<T>::Iter end();
    Component<T>::ConstIter end() const;

private:
    template <typename U>
    bool _expandVector(std::vector<U>& vector, size_t size, U value = U());

    Scene* _scene;
    Dispatcher<ComponentPoolEvent> _dispatcher;
    IdPool<ComponentId> _idPool;
    std::vector<T> _components;
    std::vector<ComponentId> _entityToComponent;
    std::vector<EntityId> _componentToEntity;
};

}

#include "ComponentPool.inl"
