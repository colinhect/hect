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
    allocateChunk();
}

Entity::Iterator EntityPool::begin()
{
    Entity::Iterator iterator(*this, 0);

    // Move to the first activated entity
    if (!iterator || !iterator->isActivated())
    {
        ++iterator;
    }
    return iterator;
}

Entity::ConstIterator EntityPool::begin() const
{
    Entity::ConstIterator iterator(*this, 0);

    // Move to the first activated entity
    if (!iterator || !iterator->isActivated())
    {
        ++iterator;
    }
    return iterator;
}

Entity::Iterator EntityPool::end()
{
    return Entity::Iterator(*this, std::max(maxId(), static_cast<EntityId>(1)));
}

Entity::ConstIterator EntityPool::end() const
{
    return Entity::ConstIterator(*this, std::max(maxId(), static_cast<EntityId>(1)));
}

Entity::Handle EntityPool::findFirstByName(Name name) const
{
    return findFirst([name](const Entity& entity) { return entity.name() == name; });
}

Entity& EntityPool::withId(EntityId id)
{
    const Entity& entity = const_cast<const EntityPool*>(this)->withId(id);
    return const_cast<Entity&>(entity);
}

const Entity& EntityPool::withId(EntityId id) const
{
    if (id < maxId())
    {
        const Entity& entity = lookUpEntity(id);
        if (entity.inPool())
        {
            return entity;
        }
    }

    throw InvalidOperation("Invalid entity");
}

Entity& EntityPool::create(Name name)
{
    EntityId id = _idPool.create();

    // Allocate chunk if needed
    while (id >= maxId())
    {
        allocateChunk();
    }

    Entity& entity = lookUpEntity(id);
    entity.setName(name);
    entity.enterPool(*this, id);

    return entity;
}

void EntityPool::destroy(EntityId id)
{
    if (!entityIsValid(id))
    {
        throw InvalidOperation("Entity does not exist");
    }

    _idPool.destroy(id);
    lookUpEntity(id).exitPool();
}

bool EntityPool::entityIsValid(EntityId id)
{
    if (id < maxId())
    {
        if (lookUpEntity(id).inPool())
        {
            return true;
        }
    }
    return false;
}

Entity& EntityPool::entityWithId(EntityId id)
{
    const Entity& entity = const_cast<const EntityPool*>(this)->entityWithId(id);
    return const_cast<Entity&>(entity);
}

const Entity& EntityPool::entityWithId(EntityId id) const
{
    if (id < maxId())
    {
        const Entity& entity = lookUpEntity(id);
        if (entity.inPool())
        {
            return entity;
        }
    }
    throw InvalidOperation("Invalid entity");
}

Entity& EntityPool::lookUpEntity(EntityId id)
{
    const size_t chunkIndex = id / _entityChunkSize;
    const size_t entityIndex = id % _entityChunkSize;
    return _entityChunks[chunkIndex][entityIndex];
}

const Entity& EntityPool::lookUpEntity(EntityId id) const
{
    const size_t chunkIndex = id / _entityChunkSize;
    const size_t entityIndex = id % _entityChunkSize;
    return _entityChunks[chunkIndex][entityIndex];
}

EntityId EntityPool::maxId() const
{
    return static_cast<EntityId>(_entityChunks.size() * _entityChunkSize);
}

void EntityPool::allocateChunk()
{
    std::unique_ptr<Entity[]> chunk(new Entity[_entityChunkSize]);

    // Initialize all of the entities
    for (size_t i = 0; i < _entityChunkSize; ++i)
    {
        chunk[i] = Entity();
    }

    _entityChunks.push_back(std::move(chunk));
}

