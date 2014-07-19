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

Dispatcher<EntityEvent>& EntityPool::dispatcher()
{
    return _dispatcher;
}

Entity::Iter EntityPool::begin()
{
    Entity::Iter iter(*this, 0);

    // Move to the first activated entity
    if (!iter || !iter->isActivated())
    {
        ++iter;
    }
    return iter;
}

Entity::ConstIter EntityPool::begin() const
{
    Entity::ConstIter iter(*this, 0);

    // Move to the first activated entity
    if (!iter || !iter->isActivated())
    {
        ++iter;
    }
    return iter;
}

Entity::Iter EntityPool::end()
{
    return Entity::Iter(*this, std::max(_maxId(), (EntityId)1));
}

Entity::ConstIter EntityPool::end() const
{
    return Entity::ConstIter(*this, std::max(_maxId(), (EntityId)1));
}

Entity::Iter EntityPool::findFirst(Entity::Predicate predicate)
{
    for (auto iter = begin(); iter != end(); ++iter)
    {
        if (predicate(*iter))
        {
            return iter;
        }
    }
    return end();
}

Entity::ConstIter EntityPool::findFirst(Entity::Predicate predicate) const
{
    for (auto iter = begin(); iter != end(); ++iter)
    {
        if (predicate(*iter))
        {
            return iter;
        }
    }
    return end();
}

Entity::Iter::Vector EntityPool::find(Entity::Predicate predicate)
{
    Entity::Iter::Vector results;
    for (auto iter = begin(); iter != end(); ++iter)
    {
        if (predicate(*iter))
        {
            results.push_back(iter);
        }
    }
    return results;
}

Entity::ConstIter::Vector EntityPool::find(Entity::Predicate predicate) const
{
    Entity::ConstIter::Vector results;
    for (auto iter = begin(); iter != end(); ++iter)
    {
        if (predicate(*iter))
        {
            results.push_back(iter);
        }
    }
    return results;
}

Entity::Iter EntityPool::_create()
{
    EntityId entityId = _idPool.create();

    if (entityId >= _entities.size())
    {
        _entities.resize(std::max(_entities.size() * 2, (size_t)8));
    }

    _entities[entityId]._enterPool(*this, entityId);
    return Entity::Iter(*this, entityId);
}

void EntityPool::_destroy(EntityId id)
{
    if (!_entityIsValid(id))
    {
        throw Error("Entity does not exist");
    }

    _idPool.destroy(id);
    _entities[id]._exitPool();
}

bool EntityPool::_entityIsValid(EntityId id)
{
    if (id < _entities.size())
    {
        if (_entities[id]._inPool())
        {
            return true;
        }
    }
    return false;
}

Entity& EntityPool::_entityWithId(EntityId id)
{
    const Entity& entity = const_cast<const EntityPool*>(this)->_entityWithId(id);
    return const_cast<Entity&>(entity);
}

const Entity& EntityPool::_entityWithId(EntityId id) const
{
    if (id < _entities.size())
    {
        const Entity& entity = _entities[id];
        if (entity._inPool())
        {
            return entity;
        }
    }
    throw Error("Invalid entity");
}

EntityId EntityPool::_maxId() const
{
    return (EntityId)_entities.size();
}