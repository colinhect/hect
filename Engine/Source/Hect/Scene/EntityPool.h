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
    public EventDispatcher<EntityEvent>,
    public Uncopyable
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
    EntityIterator begin();

    ///
    /// Returns an iterator to the beginning of the pool.
    EntityConstIterator begin() const;

    ///
    /// Returns an iterator to the end of the pool.
    EntityIterator end();

    ///
    /// Returns an iterator to the end of the pool.
    EntityConstIterator end() const;

    ///
    /// Returns a handle to the first Entity matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns A handle to the first matching entity; invalid if there
    /// was no matching entity.
    template <typename T>
    EntityHandle find_first(T&& predicate) const;

    ///
    /// Returns handles to all \link Entity Entities \endlink matching the
    /// given predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns A vector of handles to the matching entities.
    template <typename T>
    std::vector<EntityHandle> find(T&& predicate) const;

    ///
    /// Returns a handle to the first Entity with the given name.
    ///
    /// \param name The name of the entity.
    ///
    /// \returns A handle to the first matching entity; invalid if there
    /// was no matching entity.
    EntityHandle find_first_by_name(Name name) const;

    ///
    /// Returns the Entity with the given id.
    ///
    /// \param id The id of the entity to get.
    ///
    /// \throws InvalidOperation If no entity with the given id exists in
    /// the pool.
    Entity& with_id(EntityId id);

    ///
    /// \copydoc hect::EntityPool::with_id()
    const Entity& with_id(EntityId id) const;

private:
    Entity& create(Name name);
    void destroy(EntityId id);

    bool entity_is_valid(EntityId id);

    Entity& entity_with_id(EntityId id);
    const Entity& entity_with_id(EntityId id) const;

    Entity& look_up_entity(EntityId id);
    const Entity& look_up_entity(EntityId id) const;

    EntityId max_id() const;

    void allocate_chunk();

    Scene& _scene;
    IdPool<EntityId> _id_pool;

    // Avoiding use of std::vector because we need to keep the Entity
    // constructors private
    std::vector<std::unique_ptr<Entity[]>> _entity_chunks;
    size_t _entity_chunk_size { 128 };
};

}

#include "EntityPool.inl"
