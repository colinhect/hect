///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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

#include "Hect/IO/AssetCache.h"
#include "Hect/IO/AssetDecoder.h"
#include "Hect/Logic/EntityPool.h"
#include "Hect/Logic/Scene.h"

using namespace hect;

Scene& Entity::scene()
{
    ensureInPool();
    return _pool->_scene;
}

const Scene& Entity::scene() const
{
    ensureInPool();
    return _pool->_scene;
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
    return _pool->_scene.cloneEntity(*this);
}

void Entity::destroy()
{
    ensureInPool();

    // Destroy all children first
    for (Entity& child : _children)
    {
        child.destroy();
    }

    _pool->_scene.pendEntityDestruction(*this);
}

void Entity::activate()
{
    ensureInPool();

    // Activate all children first
    for (Entity& child : _children)
    {
        child.activate();
    }

    _pool->_scene.pendEntityActivation(*this);
}

bool Entity::isActivated() const
{
    return _flags[Flag_Activated];
}

bool Entity::isPendingActivation() const
{
    ensureInPool();
    return _flags[Flag_PendingActivation];
}

bool Entity::isPendingDestruction() const
{
    ensureInPool();
    return _flags[Flag_PendingDestruction];
}

bool Entity::isTransient() const
{
    ensureInPool();
    return _flags[Flag_Transient];
}

void Entity::setTransient(bool transient)
{
    ensureInPool();
    _flags[Flag_Transient] = transient;
}

EntityId Entity::id() const
{
    return _id;
}

Entity::Iterator Entity::parent()
{
    ensureInPool();
    if (_parentId != EntityId(-1))
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
    if (_parentId != EntityId(-1))
    {
        return Entity::ConstIterator(*_pool, _parentId);
    }
    else
    {
        return const_cast<const EntityPool*>(_pool)->end();
    }
}

Entity::Iterator Entity::root()
{
    auto root = const_cast<const Entity*>(this)->root();
    EntityId id = root->id();
    return Entity::Iterator(*_pool, id);
}

Entity::ConstIterator Entity::root() const
{
    ensureInPool();

    auto root = findFirstAncestor([](const Entity& entity)
    {
        return !entity.parent();
    });

    if (!root)
    {
        root = iterator();
    }

    return root;
}

void Entity::addChild(Entity& entity)
{
    if (entity._parentId != EntityId(-1))
    {
        throw InvalidOperation("Cannot add a child entity which already has a parent");
    }

    if (entity._pool != _pool)
    {
        throw InvalidOperation("Cannot add a child entity from another scene");
    }

    if (isPendingDestruction())
    {
        throw InvalidOperation("Cannot add a child entity to an entity pending destruction");
    }

    if ((isActivated() || isPendingActivation()) && (!entity.isActivated() && !entity.isPendingActivation()))
    {
        throw InvalidOperation("Cannot add unactivated entity as child of activated entity");
    }

    if ((!isActivated() && !isPendingActivation()) && (entity.isActivated() || entity.isPendingActivation()))
    {
        throw InvalidOperation("Cannot add activated entity as child of unactivated entity");
    }

    entity._parentId = _id;
    _childIds.push_back(entity._id);
}

void Entity::removeChild(Entity& entity)
{
    if (entity._pool != _pool || entity._parentId != _id)
    {
        throw InvalidOperation("Entity is not a child of this entity");
    }

    _childIds.erase(std::remove(_childIds.begin(), _childIds.end(), entity._id), _childIds.end());
    entity._parentId = EntityId(-1);
}

void Entity::destroyAllChildren()
{
    for (Entity& child : _children)
    {
        child.destroy();
    }
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

std::vector<Entity::Iterator> Entity::findChildren(Entity::Predicate predicate)
{
    std::vector<Entity::Iterator> results;
    for (Entity& child : _children)
    {
        if (predicate(child))
        {
            results.push_back(child.iterator());
        }
    }
    return results;
}

std::vector<Entity::ConstIterator> Entity::findChildren(Entity::Predicate predicate) const
{
    std::vector<Entity::ConstIterator> results;
    for (const Entity& child : _children)
    {
        if (predicate(child))
        {
            results.push_back(child.iterator());
        }
    }
    return results;
}

std::vector<Entity::Iterator> Entity::findDescendants(Entity::Predicate predicate)
{
    std::vector<Entity::Iterator> results;
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

std::vector<Entity::ConstIterator> Entity::findDescendants(Entity::Predicate predicate) const
{
    std::vector<Entity::ConstIterator> results;
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

std::vector<Entity::Iterator> Entity::findAncestors(Entity::Predicate predicate)
{
    std::vector<Entity::Iterator> results;
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

std::vector<Entity::ConstIterator> Entity::findAncestors(Entity::Predicate predicate) const
{
    std::vector<Entity::ConstIterator> results;
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

Entity& Entity::operator=(const Entity& entity)
{
    _pool = entity._pool;
    _id = entity._id;
    _parentId = entity._parentId;
    _childIds = entity._childIds;
    _flags = entity._flags;
    return *this;
}

Entity& Entity::operator=(Entity&& entity)
{
    _pool = entity._pool;
    _id = entity._id;
    _parentId = entity._parentId;
    _childIds = std::move(entity._childIds);
    _flags = entity._flags;
    return *this;
}

Entity::Entity()
{
}

Entity::Entity(const Entity& entity) :
    _pool(entity._pool),
    _id(entity._id),
    _parentId(entity._parentId),
    _childIds(entity._childIds),
    _flags(entity._flags)
{
}

Entity::Entity(Entity&& entity) :
    _pool(entity._pool),
    _id(entity._id),
    _parentId(entity._parentId),
    _childIds(std::move(entity._childIds)),
    _flags(entity._flags)
{
}

void Entity::enterPool(EntityPool& pool, EntityId id)
{
    _pool = &pool;
    _id = id;
}

void Entity::exitPool()
{
    _pool = nullptr;
    _id = EntityId(-1);
    _flags = std::bitset<4>();
}

bool Entity::inPool() const
{
    return _pool && _id != EntityId(-1);
}

void Entity::ensureInPool() const
{
    if (!inPool())
    {
        throw InvalidOperation("Invalid entity");
    }
}

void Entity::encode(Encoder& encoder) const
{
    ensureInPool();

    if (!isTransient())
    {
        Scene& scene = _pool->_scene;
        scene.encodeComponents(*this, encoder);

        encoder << beginArray("children");
        for (const Entity& child : _children)
        {
            encoder << encodeValue(child);
        }
        encoder << endArray();
    }
}

void Entity::decode(Decoder& decoder)
{
    ensureInPool();

    Scene& scene = _pool->_scene;

    if (!decoder.isBinaryStream())
    {
        if (decoder.selectMember("base"))
        {
            Path basePath;
            decoder >> decodeValue(basePath);

            try
            {
                AssetDecoder baseDecoder(decoder.assetCache(), basePath);
                baseDecoder >> decodeValue(*this);
            }
            catch (const Exception& exception)
            {
                throw DecodeError(format("Failed to load base entity '%s': %s", basePath.asString().c_str(), exception.what()));
            }
        }
    }

    if (decoder.selectMember("components"))
    {
        scene.decodeComponents(*this, decoder);
    }

    if (decoder.selectMember("children"))
    {
        // Use an iterator since the "this" pointer might be invalidated as new
        // entities are created
        Entity::Iterator entity = iterator();

        decoder >> beginArray();
        while (decoder.hasMoreElements())
        {
            Entity::Iterator child = scene.createEntity();

            decoder >> beginObject();
            child->decode(decoder);
            decoder >> endObject();

            entity->addChild(*child);
        }
        decoder >> endArray();
    }
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const Entity& entity)
{
    encoder << beginObject();
    entity.encode(encoder);
    encoder << endObject();
    return encoder;
}

Decoder& operator>>(Decoder& decoder, Entity& entity)
{
    decoder >> beginObject();
    entity.decode(decoder);
    decoder >> endObject();
    return decoder;
}

}