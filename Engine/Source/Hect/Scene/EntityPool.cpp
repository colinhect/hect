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
    _scene(scene),
    _entities(new Entity[64]),
    _entityCount(64)
{
    // Initialize all of the entities
    for (size_t i = 0; i < _entityCount; ++i)
    {
        _entities[i] = Entity();
    }
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
    return Entity::Iterator(*this, std::max(maxId(), (EntityId)1));
}

Entity::ConstIterator EntityPool::end() const
{
    return Entity::ConstIterator(*this, std::max(maxId(), (EntityId)1));
}

Entity::Iterator EntityPool::findFirstByName(Name name)
{
    return findFirst([name](const Entity& entity) { return entity.name() == name; });
}

Entity::ConstIterator EntityPool::findFirstByName(Name name) const
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
    if (id < _entityCount)
    {
        const Entity& entity = _entities[id];
        if (entity.inPool())
        {
            return entity;
        }
    }

    throw InvalidOperation("Invalid entity");
}

Entity::Iterator EntityPool::create(Name name)
{
    EntityId id = _idPool.create();

    // Expand the pool if needed
    if (id >= _entityCount)
    {
        expand();
    }

    Entity& entity = _entities[id];
    entity.setName(name);
    entity.enterPool(*this, id);

    return Entity::Iterator(*this, id);
}

void EntityPool::destroy(EntityId id)
{
    if (!entityIsValid(id))
    {
        throw InvalidOperation("Entity does not exist");
    }

    _idPool.destroy(id);
    _entities[id].exitPool();
}

bool EntityPool::entityIsValid(EntityId id)
{
    if (id < _entityCount)
    {
        if (_entities[id].inPool())
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
    if (id < _entityCount)
    {
        const Entity& entity = _entities[id];
        if (entity.inPool())
        {
            return entity;
        }
    }
    throw InvalidOperation("Invalid entity");
}

EntityId EntityPool::maxId() const
{
    return static_cast<EntityId>(_entityCount);
}

void EntityPool::expand()
{
    size_t newEntityCount = _entityCount * 2;
    std::unique_ptr<Entity[]> newEntities(new Entity[newEntityCount]);

    for (size_t i = 0; i < newEntityCount; ++i)
    {
        if (i < _entityCount)
        {
            // Move the existing entity
            newEntities[i] = std::move(_entities[i]);
        }
        else
        {
            // Initialize new entity
            newEntities[i] = Entity();
        }
    }

    _entities.swap(newEntities);
    _entityCount = newEntityCount;
}
