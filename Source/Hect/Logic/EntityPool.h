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

#include "Hect/Core/IdPool.h"
#include "Hect/Logic/Entity.h"

namespace hect
{

class Scene;

///
/// A pool of entities within a Scene.
class EntityPool
{
    friend class Scene;
    friend class Entity;
    template <typename T> friend class ComponentPool;
public:
    EntityPool(Scene& scene);

    ///
    /// Returns an iterator to the beginning of the pool.
    Entity::Iter begin();

    ///
    /// Returns an iterator to the beginning of the pool.
    Entity::ConstIter begin() const;

    ///
    /// Returns an iterator to the end of the pool.
    Entity::Iter end();

    ///
    /// Returns an iterator to the end of the pool.
    Entity::ConstIter end() const;

    ///
    /// Returns an iterator to the first entity matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An iterator to the first matching entity; invalid if there
    /// was no matching entity.
    Entity::Iter findFirst(Entity::Predicate predicate);

    ///
    /// Returns an iterator to the first entity matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An iterator to the first matching entity; invalid if there
    /// was no matching entity.
    Entity::ConstIter findFirst(Entity::Predicate predicate) const;

    ///
    /// Returns iterators to all entities matching the given predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An array of iterators to the matching entities.
    Entity::Iter::Array find(Entity::Predicate predicate);

    ///
    /// Returns iterators to all entities matching the given predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An array of iterators to the matching entities.
    Entity::ConstIter::Array find(Entity::Predicate predicate) const;

private:
    Entity::Iter _create();
    void _destroy(EntityId id);

    bool _entityIsValid(EntityId id);

    Entity& _entityWithId(EntityId id);
    const Entity& _entityWithId(EntityId id) const;
    
    EntityId _maxId() const;

    Scene* _scene;
    IdPool<EntityId> _idPool;
    std::vector<Entity> _entities;
};

}