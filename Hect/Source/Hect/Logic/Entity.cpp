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
#include "Entity.h"

#include "Hect/Logic/Scene.h"
#include "Hect/Logic/EntityPool.h"

using namespace hect;

Entity::IterBase::IterBase() :
    _pool(nullptr),
    _id((EntityId)-1)
{
}

Entity::IterBase::IterBase(EntityPool* pool, EntityId id) :
    _pool(pool),
    _id(id)
{
    if (_pool && _id != (EntityId)-1)
    {
        _nextValidEntity();
    }
}

void Entity::IterBase::_nextValidEntity()
{
    size_t maxEntityId = _pool->maxId();
    while (_id < maxEntityId)
    {
        if (_isValid())
        {
            break;
        }

        ++_id;
    }
}

void Entity::IterBase::_increment()
{
    ++_id;
    _nextValidEntity();
}

bool Entity::IterBase::_isValid() const
{
    return _pool->entityIsValid(_id);
}

void Entity::IterBase::_ensureValid() const
{
    if (!_isValid())
    {
        throw Error("Invalid entity iterator");
    }
}

bool Entity::IterBase::_equals(const IterBase& other) const
{
    return _pool == other._pool && _id == other._id;
}

Entity::Iter::Iter() :
    IterBase()
{
}

Entity::Iter::Iter(EntityPool* pool, EntityId id) :
    IterBase(pool, id)
{
}

Entity& Entity::Iter::operator*() const
{
    return _pool->entityWithId(_id);
}

Entity* Entity::Iter::operator->() const
{
    return &_pool->entityWithId(_id);
}

Entity::Iter& Entity::Iter::operator++()
{
    _increment();
}

bool Entity::Iter::operator==(const Iter& other) const
{
    return _equals(other);
}

bool Entity::Iter::operator!=(const Iter& other) const
{
    return !_equals(other);
}

Entity::Iter::operator bool() const
{
    return _isValid();
}

Entity::ConstIter::ConstIter() :
    IterBase()
{
}

Entity::ConstIter::ConstIter(const EntityPool* pool, EntityId id) :
    IterBase(const_cast<EntityPool*>(pool), id)
{
}

const Entity& Entity::ConstIter::operator*() const
{
    return _pool->entityWithId(_id);
}

const Entity* Entity::ConstIter::operator->() const
{
    return &_pool->entityWithId(_id);
}

Entity::ConstIter& Entity::ConstIter::operator++()
{
    _increment();
}

bool Entity::ConstIter::operator==(const ConstIter& other) const
{
    return _equals(other);
}

bool Entity::ConstIter::operator!=(const ConstIter& other) const
{
    return !_equals(other);
}

Entity::ConstIter::operator bool() const
{
    return _isValid();
}

Entity::Entity() :
    _pool(nullptr),
    _id((EntityId)-1),
    _activated(false)
{
}

void Entity::addComponentBase(const ComponentBase& component)
{
    _ensureInPool();
    _pool->scene().addEntityComponentBase(*this, component);
}

Entity::Iter Entity::clone() const
{
    _ensureInPool();
    return _pool->scene().cloneEntity(*this);
}

void Entity::destroy()
{
    _ensureInPool();
    _pool->scene().destroyEntity(*this);
}

void Entity::activate()
{
    _ensureInPool();
    _pool->scene().activateEntity(*this);
}

bool Entity::isActivated() const
{
    return _activated;
}

void Entity::enterPool(EntityPool* pool, EntityId id)
{
    _pool = pool;
    _id = id;
}

void Entity::exitPool()
{
    _pool = nullptr;
    _id = (EntityId)-1;
}

bool Entity::inPool() const
{
    return _pool && _id != (EntityId)-1;
}

EntityId Entity::id() const
{
    return _id;
}

void Entity::encode(ObjectEncoder& encoder) const
{
    _ensureInPool();
    _pool->scene().encodeComponents(*this, encoder);
}

void Entity::decode(ObjectDecoder& decoder, AssetCache& assetCache)
{
    _ensureInPool();
    _pool->scene().decodeComponents(*this, decoder, assetCache);
}

void Entity::_ensureInPool() const
{
    if (!inPool())
    {
        throw Error("Invalid entity");
    }
}