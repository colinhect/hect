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
#include "EntityPool.h"

#include <algorithm>

using namespace hect;

EntityPool::EntityPool(Scene& scene) :
    _scene(&scene)
{
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

Entity::Iterator EntityPool::findFirst(Entity::Predicate predicate)
{
    for (auto iterator = begin(); iterator != end(); ++iterator)
    {
        if (predicate(*iterator))
        {
            return iterator;
        }
    }
    return end();
}

Entity::ConstIterator EntityPool::findFirst(Entity::Predicate predicate) const
{
    for (auto iterator = begin(); iterator != end(); ++iterator)
    {
        if (predicate(*iterator))
        {
            return iterator;
        }
    }
    return end();
}

Entity::Iterator::Vector EntityPool::find(Entity::Predicate predicate)
{
    Entity::Iterator::Vector results;
    for (auto iterator = begin(); iterator != end(); ++iterator)
    {
        if (predicate(*iterator))
        {
            results.push_back(iterator);
        }
    }
    return results;
}

Entity::ConstIterator::Vector EntityPool::find(Entity::Predicate predicate) const
{
    Entity::ConstIterator::Vector results;
    for (auto iterator = begin(); iterator != end(); ++iterator)
    {
        if (predicate(*iterator))
        {
            results.push_back(iterator);
        }
    }
    return results;
}

Entity::Iterator EntityPool::create()
{
    EntityId entityId = _idPool.create();

    if (entityId >= _entities.size())
    {
	Entity entity;
        _entities.resize(std::max(_entities.size() * 2, (size_t)8), entity);
    }

    _entities[entityId].enterPool(*this, entityId);
    return Entity::Iterator(*this, entityId);
}

void EntityPool::destroy(EntityId id)
{
    if (!entityIsValid(id))
    {
        throw Error("Entity does not exist");
    }

    _idPool.destroy(id);
    _entities[id].exitPool();
}

bool EntityPool::entityIsValid(EntityId id)
{
    if (id < _entities.size())
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
    if (id < _entities.size())
    {
        const Entity& entity = _entities[id];
        if (entity.inPool())
        {
            return entity;
        }
    }
    throw Error("Invalid entity");
}

EntityId EntityPool::maxId() const
{
    return (EntityId)_entities.size();
}
