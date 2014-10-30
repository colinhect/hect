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

#include <cstddef>

#include "Hect/IO/AssetCache.h"
#include "Hect/IO/AssetDecoder.h"
#include "Hect/Logic/EntityPool.h"
#include "Hect/Logic/Scene.h"

using namespace hect;

Entity::IteratorBase::IteratorBase() :
    _pool(nullptr),
    _id((EntityId)-1)
{
}

Entity::IteratorBase::IteratorBase(EntityPool& pool, EntityId id) :
    _pool(&pool),
    _id(id)
{
}

void Entity::IteratorBase::increment()
{
    ++_id;
    size_t maxEntityId = _pool->maxId();
    while (_id < maxEntityId)
    {
        if (isValid() && _pool->entityWithId(_id).isActivated())
        {
            break;
        }

        ++_id;
    }
}

bool Entity::IteratorBase::isValid() const
{
    return _pool->entityIsValid(_id);
}

void Entity::IteratorBase::ensureValid() const
{
    if (!isValid())
    {
        throw Error("Invalid entity iterator");
    }
}

bool Entity::IteratorBase::equals(const IteratorBase& other) const
{
    return _pool == other._pool && _id == other._id;
}

Entity::Iterator::Iterator() :
    IteratorBase()
{
}

Entity::Iterator::Iterator(EntityPool& pool, EntityId id) :
    IteratorBase(pool, id)
{
}

Entity& Entity::Iterator::operator*() const
{
    return _pool->entityWithId(_id);
}

Entity* Entity::Iterator::operator->() const
{
    return &_pool->entityWithId(_id);
}

Entity::Iterator& Entity::Iterator::operator++()
{
    increment();
    return *this;
}

bool Entity::Iterator::operator==(const Iterator& other) const
{
    return equals(other);
}

bool Entity::Iterator::operator!=(const Iterator& other) const
{
    return !equals(other);
}

Entity::Iterator::operator bool() const
{
    return isValid();
}

Entity::ConstIterator::ConstIterator() :
    IteratorBase()
{
}

Entity::ConstIterator::ConstIterator(const EntityPool& pool, EntityId id) :
    IteratorBase(*const_cast<EntityPool*>(&pool), id)
{
}

const Entity& Entity::ConstIterator::operator*() const
{
    return _pool->entityWithId(_id);
}

const Entity* Entity::ConstIterator::operator->() const
{
    return &_pool->entityWithId(_id);
}

Entity::ConstIterator& Entity::ConstIterator::operator++()
{
    increment();
    return *this;
}

bool Entity::ConstIterator::operator==(const ConstIterator& other) const
{
    return equals(other);
}

bool Entity::ConstIterator::operator!=(const ConstIterator& other) const
{
    return !equals(other);
}

Entity::ConstIterator::operator bool() const
{
    return isValid();
}

Entity::Children::IteratorBase::IteratorBase() :
    _pool(nullptr),
    _parentId((EntityId)-1),
    _index(0)
{
}

Entity::Children::IteratorBase::IteratorBase(EntityPool& pool, EntityId parentId, size_t index) :
    _pool(&pool),
    _parentId(parentId),
    _index(index)
{
}

void Entity::Children::IteratorBase::increment()
{
    if (isValid())
    {
        ++_index;
    }
}

bool Entity::Children::IteratorBase::isValid() const
{
    return _index < _pool->entityWithId(_parentId)._childIds.size();
}

void Entity::Children::IteratorBase::ensureValid() const
{
    if (!isValid())
    {
        throw Error("Invalid entity iterator");
    }
}

bool Entity::Children::IteratorBase::equals(const IteratorBase& other) const
{
    return _pool == other._pool && _parentId == other._parentId && _index == other._index;
}

Entity::Children::Iterator::Iterator() :
    IteratorBase()
{
}

Entity::Children::Iterator::Iterator(EntityPool& pool, EntityId parentId, size_t index) :
    IteratorBase(pool, parentId, index)
{
}

Entity& Entity::Children::Iterator::operator*() const
{
    ensureValid();
    Entity& entity = _pool->entityWithId(_parentId);
    EntityId id = entity._childIds[_index];
    return _pool->entityWithId(id);
}

Entity* Entity::Children::Iterator::operator->() const
{
    ensureValid();
    Entity& entity = _pool->entityWithId(_parentId);
    EntityId id = entity._childIds[_index];
    return &_pool->entityWithId(id);
}

Entity::Children::Iterator& Entity::Children::Iterator::operator++()
{
    increment();
    return *this;
}

bool Entity::Children::Iterator::operator==(const Iterator& other) const
{
    return equals(other);
}

bool Entity::Children::Iterator::operator!=(const Iterator& other) const
{
    return !equals(other);
}

Entity::Children::Iterator::operator bool() const
{
    return isValid();
}

Entity::Children::ConstIterator::ConstIterator() :
    IteratorBase()
{
}

Entity::Children::ConstIterator::ConstIterator(const EntityPool& pool, EntityId parentId, size_t index) :
    IteratorBase(*const_cast<EntityPool*>(&pool), parentId, index)
{
}

const Entity& Entity::Children::ConstIterator::operator*() const
{
    ensureValid();
    const Entity& entity = _pool->entityWithId(_parentId);
    EntityId id = entity._childIds[_index];
    return _pool->entityWithId(id);
}

const Entity* Entity::Children::ConstIterator::operator->() const
{
    ensureValid();
    const Entity& entity = _pool->entityWithId(_parentId);
    EntityId id = entity._childIds[_index];
    return &_pool->entityWithId(id);
}

Entity::Children::ConstIterator& Entity::Children::ConstIterator::operator++()
{
    increment();
    return *this;
}

bool Entity::Children::ConstIterator::operator==(const ConstIterator& other) const
{
    return equals(other);
}

bool Entity::Children::ConstIterator::operator!=(const ConstIterator& other) const
{
    return !equals(other);
}

Entity::Children::ConstIterator::operator bool() const
{
    return isValid();
}

Entity::Children::Iterator Entity::Children::begin()
{
    Entity* entity = reinterpret_cast<Entity*>(this - offsetof(Entity, _children));
    return Entity::Children::Iterator(*entity->_pool, entity->_id, 0);
}

Entity::Children::ConstIterator Entity::Children::begin() const
{
    const Entity* entity = reinterpret_cast<const Entity*>(this - offsetof(Entity, _children));
    return Entity::Children::ConstIterator(*entity->_pool, entity->_id, 0);
}

Entity::Children::Iterator Entity::Children::end()
{
    Entity* entity = reinterpret_cast<Entity*>(this - offsetof(Entity, _children));
    return Entity::Children::Iterator(*entity->_pool, entity->_id, entity->_childIds.size());
}

Entity::Children::ConstIterator Entity::Children::end() const
{
    const Entity* entity = reinterpret_cast<const Entity*>(this - offsetof(Entity, _children));
    return Entity::Children::ConstIterator(*entity->_pool, entity->_id, entity->_childIds.size());
}

Entity::Handle::Handle()
{
}

Entity::Handle::Handle(const Entity::Handle& handle) :
    _context(handle._context)
{
}

Entity::Handle::Handle(Entity::Handle&& handle) :
    _context(std::move(handle._context))
{
}

Entity& Entity::Handle::operator*()
{
    ensureValid();
    return _context->pool->entityWithId(_context->id);
}

const Entity& Entity::Handle::operator*() const
{
    ensureValid();
    return _context->pool->entityWithId(_context->id);
}

Entity* Entity::Handle::operator->()
{
    ensureValid();
    return &_context->pool->entityWithId(_context->id);
}

const Entity* Entity::Handle::operator->() const
{
    ensureValid();
    return &_context->pool->entityWithId(_context->id);
}

bool Entity::Handle::operator==(const Entity::Handle& other) const
{
    return _context == other._context;
}

bool Entity::Handle::operator!=(const Entity::Handle& other) const
{
    return _context != other._context;
}

Entity::Handle::operator bool() const
{
    return isValid();
}

Entity::Handle::Handle(EntityPool& pool, EntityId id) :
    _context(std::make_shared<Entity::Handle::Context>(pool, id))
{
}

bool Entity::Handle::isValid() const
{
    return _context && _context->valid;
}

void Entity::Handle::ensureValid() const
{
    if (!isValid())
    {
        throw Error("Invalid entity handle");
    }
}

Entity::Handle::Context::Context(EntityPool& pool, EntityId id) :
    pool(&pool),
    id(id),
    valid(true)
{
    pool.addListener(*this);
}

Entity::Handle::Context::~Context()
{
    pool->removeListener(*this);
}

void Entity::Handle::Context::receiveEvent(const EntityEvent& event)
{
    if (valid && event.entity().id() == id)
    {
        if (event.type == EntityEventType_Destroy)
        {
            valid = false;
        }
    }
}

Entity::Entity() :
    _pool(nullptr),
    _id((EntityId)-1),
    _parentId((EntityId)-1),
    _activated(false)
{
}

Entity::Entity(const Entity& entity) :
    _pool(entity._pool),
    _id(entity._id),
    _parentId(entity._parentId),
    _childIds(entity._childIds),
    _activated(entity._activated)
{
}

Entity::Entity(Entity&& entity) :
    _pool(entity._pool),
    _id(entity._id),
    _parentId(entity._parentId),
    _childIds(std::move(entity._childIds)),
    _activated(entity._activated)
{
    entity._pool = nullptr;
    entity._id = (EntityId)-1;
    entity._parentId = (EntityId)-1;
    entity._activated = false;
}

Entity::Handle Entity::createHandle() const
{
    if (!inPool())
    {
        return Entity::Handle();
    }
    return Entity::Handle(*_pool, _id);
}

Entity::Iterator Entity::iterator()
{
    return Entity::Iterator(*_pool, _id);
}

Entity::ConstIterator Entity::iterator() const
{
    return Entity::ConstIterator(*_pool, _id);
}

Entity::Iterator Entity::clone() const
{
    ensureInPool();
    return _pool->_scene->cloneEntity(*this);
}

void Entity::destroy()
{
    ensureInPool();
    _pool->_scene->destroyEntity(*this);
}

void Entity::activate()
{
    ensureInPool();
    _pool->_scene->activateEntity(*this);
}

bool Entity::isActivated() const
{
    if (_parentId == (EntityId)-1)
    {
        return _activated;
    }
    else
    {
        return parent()->isActivated();
    }
}

EntityId Entity::id() const
{
    return _id;
}

Entity::Iterator Entity::parent()
{
    ensureInPool();
    if (_parentId != (EntityId)-1)
    {
        return Entity::Iterator(*_pool, _parentId);
    }
    else
    {
        return _pool->end();
    }
}

Entity::ConstIterator Entity::parent() const
{
    ensureInPool();
    if (_parentId != (EntityId)-1)
    {
        return Entity::ConstIterator(*_pool, _parentId);
    }
    else
    {
        return const_cast<const EntityPool*>(_pool)->end();
    }
}

void Entity::addChild(Entity& entity)
{
    if (entity._parentId != (EntityId)-1)
    {
        throw Error("Cannot add a child entity which already has a parent");
    }

    if (entity._pool != _pool)
    {
        throw Error("Cannot add a child entity from another scene");
    }

    if (_activated && !entity._activated)
    {
        throw Error("Cannot add unactivated entity as child of activated entity");
    }

    if (!_activated && entity._activated)
    {
        throw Error("Cannot add activated entity as child of unactivated entity");
    }

    entity._parentId = _id;
    _childIds.push_back(entity._id);
}

void Entity::removeChild(Entity& entity)
{
    if (entity._pool != _pool || entity._parentId != _id)
    {
        throw Error("Entity is not a child of this entity");
    }

    _childIds.erase(std::remove(_childIds.begin(), _childIds.end(), entity._id), _childIds.end());
    entity._parentId = (EntityId)-1;
}

Entity::Children& Entity::children()
{
    return _children;
}

const Entity::Children& Entity::children() const
{
    return _children;
}

Entity::Iterator Entity::findFirstChild(Entity::Predicate predicate)
{
    for (Entity& child : _children)
    {
        if (predicate(child))
        {
            return child.iterator();
        }
    }
    return _pool->end();
}

Entity::ConstIterator Entity::findFirstChild(Entity::Predicate predicate) const
{
    for (const Entity& child : _children)
    {
        if (predicate(child))
        {
            return child.iterator();
        }
    }
    return static_cast<const EntityPool*>(_pool)->end();
}

Entity::Iterator Entity::findFirstDescendant(Entity::Predicate predicate)
{
    for (Entity& child : _children)
    {
        if (predicate(child))
        {
            return child.iterator();
        }
        else
        {
            Entity::Iterator nextGeneration = child.findFirstDescendant(predicate);
            if (nextGeneration)
            {
                return nextGeneration;
            }
        }
    }
    return _pool->end();
}

Entity::ConstIterator Entity::findFirstDescendant(Entity::Predicate predicate) const
{
    for (const Entity& child : _children)
    {
        if (predicate(child))
        {
            return Entity::ConstIterator(*_pool, child._id);
        }
        else
        {
            Entity::ConstIterator nextGeneration = child.findFirstDescendant(predicate);
            if (nextGeneration)
            {
                return nextGeneration;
            }
        }
    }
    return static_cast<const EntityPool*>(_pool)->end();
}

Entity::Iterator Entity::findFirstAncestor(Entity::Predicate predicate)
{
    Entity::Iterator parentIter = parent();
    if (parentIter)
    {
        if (predicate(*parentIter))
        {
            return parentIter;
        }
        else
        {
            return parentIter->findFirstAncestor(predicate);
        }
    }
    return _pool->end();
}

Entity::ConstIterator Entity::findFirstAncestor(Entity::Predicate predicate) const
{
    Entity::ConstIterator parentIter = parent();
    if (parentIter)
    {
        if (predicate(*parentIter))
        {
            return parentIter;
        }
        else
        {
            return parentIter->findFirstAncestor(predicate);
        }
    }
    return static_cast<const EntityPool*>(_pool)->end();
}

Entity::Iterator::Vector Entity::findChildren(Entity::Predicate predicate)
{
    Entity::Iterator::Vector results;
    for (Entity& child : _children)
    {
        if (predicate(child))
        {
            results.push_back(child.iterator());
        }
    }
    return results;
}

Entity::ConstIterator::Vector Entity::findChildren(Entity::Predicate predicate) const
{
    Entity::ConstIterator::Vector results;
    for (const Entity& child : _children)
    {
        if (predicate(child))
        {
            results.push_back(child.iterator());
        }
    }
    return results;
}

Entity::Iterator::Vector Entity::findDescendants(Entity::Predicate predicate)
{
    Entity::Iterator::Vector results;
    for (Entity& child : _children)
    {
        if (predicate(child))
        {
            results.push_back(child.iterator());
        }

        for (Entity::Iterator descendant : child.findDescendants(predicate))
        {
            results.push_back(descendant);
        }
    }
    return results;
}

Entity::ConstIterator::Vector Entity::findDescendants(Entity::Predicate predicate) const
{
    Entity::ConstIterator::Vector results;
    for (const Entity& child : _children)
    {
        if (predicate(child))
        {
            results.push_back(Entity::ConstIterator(*_pool, child._id));
        }

        for (Entity::ConstIterator descendant : child.findDescendants(predicate))
        {
            results.push_back(descendant);
        }
    }
    return results;
}

Entity::Iterator::Vector Entity::findAncestors(Entity::Predicate predicate)
{
    Entity::Iterator::Vector results;
    Entity::Iterator iterator = parent();
    while (iterator)
    {
        if (predicate(*iterator))
        {
            results.push_back(iterator);
        }
        iterator = iterator->parent();
    }
    return results;
}

Entity::ConstIterator::Vector Entity::findAncestors(Entity::Predicate predicate) const
{
    Entity::ConstIterator::Vector results;
    Entity::ConstIterator iterator = parent();
    while (iterator)
    {
        if (predicate(*iterator))
        {
            results.push_back(iterator);
        }
        iterator = iterator->parent();
    }
    return results;
}

void Entity::enterPool(EntityPool& pool, EntityId id)
{
    _pool = &pool;
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

void Entity::ensureInPool() const
{
    if (!inPool())
    {
        throw Error("Invalid entity");
    }
}

void Entity::encode(Encoder& encoder) const
{
    ensureInPool();

    Scene& scene = *_pool->_scene;

    encoder << beginObject();

    scene.encodeComponents(*this, encoder);

    encoder << beginArray("children");

    for (const Entity& child : _children)
    {
        encoder << encodeValue(child);
    }

    encoder << endArray() << endObject();
}

void Entity::decode(Decoder& decoder)
{
    ensureInPool();

    Scene& scene = *_pool->_scene;

    decoder >> beginObject();

    if (!decoder.isBinaryStream())
    {
        if (decoder.selectMember("base"))
        {
            Path basePath;
            decoder >> decodeValue(basePath);

            AssetDecoder baseDecoder(decoder.assetCache(), basePath);
            baseDecoder >> decodeValue(*this);
        }
    }

    if (decoder.selectMember("components"))
    {
        scene.decodeComponents(*this, decoder);
    }

    if (decoder.selectMember("children"))
    {
        decoder >> beginArray();
        while (decoder.hasMoreElements())
        {
            Entity::Iterator child = _pool->_scene->createEntity();

            decoder >> beginObject();
            child->decode(decoder);
            decoder >> endObject();

            addChild(*child);
        }
        decoder >> endArray();
    }

    decoder >> endObject();
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const Entity& entity)
{
    entity.encode(encoder);
    return encoder;
}

Decoder& operator>>(Decoder& decoder, Entity& entity)
{
    entity.decode(decoder);
    return decoder;
}

}
