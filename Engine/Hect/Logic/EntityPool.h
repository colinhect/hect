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

#include <vector>

#include "Hect/Event/Dispatcher.h"
#include "Hect/Logic/IdPool.h"
#include "Hect/Logic/Entity.h"
#include "Hect/Logic/EntityEvent.h"

namespace hect
{

class Scene;

///
/// A pool of entities within a Scene.
class EntityPool :
    public Dispatcher<EntityEvent>
{
    friend class Scene;
    friend class Entity;
    template <typename T> friend class ComponentPool;
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
    /// Returns an iterator to the first entity matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An iterator to the first matching entity; invalid if there
    /// was no matching entity.
    Entity::Iterator findFirst(Entity::Predicate predicate);

    ///
    /// Returns an iterator to the first entity matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An iterator to the first matching entity; invalid if there
    /// was no matching entity.
    Entity::ConstIterator findFirst(Entity::Predicate predicate) const;

    ///
    /// Returns iterators to all entities matching the given predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns A vector of iterators to the matching entities.
    Entity::Iterator::Vector find(Entity::Predicate predicate);

    ///
    /// Returns iterators to all entities matching the given predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns A vector of iterators to the matching entities.
    Entity::ConstIterator::Vector find(Entity::Predicate predicate) const;

private:
    Entity::Iterator create();
    void destroy(EntityId id);

    bool entityIsValid(EntityId id);

    Entity& entityWithId(EntityId id);
    const Entity& entityWithId(EntityId id) const;

    EntityId maxId() const;

    Scene* _scene;
    IdPool<EntityId> _idPool;
    std::vector<Entity> _entities;
};

}