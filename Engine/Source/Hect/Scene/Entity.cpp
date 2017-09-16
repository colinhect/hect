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
#include "Hect/Scene/EntityPool.h"
#include "Hect/Scene/Scene.h"

using namespace hect;

Name Entity::name() const
{
    return _name;
}

void Entity::set_name(Name name)
{
    _name = name;
}

Scene& Entity::scene()
{
    ensure_in_pool();
    return _pool->_scene;
}

const Scene& Entity::scene() const
{
    ensure_in_pool();
    return _pool->_scene;
}

Entity::Handle Entity::handle() const
{
    if (!in_pool())
    {
        return Entity::Handle();
    }

    if (!_handle)
    {
        _handle = Entity::Handle(*const_cast<Entity*>(reinterpret_cast<const Entity*>(this)));
    }

    return _handle;
}

Entity::Iterator Entity::iterator()
{
    return Entity::Iterator(*_pool, _id);
}

Entity::ConstIterator Entity::iterator() const
{
    return Entity::ConstIterator(*_pool, _id);
}

Entity& Entity::clone() const
{
    ensure_in_pool();
    return _pool->_scene.clone_entity(*this);
}

void Entity::destroy()
{
    ensure_in_pool();

    // Destroy all children first
    for (Entity& child : children())
    {
        child.destroy();
    }

    _pool->_scene.pend_entity_destruction(*this);
}

void Entity::activate()
{
    ensure_in_pool();

    // Activate all children first
    for (Entity& child : children())
    {
        child.activate();
    }

    _pool->_scene.pend_entity_activation(*this);
}

bool Entity::is_activated() const
{
    return flag(Flag::Activated);
}

bool Entity::is_pending_activation() const
{
    ensure_in_pool();
    return flag(Flag::PendingActivation);
}

bool Entity::is_pending_destruction() const
{
    ensure_in_pool();
    return flag(Flag::PendingDestruction);
}

bool Entity::is_transient() const
{
    ensure_in_pool();
    return flag(Flag::Transient);
}

void Entity::set_transient(bool transient)
{
    ensure_in_pool();
    set_flag(Flag::Transient, transient);
}

EntityId Entity::id() const
{
    return _id;
}

Entity::Handle Entity::parent() const
{
    ensure_in_pool();
    if (_parent_id != EntityId(-1))
    {
        return _pool->with_id(_parent_id).handle();
    }
    else
    {
        return Entity::Handle();
    }
}

Entity::Handle Entity::root() const
{
    ensure_in_pool();

    auto root = find_first_ancestor([](const Entity& entity)
    {
        return !entity.parent();
    });

    if (!root)
    {
        root = handle();
    }

    return root;
}

void Entity::add_child(Entity& entity)
{
    if (entity._parent_id != EntityId(-1))
    {
        throw InvalidOperation("Cannot add a child entity which already has a parent");
    }

    if (entity._pool != _pool)
    {
        throw InvalidOperation("Cannot add a child entity from another scene");
    }

    if (is_pending_destruction())
    {
        throw InvalidOperation("Cannot add a child entity to an entity pending destruction");
    }

    if ((is_activated() || is_pending_activation()) && (!entity.is_activated() && !entity.is_pending_activation()))
    {
        throw InvalidOperation("Cannot add unactivated entity as child of activated entity");
    }

    if ((!is_activated() && !is_pending_activation()) && (entity.is_activated() || entity.is_pending_activation()))
    {
        throw InvalidOperation("Cannot add activated entity as child of unactivated entity");
    }

    entity._parent_id = _id;
    _child_ids.push_back(entity._id);
}

void Entity::remove_child(Entity& entity)
{
    if (entity._pool != _pool || entity._parent_id != _id)
    {
        throw InvalidOperation("Entity is not a child of this entity");
    }

    _child_ids.erase(std::remove(_child_ids.begin(), _child_ids.end(), entity._id), _child_ids.end());
    entity._parent_id = EntityId(-1);
}

void Entity::destroy_all_children()
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

bool Entity::has_children() const
{
    return !_child_ids.empty();
}

Entity::Entity() :
    _name(Name::Unnamed)
{
}

Entity::Entity(const Entity& entity) :
    Uncopyable(),
    _pool(entity._pool),
    _id(entity._id),
    _parent_id(entity._parent_id),
    _child_ids(entity._child_ids),
    _name(entity._name),
    _flags(entity._flags)
{
}

Entity::Entity(Entity&& entity) :
    _pool(entity._pool),
    _id(entity._id),
    _parent_id(entity._parent_id),
    _child_ids(std::move(entity._child_ids)),
    _name(entity._name),
    _flags(entity._flags)
{
}

Entity& Entity::operator=(const Entity& entity)
{
    _pool = entity._pool;
    _id = entity._id;
    _parent_id = entity._parent_id;
    _child_ids = entity._child_ids;
    _handle = entity._handle;
    _name = entity._name;
    _flags = entity._flags;
    return *this;
}

Entity& Entity::operator=(Entity&& entity)
{
    _pool = entity._pool;
    _id = entity._id;
    _parent_id = entity._parent_id;
    _child_ids = std::move(entity._child_ids);
    _handle = entity._handle;
    _name = entity._name;
    _flags = entity._flags;
    return *this;
}

void Entity::enter_pool(EntityPool& pool, EntityId id)
{
    _pool = &pool;
    _id = id;
}

void Entity::exit_pool()
{
    _pool = nullptr;
    _id = EntityId(-1);
    _parent_id = EntityId(-1);
    _child_ids.clear();
    _flags = std::bitset<4>();

    if (_handle)
    {
        _handle.invalidate();
    }
    _handle = EntityHandle();
}

bool Entity::in_pool() const
{
    return _pool && _id != EntityId(-1);
}

void Entity::ensure_in_pool() const
{
    if (!in_pool())
    {
        throw InvalidOperation("Invalid entity");
    }
}

void Entity::set_flag(Flag flag, bool value)
{
    _flags[static_cast<size_t>(flag)] = value;
}

bool Entity::flag(Flag flag) const
{
    return _flags[static_cast<size_t>(flag)];
}

Entity::operator bool() const
{
    return in_pool();
}

void Entity::encode(Encoder& encoder) const
{
    ensure_in_pool();

    if (!is_transient())
    {
        const bool has_name = _name != Name::Unnamed;
        if (encoder.is_binary_stream())
        {
            encoder << encode_value(has_name);
        }

        if (has_name)
        {
            encoder << encode_value("name", _name);
        }

        Scene& scene = _pool->_scene;
        scene.encode_components(*this, encoder);

        encoder << begin_array("children");
        for (const Entity& child : children())
        {
            encoder << encode_value(child);
        }
        encoder << end_array();
    }
}

void Entity::decode(Decoder& decoder)
{
    ensure_in_pool();

    Scene& scene = _pool->_scene;

    // Use an iterator since the "this" pointer might be invalidated as new
    // entities are created
    Entity::Iterator entity = iterator();

    if (!decoder.is_binary_stream())
    {
        if (decoder.select_member("base"))
        {
            Path base_path;
            decoder >> decode_value(base_path);

            try
            {
                AssetDecoder base_decoder(decoder.asset_cache(), base_path);
                base_decoder >> decode_value(*entity);
            }
            catch (const Exception& exception)
            {
                throw DecodeError(format("Failed to load base entity '%s': %s", base_path.as_string().data(), exception.what()));
            }
        }
    }

    bool has_name = true;
    if (decoder.is_binary_stream())
    {
        decoder >> decode_value(has_name);
    }

    if (has_name)
    {
        decoder >> decode_value("name", entity->_name);
    }

    if (decoder.select_member("components"))
    {
        scene.decode_components(*this, decoder);
    }

    if (decoder.select_member("children"))
    {
        decoder >> begin_array();
        while (decoder.has_more_elements())
        {
            Entity& child = scene.create_entity();

            decoder >> begin_object();
            child.decode(decoder);
            decoder >> end_object();

            entity->add_child(child);
        }
        decoder >> end_array();
    }
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const Entity& entity)
{
    encoder << begin_object();
    entity.encode(encoder);
    encoder << end_object();
    return encoder;
}

Decoder& operator>>(Decoder& decoder, Entity& entity)
{
    decoder >> begin_object();
    entity.decode(decoder);
    decoder >> end_object();
    return decoder;
}

}
