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
    for (Entity& child : children())
    {
        child.destroy();
    }

    _pool->_scene.pendEntityDestruction(*this);
}

void Entity::activate()
{
    ensureInPool();

    // Activate all children first
    for (Entity& child : children())
    {
        child.activate();
    }

    _pool->_scene.pendEntityActivation(*this);
}

bool Entity::isActivated() const
{
    return flag(Flag::Activated);
}

bool Entity::isPendingActivation() const
{
    ensureInPool();
    return flag(Flag::PendingActivation);
}

bool Entity::isPendingDestruction() const
{
    ensureInPool();
    return flag(Flag::PendingDestruction);
}

bool Entity::isTransient() const
{
    ensureInPool();
    return flag(Flag::Transient);
}

void Entity::setTransient(bool transient)
{
    ensureInPool();
    setFlag(Flag::Transient, transient);
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
    for (Entity& child : children())
    {
        child.destroy();
    }
}

Entity::Children& Entity::children()
{
    return *reinterpret_cast<EntityChildren*>(this);
}

const Entity::Children& Entity::children() const
{
    return *reinterpret_cast<const EntityChildren*>(this);
}

bool Entity::hasChildren() const
{
    return !_childIds.empty();
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
    Uncopyable(),
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

void Entity::setFlag(Flag flag, bool value)
{
    _flags[static_cast<size_t>(flag)] = value;
}

bool Entity::flag(Flag flag) const
{
    return _flags[static_cast<size_t>(flag)];
}

void Entity::encode(Encoder& encoder) const
{
    ensureInPool();

    if (!isTransient())
    {
        Scene& scene = _pool->_scene;
        scene.encodeComponents(*this, encoder);

        encoder << beginArray("children");
        for (const Entity& child : children())
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
                throw DecodeError(format("Failed to load base entity '%s': %s", basePath.asString().data(), exception.what()));
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
