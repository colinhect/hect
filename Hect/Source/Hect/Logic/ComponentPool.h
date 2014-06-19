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
#include "Hect/Logic/ComponentBase.h"

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

    ///
    /// Adds a component to an entity.
    ///
    /// \remarks If the entity already has an entity of the given type then the
    /// existing component is overwritten.
    ///
    /// \param entityId The id of the entity.
    /// \param component The component to add.
    virtual void addBase(EntityId entityId, const ComponentBase& component) = 0;

    ///
    /// Removes a component from an entity.
    ///
    /// \param entityId The id of the entity.
    ///
    /// \returns True if the component was removed from the entity; false if
    /// entity did not have the component.
    virtual bool remove(EntityId entityId) = 0;

    ///
    /// Clones the component from one entity to another.
    ///
    /// \param sourceEntityId The id of the entity to copy the component from.
    /// \param destEntityId The id of the entity to copy the component to.
    virtual void clone(EntityId sourceEntityId, EntityId destEntityId) = 0;

    ///
    /// Returns whether an entity has a component in the pool.
    ///
    /// \param entityId The id of the entity.
    virtual bool has(EntityId entityId) const = 0;

    virtual ComponentBase& getBase(EntityId entityId) = 0;
};

class Scene;

///
/// Contains all of the components of a specific type in a scene.
template <typename T>
class ComponentPool :
    public ComponentPoolBase
{
public:

    ///
    /// A component iterator.
    class Iterator
    {
        friend class ComponentPool<T>;
    public:
        Iterator();

        ///
        /// Returns whether the iterator refers to a valid component.
        bool isValid() const;

        ///
        /// Dereferences the iterator.
        T& operator*() const;

        ///
        /// Provides pointer-like access to the underlying component.
        T* operator->() const;

        ///
        /// Moves to the next component.
        Iterator& operator++();

        ///
        /// Returns whether the iterator refers to the same component as another
        /// iterator.
        ///
        /// \param other The other iterator.
        bool operator==(const Iterator& other) const;

        ///
        /// Returns whether the iterator refers to a different component than
        /// another iterator.
        ///
        /// \param other The other iterator.
        bool operator!=(const Iterator& other) const;

        operator bool() const;

    private:
        Iterator(std::vector<T>* components, size_t index);

        std::vector<T>* _components;
        size_t _index;
    };

    ///
    /// A constant component iterator.
    class ConstIterator
    {
        friend class ComponentPool<T>;
    public:
        ConstIterator();

        ///
        /// Returns whether the iterator refers to a valid component.
        bool isValid() const;

        ///
        /// Dereferences the iterator.
        const T& operator*() const;

        ///
        /// Provides pointer-like access to the underlying component.
        const T* operator->() const;

        ///
        /// Moves to the next component.
        ConstIterator& operator++();

        ///
        /// Returns whether the iterator refers to the same component as another
        /// iterator.
        ///
        /// \param other The other iterator.
        bool operator==(const ConstIterator& other) const;

        ///
        /// Returns whether the iterator refers to a different component than
        /// another iterator.
        ///
        /// \param other The other iterator.
        bool operator!=(const ConstIterator& other) const;

        operator bool() const;

    private:
        ConstIterator(const std::vector<T>* components, size_t index);

        const std::vector<T>* _components;
        size_t _index;
    };

    ComponentPool(Scene* scene);

    ///
    /// Returns the dispatcher of component pool events.
    Dispatcher<ComponentPoolEvent>& dispatcher();

    ///
    /// Adds a component to an entity.
    ///
    /// \remarks If the entity already has an entity of the given type then the
    /// existing entity is overwritten.
    ///
    /// \param entityId The id of the entity.
    /// \param component The component to add.
    void addBase(EntityId entityId, const ComponentBase& component);

    ///
    /// Adds a component to an entity.
    ///
    /// \remarks If the entity already has an entity of the given type then the
    /// existing entity is overwritten.
    ///
    /// \param entityId The id of the entity.
    /// \param component The component to add.
    ///
    /// \returns An iterator to the newly added component.
    Iterator add(EntityId entityId, T component);

    ///
    /// Removes a component from an entity.
    ///
    /// \param entityId The id of the entity.
    ///
    /// \returns True if the component was removed from the entity; false if
    /// entity did not have the component.
    bool remove(EntityId entityId);

    ///
    /// Clones the component from one entity to another.
    ///
    /// \param sourceEntityId The id of the entity to copy the component from.
    /// \param destEntityId The id of the entity to copy the component to.
    void clone(EntityId sourceEntityId, EntityId destEntityId);

    ///
    /// Returns whether an entity has a component in the pool.
    ///
    /// \param entityId The id of the entity.
    bool has(EntityId entityId) const;

    ComponentBase& getBase(EntityId entityId);

    ///
    /// Returns the component of the given entity.
    ///
    /// \param entityId The id of the entity.
    ///
    /// \throws Error If the entity does not have a component in the pool.
    Iterator get(EntityId entityId);

    ///
    /// Returns an iterator to the first component in the pool.
    Iterator begin();

    ///
    /// Returns an iterator to the first component in the pool.
    ConstIterator begin() const;

    ///
    /// Returns an iterator to the end of the pool.
    Iterator end();

    ///
    /// Returns an iterator to the end of the pool.
    ConstIterator end() const;

private:
    Scene* _scene;
    Dispatcher<ComponentPoolEvent> _dispatcher;
    IdPool<ComponentId> _componentIdPool;
    std::vector<T> _components;
    std::vector<ComponentId> _entityIdToComponentId;
};

}

#include "ComponentPool.inl"
