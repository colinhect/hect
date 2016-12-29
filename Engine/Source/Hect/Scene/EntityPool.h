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

#include <memory>

#include "Hect/Core/EventDispatcher.h"
#include "Hect/Core/Export.h"
#include "Hect/Scene/IdPool.h"
#include "Hect/Scene/Entity.h"
#include "Hect/Scene/EntityEvent.h"

namespace hect
{

class Scene;

///
/// A pool of \link Entity Entities \endlink within a Scene.
class HECT_EXPORT EntityPool :
    public EventDispatcher<EntityEvent>
{
    friend class Scene;

    template <typename T>
    friend class ComponentPool;

    friend class Entity;
    friend class EntityHandle;
    friend class EntityIteratorBase;
    friend class EntityChildIteratorBase;
public:
    EntityPool(Scene& scene);

    ///
    /// Returns an iterator to the beginning of the pool.
    Entity::Iterator begin();

    ///
    /// Returns an iterator to the beginning of the pool.
    Entity::ConstIterator begin() const;

    ///
    /// Returns an iterator to the end of the pool.
    Entity::Iterator end();

    ///
    /// Returns an iterator to the end of the pool.
    Entity::ConstIterator end() const;

    ///
    /// Returns an iterator to the first Entity matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An iterator to the first matching entity; invalid if there
    /// was no matching entity.
    template <typename T>
    Entity::Iterator findFirst(T&& predicate);

    ///
    /// Returns an iterator to the first Entity matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An iterator to the first matching entity; invalid if there
    /// was no matching entity.
    template <typename T>
    Entity::ConstIterator findFirst(T&& predicate) const;

    ///
    /// Returns iterators to all \link Entity Entities \endlink matching the
    /// given predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns A vector of iterators to the matching entities.
    template <typename T>
    std::vector<Entity::Iterator> find(T&& predicate);

    ///
    /// Returns iterators to all \link Entity Entities \endlink matching the
    /// given predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns A vector of iterators to the matching entities.
    template <typename T>
    std::vector<Entity::ConstIterator> find(T&& predicate) const;

    ///
    /// Returns the Entity with the given id.
    ///
    /// \param id The id of the entity to get.
    ///
    /// \throws InvalidOperation If no entity with the given id exists in
    /// the pool.
    Entity& withId(EntityId id);

    ///
    /// \copydoc hect::EntityPool::withId()
    const Entity& withId(EntityId id) const;

private:
    Entity::Iterator create(Name name);
    void destroy(EntityId id);

    bool entityIsValid(EntityId id);

    Entity& entityWithId(EntityId id);
    const Entity& entityWithId(EntityId id) const;

    EntityId maxId() const;
    void expand();

    Scene& _scene;
    IdPool<EntityId> _idPool;

    // Avoiding use of std::vector because we need to keep the Entity
    // constructors private
    std::unique_ptr<Entity[]> _entities;
    size_t _entityCount;
};

}

#include "EntityPool.inl"