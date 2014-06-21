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

using namespace hect;

#include <algorithm>

EntityPool::EntityPool(Scene& scene) :
    _scene(&scene)
{
}

Entity::Iter EntityPool::create()
{
    EntityId entityId = _idPool.create();

    if (entityId >= _entities.size())
    {
        _entities.resize(std::max(_entities.size() * 2, (size_t)8));
    }

    _entities[entityId].enterPool(this, entityId);
    return Entity::Iter(this, entityId);
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
    if (id < _entities.size())
    {
        Entity& entity = _entities[id];
        if (entity.inPool())
        {
            return entity;
        }
    }
    throw Error("Invalid entity");
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

Scene& EntityPool::scene()
{
    return *_scene;
}

const Scene& EntityPool::scene() const
{
    return *_scene;
}

EntityId EntityPool::maxId() const
{
    return (EntityId)_entities.size();
}

Entity::Iter EntityPool::begin()
{
    return Entity::Iter(this, 0);
}

Entity::ConstIter EntityPool::begin() const
{
    return Entity::ConstIter(this, 0);
}

Entity::Iter EntityPool::end()
{
    return Entity::Iter(this, maxId());
}

Entity::ConstIter EntityPool::end() const
{
    return Entity::ConstIter(this, maxId());
}