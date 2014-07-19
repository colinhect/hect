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

#include "Hect/IO/JsonDecoder.h"
#include "Hect/Logic/EntityPool.h"
#include "Hect/Logic/Scene.h"

using namespace hect;

Entity::IterBase::IterBase() :
    _pool(nullptr),
    _id((EntityId)-1)
{
}

Entity::IterBase::IterBase(EntityPool& pool, EntityId id) :
    _pool(&pool),
    _id(id)
{
}

void Entity::IterBase::_increment()
{
    ++_id;
    size_t maxEntityId = _pool->_maxId();
    while (_id < maxEntityId)
    {
        if (_isValid() && _pool->_entityWithId(_id).isActivated())
        {
            break;
        }

        ++_id;
    }
}

bool Entity::IterBase::_isValid() const
{
    return _pool->_entityIsValid(_id);
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

Entity::Iter::Iter(EntityPool& pool, EntityId id) :
    IterBase(pool, id)
{
}

Entity& Entity::Iter::operator*() const
{
    return _pool->_entityWithId(_id);
}

Entity* Entity::Iter::operator->() const
{
    return &_pool->_entityWithId(_id);
}

Entity::Iter& Entity::Iter::operator++()
{
    _increment();
    return *this;
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

Entity::ConstIter::ConstIter(const EntityPool& pool, EntityId id) :
    IterBase(*const_cast<EntityPool*>(&pool), id)
{
}

const Entity& Entity::ConstIter::operator*() const
{
    return _pool->_entityWithId(_id);
}

const Entity* Entity::ConstIter::operator->() const
{
    return &_pool->_entityWithId(_id);
}

Entity::ConstIter& Entity::ConstIter::operator++()
{
    _increment();
    return *this;
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

Entity::Children::IterBase::IterBase() :
    _pool(nullptr),
    _parentId((EntityId)-1),
    _index(0)
{
}

Entity::Children::IterBase::IterBase(EntityPool& pool, EntityId parentId, size_t index) :
    _pool(&pool),
    _parentId(parentId),
    _index(index)
{
}

void Entity::Children::IterBase::_increment()
{
    if (_isValid())
    {
        ++_index;
    }
}

bool Entity::Children::IterBase::_isValid() const
{
    return _index < _pool->_entityWithId(_parentId)._childIds.size();
}

void Entity::Children::IterBase::_ensureValid() const
{
    if (!_isValid())
    {
        throw Error("Invalid entity iterator");
    }
}

bool Entity::Children::IterBase::_equals(const IterBase& other) const
{
    return _pool == other._pool && _parentId == other._parentId && _index == other._index;
}

Entity::Children::Iter::Iter() :
    IterBase()
{
}

Entity::Children::Iter::Iter(EntityPool& pool, EntityId parentId, size_t index) :
    IterBase(pool, parentId, index)
{
}

Entity& Entity::Children::Iter::operator*() const
{
    _ensureValid();
    Entity& entity = _pool->_entityWithId(_parentId);
    EntityId id = entity._childIds[_index];
    return _pool->_entityWithId(id);
}

Entity* Entity::Children::Iter::operator->() const
{
    _ensureValid();
    Entity& entity = _pool->_entityWithId(_parentId);
    EntityId id = entity._childIds[_index];
    return &_pool->_entityWithId(id);
}

Entity::Children::Iter& Entity::Children::Iter::operator++()
{
    _increment();
    return *this;
}

bool Entity::Children::Iter::operator==(const Iter& other) const
{
    return _equals(other);
}

bool Entity::Children::Iter::operator!=(const Iter& other) const
{
    return !_equals(other);
}

Entity::Children::Iter::operator bool() const
{
    return _isValid();
}

Entity::Children::ConstIter::ConstIter() :
    IterBase()
{
}

Entity::Children::ConstIter::ConstIter(const EntityPool& pool, EntityId parentId, size_t index) :
    IterBase(*const_cast<EntityPool*>(&pool), parentId, index)
{
}

const Entity& Entity::Children::ConstIter::operator*() const
{
    _ensureValid();
    const Entity& entity = _pool->_entityWithId(_parentId);
    EntityId id = entity._childIds[_index];
    return _pool->_entityWithId(id);
}

const Entity* Entity::Children::ConstIter::operator->() const
{
    _ensureValid();
    const Entity& entity = _pool->_entityWithId(_parentId);
    EntityId id = entity._childIds[_index];
    return &_pool->_entityWithId(id);
}

Entity::Children::ConstIter& Entity::Children::ConstIter::operator++()
{
    _increment();
    return *this;
}

bool Entity::Children::ConstIter::operator==(const ConstIter& other) const
{
    return _equals(other);
}

bool Entity::Children::ConstIter::operator!=(const ConstIter& other) const
{
    return !_equals(other);
}

Entity::Children::ConstIter::operator bool() const
{
    return _isValid();
}

Entity::Children::Iter Entity::Children::begin()
{
    Entity* entity = reinterpret_cast<Entity*>(this - offsetof(Entity, _children));
    return Entity::Children::Iter(*entity->_pool, entity->_id, 0);
}

Entity::Children::ConstIter Entity::Children::begin() const
{
    const Entity* entity = reinterpret_cast<const Entity*>(this - offsetof(Entity, _children));
    return Entity::Children::ConstIter(*entity->_pool, entity->_id, 0);
}

Entity::Children::Iter Entity::Children::end()
{
    Entity* entity = reinterpret_cast<Entity*>(this - offsetof(Entity, _children));
    return Entity::Children::Iter(*entity->_pool, entity->_id, entity->_childIds.size());
}

Entity::Children::ConstIter Entity::Children::end() const
{
    const Entity* entity = reinterpret_cast<const Entity*>(this - offsetof(Entity, _children));
    return Entity::Children::ConstIter(*entity->_pool, entity->_id, entity->_childIds.size());
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

const Entity& Entity::Handle::operator*() const
{
    _ensureValid();
    return _context->pool->_entityWithId(_context->id);
}

const Entity* Entity::Handle::operator->() const
{
    _ensureValid();
    return &_context->pool->_entityWithId(_context->id);
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
    return _isValid();
}

Entity::Handle::Handle(EntityPool& pool, EntityId id) :
    _context(std::make_shared<Entity::Handle::Context>(pool, id))
{
}

bool Entity::Handle::_isValid() const
{
    return _context && _context->valid;
}

void Entity::Handle::_ensureValid() const
{
    if (!_isValid())
    {
        throw Error("Invalid entity handle");
    }
}

Entity::Handle::Context::Context(EntityPool& pool, EntityId id) :
    pool(&pool),
    id(id),
    valid(true)
{
    pool.dispatcher().addListener(*this);
}

Entity::Handle::Context::~Context()
{
    pool->dispatcher().removeListener(*this);
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

Entity::Handle Entity::createHandle() const
{
    _ensureInPool();
    return Entity::Handle(*_pool, _id);
}

Entity::Iter Entity::iter()
{
    return Entity::Iter(*_pool, _id);
}

Entity::ConstIter Entity::iter() const
{
    return Entity::ConstIter(*_pool, _id);
}

Entity::Iter Entity::clone() const
{
    _ensureInPool();
    return _pool->_scene->_cloneEntity(*this);
}

void Entity::destroy()
{
    _ensureInPool();
    _pool->_scene->_destroyEntity(*this);
}

void Entity::activate()
{
    _ensureInPool();
    _pool->_scene->_activateEntity(*this);
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

Entity::Iter Entity::parent()
{
    _ensureInPool();
    if (_parentId != (EntityId)-1)
    {
        return Entity::Iter(*_pool, _parentId);
    }
    else
    {
        return _pool->end();
    }
}

Entity::ConstIter Entity::parent() const
{
    _ensureInPool();
    if (_parentId != (EntityId)-1)
    {
        return Entity::ConstIter(*_pool, _parentId);
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

Entity::Iter Entity::findFirstChild(Entity::Predicate predicate)
{
    for (Entity& child : _children)
    {
        if (predicate(child))
        {
            return child.iter();
        }
    }
    return _pool->end();
}

Entity::ConstIter Entity::findFirstChild(Entity::Predicate predicate) const
{
    for (const Entity& child : _children)
    {
        if (predicate(child))
        {
            return child.iter();
        }
    }
    return static_cast<const EntityPool*>(_pool)->end();
}

Entity::Iter Entity::findFirstDescendant(Entity::Predicate predicate)
{
    for (Entity& child : _children)
    {
        if (predicate(child))
        {
            return child.iter();
        }
        else
        {
            Entity::Iter nextGeneration = child.findFirstDescendant(predicate);
            if (nextGeneration)
            {
                return nextGeneration;
            }
        }
    }
    return _pool->end();
}

Entity::ConstIter Entity::findFirstDescendant(Entity::Predicate predicate) const
{
    for (const Entity& child : _children)
    {
        if (predicate(child))
        {
            return Entity::ConstIter(*_pool, child._id);
        }
        else
        {
            Entity::ConstIter nextGeneration = child.findFirstDescendant(predicate);
            if (nextGeneration)
            {
                return nextGeneration;
            }
        }
    }
    return static_cast<const EntityPool*>(_pool)->end();
}

Entity::Iter Entity::findFirstAncestor(Entity::Predicate predicate)
{
    Entity::Iter parentIter = parent();
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

Entity::ConstIter Entity::findFirstAncestor(Entity::Predicate predicate) const
{
    Entity::ConstIter parentIter = parent();
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

Entity::Iter::Vector Entity::findChildren(Entity::Predicate predicate)
{
    Entity::Iter::Vector results;
    for (Entity& child : _children)
    {
        if (predicate(child))
        {
            results.push_back(child.iter());
        }
    }
    return results;
}

Entity::ConstIter::Vector Entity::findChildren(Entity::Predicate predicate) const
{
    Entity::ConstIter::Vector results;
    for (const Entity& child : _children)
    {
        if (predicate(child))
        {
            results.push_back(child.iter());
        }
    }
    return results;
}

Entity::Iter::Vector Entity::findDescendants(Entity::Predicate predicate)
{
    Entity::Iter::Vector results;
    for (Entity& child : _children)
    {
        if (predicate(child))
        {
            results.push_back(child.iter());
        }

        for (Entity::Iter descendant : child.findDescendants(predicate))
        {
            results.push_back(descendant);
        }
    }
    return results;
}

Entity::ConstIter::Vector Entity::findDescendants(Entity::Predicate predicate) const
{
    Entity::ConstIter::Vector results;
    for (const Entity& child : _children)
    {
        if (predicate(child))
        {
            results.push_back(Entity::ConstIter(*_pool, child._id));
        }

        for (Entity::ConstIter descendant : child.findDescendants(predicate))
        {
            results.push_back(descendant);
        }
    }
    return results;
}

Entity::Iter::Vector Entity::findAncestors(Entity::Predicate predicate)
{
    Entity::Iter::Vector results;
    Entity::Iter iter = parent();
    while (iter)
    {
        if (predicate(*iter))
        {
            results.push_back(iter);
        }
        iter = iter->parent();
    }
    return results;
}

Entity::ConstIter::Vector Entity::findAncestors(Entity::Predicate predicate) const
{
    Entity::ConstIter::Vector results;
    Entity::ConstIter iter = parent();
    while (iter)
    {
        if (predicate(*iter))
        {
            results.push_back(iter);
        }
        iter = iter->parent();
    }
    return results;
}

void Entity::encode(ObjectEncoder& encoder) const
{
    _ensureInPool();

    Scene& scene = *_pool->_scene;

    scene._encodeComponents(*this, encoder);

    ArrayEncoder childrenEncoder = encoder.encodeArray("children");
    for (const Entity& child : _children)
    {
        ObjectEncoder childEncoder = childrenEncoder.encodeObject();
        child.encode(childEncoder);
    }
}

void Entity::decode(ObjectDecoder& decoder, AssetCache& assetCache)
{
    _ensureInPool();

    Scene& scene = *_pool->_scene;

    if (decoder.hasMember("archetype"))
    {
        std::string archetype = decoder.decodeString("archetype");
        JsonValue& jsonValue = assetCache.get<JsonValue>(archetype);

        JsonDecoder jsonDecoder(jsonValue);
        ObjectDecoder archetypeDecoder = jsonDecoder.decodeObject();
        scene._decodeComponents(*this, archetypeDecoder, assetCache);
    }

    if (decoder.hasMember("components"))
    {
        scene._decodeComponents(*this, decoder, assetCache);
    }

    if (decoder.hasMember("children"))
    {
        ArrayDecoder childrenDecoder = decoder.decodeArray("children");
        while (childrenDecoder.hasMoreElements())
        {
            Entity::Iter child = _pool->_scene->createEntity();

            ObjectDecoder childDecoder = childrenDecoder.decodeObject();
            child->decode(childDecoder, assetCache);

            addChild(*child);
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

void Entity::_addComponentBase(const ComponentBase& component)
{
    _ensureInPool();
    _pool->_scene->_addEntityComponentBase(*this, component);
}

void Entity::_enterPool(EntityPool& pool, EntityId id)
{
    _pool = &pool;
    _id = id;
}

void Entity::_exitPool()
{
    _pool = nullptr;
    _id = (EntityId)-1;
}

bool Entity::_inPool() const
{
    return _pool && _id != (EntityId)-1;
}

void Entity::_ensureInPool() const
{
    if (!_inPool())
    {
        throw Error("Invalid entity");
    }
}