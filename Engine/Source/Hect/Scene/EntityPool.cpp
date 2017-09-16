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
#include "EntityPool.h"

#include <algorithm>

using namespace hect;

EntityPool::EntityPool(Scene& scene) :
    _scene(scene)
{
    allocate_chunk();
}

Entity::Iterator EntityPool::begin()
{
    Entity::Iterator iterator(*this, 0);

    // Move to the first activated entity
    if (!iterator || !iterator->is_activated())
    {
        ++iterator;
    }
    return iterator;
}

Entity::ConstIterator EntityPool::begin() const
{
    Entity::ConstIterator iterator(*this, 0);

    // Move to the first activated entity
    if (!iterator || !iterator->is_activated())
    {
        ++iterator;
    }
    return iterator;
}

Entity::Iterator EntityPool::end()
{
    return Entity::Iterator(*this, std::max(max_id(), static_cast<EntityId>(1)));
}

Entity::ConstIterator EntityPool::end() const
{
    return Entity::ConstIterator(*this, std::max(max_id(), static_cast<EntityId>(1)));
}

Entity::Handle EntityPool::find_first_by_name(Name name) const
{
    return find_first([name](const Entity& entity) { return entity.name() == name; });
}

Entity& EntityPool::with_id(EntityId id)
{
    const Entity& entity = const_cast<const EntityPool*>(this)->with_id(id);
    return const_cast<Entity&>(entity);
}

const Entity& EntityPool::with_id(EntityId id) const
{
    if (id < max_id())
    {
        const Entity& entity = look_up_entity(id);
        if (entity.in_pool())
        {
            return entity;
        }
    }

    throw InvalidOperation("Invalid entity");
}

Entity& EntityPool::create(Name name)
{
    EntityId id = _id_pool.create();

    // Allocate chunk if needed
    while (id >= max_id())
    {
        allocate_chunk();
    }

    Entity& entity = look_up_entity(id);
    entity.set_name(name);
    entity.enter_pool(*this, id);

    return entity;
}

void EntityPool::destroy(EntityId id)
{
    if (!entity_is_valid(id))
    {
        throw InvalidOperation("Entity does not exist");
    }

    _id_pool.destroy(id);
    look_up_entity(id).exit_pool();
}

bool EntityPool::entity_is_valid(EntityId id)
{
    if (id < max_id())
    {
        if (look_up_entity(id).in_pool())
        {
            return true;
        }
    }
    return false;
}

Entity& EntityPool::entity_with_id(EntityId id)
{
    const Entity& entity = const_cast<const EntityPool*>(this)->entity_with_id(id);
    return const_cast<Entity&>(entity);
}

const Entity& EntityPool::entity_with_id(EntityId id) const
{
    if (id < max_id())
    {
        const Entity& entity = look_up_entity(id);
        if (entity.in_pool())
        {
            return entity;
        }
    }
    throw InvalidOperation("Invalid entity");
}

Entity& EntityPool::look_up_entity(EntityId id)
{
    const size_t chunk_index = id / _entity_chunk_size;
    const size_t entity_index = id % _entity_chunk_size;
    return _entity_chunks[chunk_index][entity_index];
}

const Entity& EntityPool::look_up_entity(EntityId id) const
{
    const size_t chunk_index = id / _entity_chunk_size;
    const size_t entity_index = id % _entity_chunk_size;
    return _entity_chunks[chunk_index][entity_index];
}

EntityId EntityPool::max_id() const
{
    return static_cast<EntityId>(_entity_chunks.size() * _entity_chunk_size);
}

void EntityPool::allocate_chunk()
{
    std::unique_ptr<Entity[]> chunk(new Entity[_entity_chunk_size]);

    // Initialize all of the entities
    for (size_t i = 0; i < _entity_chunk_size; ++i)
    {
        chunk[i] = Entity();
    }

    _entity_chunks.push_back(std::move(chunk));
}

