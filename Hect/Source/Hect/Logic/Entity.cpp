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
#include "Hect/IO/JsonDecoder.h"

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
    _pool(nullptr)
{
}

Entity::Children::IterBase::IterBase(EntityPool& pool, std::vector<EntityId>::iterator it, std::vector<EntityId>::iterator end) :
    _pool(&pool),
    _it(it),
    _end(end)
{
}

void Entity::Children::IterBase::_increment()
{
    ++_it;
}

bool Entity::Children::IterBase::_isValid() const
{
    return _it != _end;
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
    return _pool == other._pool && _it == other._it;
}

Entity::Children::Iter::Iter() :
    IterBase()
{
}

Entity::Children::Iter::Iter(EntityPool& pool, std::vector<EntityId>::iterator it, std::vector<EntityId>::iterator end) :
    IterBase(pool, it, end)
{
}

Entity& Entity::Children::Iter::operator*() const
{
    _ensureValid();
    return _pool->_entityWithId(*_it);
}

Entity* Entity::Children::Iter::operator->() const
{
    _ensureValid();
    return &_pool->_entityWithId(*_it);
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

Entity::Children::ConstIter::ConstIter(const EntityPool& pool, std::vector<EntityId>::iterator it, std::vector<EntityId>::iterator end) :
    IterBase(*const_cast<EntityPool*>(&pool), it, end)
{
}

const Entity& Entity::Children::ConstIter::operator*() const
{
    _ensureValid();
    return _pool->_entityWithId(*_it);
}

const Entity* Entity::Children::ConstIter::operator->() const
{
    _ensureValid();
    return &_pool->_entityWithId(*_it);
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
    return Entity::Children::Iter(*_entity->_pool, _ids.begin(), _ids.end());
}

Entity::Children::ConstIter Entity::Children::begin() const
{
    return Entity::Children::ConstIter(*_entity->_pool, _ids.begin(), _ids.end());
}

Entity::Children::Iter Entity::Children::end()
{
    return Entity::Children::Iter(*_entity->_pool, _ids.end(), _ids.end());
}

Entity::Children::ConstIter Entity::Children::end() const
{
    return Entity::Children::ConstIter(*_entity->_pool, _ids.end(), _ids.end());
}

Entity::Children::Children(Entity& entity) :
    _entity(&entity)
{
}

Entity::Iter Entity::clone(const std::string& name) const
{
    _ensureInPool();
    return _pool->_scene->_cloneEntity(*this, name);
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

const std::string& Entity::name() const
{
    return _name;
}

void Entity::setName(const std::string& name)
{
    _name = name;
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
    _children._ids.push_back(entity._id);
}

void Entity::removeChild(Entity& entity)
{
    if (entity._pool != _pool || entity._parentId != _id)
    {
        throw Error("Entity is not a child of this entity");
    }

    _children._ids.erase(std::remove(_children._ids.begin(), _children._ids.end(), entity._id), _children._ids.end());
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

void Entity::encode(ObjectEncoder& encoder) const
{
    _ensureInPool();

    Scene& scene = *_pool->_scene;

    if (!_name.empty())
    {
        encoder.encodeString("name", _name);
    }

    scene._encodeComponents(*this, encoder);
}

void Entity::decode(ObjectDecoder& decoder, AssetCache& assetCache)
{
    _ensureInPool();

    Scene& scene = *_pool->_scene;

    if (decoder.hasMember("name"))
    {
        _name = decoder.decodeString("name");
    }

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
}

Entity::Entity() :
    _pool(nullptr),
    _id((EntityId)-1),
    _parentId((EntityId)-1),
    _children(*this),
    _activated(false)
{
}

Entity::Entity(Entity&& entity) :
    _pool(entity._pool),
    _id(entity._id),
    _parentId((EntityId)-1),
    _children(std::move(entity._children)),
    _name(std::move(entity._name)),
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