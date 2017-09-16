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
#include "Scene.h"

#include "Hect/IO/AssetDecoder.h"
#include "Hect/Scene/SceneRegistry.h"
#include "Hect/Runtime/Engine.h"

using namespace hect;

Scene::Scene(Engine& engine) :
    _engine(&engine),
    _entity_pool(*this)
{
    // Register to entity events if trace-level logging is enabled
    if (is_log_level_enabled(LogLevel::Trace))
    {
        _entity_pool.register_listener(*this);
    }

    for (ComponentTypeId type_id : ComponentRegistry::type_ids())
    {
        add_component_type(type_id);
    }
}

Scene::~Scene()
{
}

void Scene::add_system(SystemBase& system)
{
    _systems.push_back(&system);
}

bool Scene::active() const
{
    return _active;
}

void Scene::set_active(bool active)
{
    _active = active;
}

void Scene::refresh()
{
    // Create/activate/destroy all pending entities and dispatch related
    while (has_pending_entities())
    {
        dispatch_entity_creation_events();
        activate_pending_entities();
        destroy_pending_entities();
    }
}

bool Scene::is_initialized() const
{
    return _initialized;
}

void Scene::initialize()
{
    _initialized = true;
}

Entity& Scene::create_entity(Name name)
{
    Entity& entity = _entity_pool.create(name);
    _entities_pending_creation.push_back(entity.id());
    return entity;
}

Entity& Scene::load_entity(const Path& path)
{
    Timer timer;

    Entity& entity = create_entity();

    // Resolve the path to the entity
    AssetCache& asset_cache = _engine->asset_cache();
    const Path resolved_path = asset_cache.resolve_path(path);

    AssetDecoder decoder(_engine->asset_cache(), resolved_path);
    decoder >> decode_value(entity);

    HECT_INFO(format("Loaded entity '%s' in %ims", resolved_path.as_string().data(), Milliseconds(timer.elapsed()).value));

    return entity;
}

void Scene::destroy_all_entities()
{
    for (Entity& entity : entities())
    {
        destroy_entity(entity);
    }
}

EntityPool& Scene::entities()
{
    return _entity_pool;
}

const EntityPool& Scene::entities() const
{
    return _entity_pool;
}

size_t Scene::entity_count() const
{
    return _entity_count;
}

void Scene::load(const Path& path)
{
    if (_initialized)
    {
        throw InvalidOperation("Scene is already initialized and cannot be loaded");
    }

    Timer timer;

    // Resolve the path to the scene
    AssetCache& asset_cache = _engine->asset_cache();
    const Path resolved_path = asset_cache.resolve_path(path);

    // Set the name of the scene to the resolved path
    set_name(resolved_path.as_string());

    // Decode the scene from the asset
    AssetDecoder decoder(_engine->asset_cache(), resolved_path);
    decoder >> decode_value(*this);

    // Refresh and initialize the scene
    refresh();
    initialize();
    refresh();

    HECT_INFO(format("Loaded scene '%s' in %ims", resolved_path.as_string().data(), Milliseconds(timer.elapsed()).value));
}

void Scene::encode(Encoder& encoder) const
{
    const std::type_index type_index(typeid(*this));
    const SceneTypeId scene_type_id = SceneRegistry::type_id_of(type_index);
    if (encoder.is_binary_stream())
    {
        encoder << encode_value(scene_type_id);
    }
    else
    {
        const Name type_name = SceneRegistry::type_name_of(scene_type_id);
        encoder << encode_value("scene_type", type_name.as_string());
    }

    // Systems
    encoder << begin_array("systems");
    for (auto& system : _systems)
    {
        encoder << begin_object();

        if (!encoder.is_binary_stream())
        {
            const Name type_name = Type::of(*system).name();
            encoder << encode_value("system_type", type_name);
        }

        system->encode(encoder);

        encoder << end_object();
    }
    encoder << end_array();

    // Entities
    encoder << begin_array("entities");
    for (const Entity& entity : entities())
    {
        // Only encode the root entities (children are encoded recursively)
        if (!entity.parent())
        {
            encoder << encode_value(entity);
        }
    }
    encoder << end_array();
}

void Scene::decode(Decoder& decoder)
{
    // Base
    if (!decoder.is_binary_stream())
    {
        if (decoder.select_member("base"))
        {
            Path base_path;
            decoder >> decode_value(base_path);

            try
            {
                AssetDecoder base_decoder(decoder.asset_cache(), base_path);
                base_decoder >> begin_object() >> decode_value(*this) >> end_object();
            }
            catch (const Exception& exception)
            {
                throw DecodeError(format("Failed to load base scene '%s': %s", base_path.as_string().data(), exception.what()));
            }
        }
    }

    // Decode the scene type; nothing is done with the resulting value since
    // the scene has already been constructed (this is only done to make
    // encoding and decoding reflective)
    if (decoder.is_binary_stream())
    {
        SceneTypeId type_id;
        decoder >> decode_value(type_id);
    }
    else
    {
        std::string type_name;
        decoder >> decode_value("scene_type", type_name);
    }

    // Systems
    if (decoder.select_member("systems"))
    {
        decoder >> begin_array();

        if (decoder.is_binary_stream())
        {
            size_t system_index = 0;
            while (decoder.has_more_elements())
            {
                decoder >> begin_object();
                _systems[system_index++]->decode(decoder);
                decoder >> end_object();
            }
        }
        else
        {
            while (decoder.has_more_elements())
            {
                decoder >> begin_object();

                std::string system_type;
                decoder >> decode_value("system_type", system_type);

                // Find the system of the specified type
                auto it = std::find_if(_systems.begin(), _systems.end(), [&system_type](const SystemBase* system)
                    {
                        const Name type_name = Type::of(*system).name();
                        return type_name.as_string() == system_type;
                    });

                // If the system was found
                if (it != _systems.end())
                {
                    SystemBase& system = **it;
                    system.decode(decoder);
                }
                else
                {
                    throw DecodeError(format("Scene does not have system of type '%s'", system_type.data()));
                }

                decoder >> end_object();
            }
        }

        decoder >> end_array();
    }

    // Entities
    if (decoder.select_member("entities"))
    {
        decoder >> begin_array();
        while (decoder.has_more_elements())
        {
            Entity& entity = create_entity();
            decoder >> decode_value(entity);
            entity.activate();
        }
        decoder >> end_array();
    }

    refresh();
}

Engine& Scene::engine() const
{
    assert(_engine);
    return *_engine;
}

void Scene::add_component_type(ComponentTypeId type_id)
{
    // Make sure the component type isn't already added
    if (type_id < _component_pools.size() && _component_pools[type_id])
    {
        Name type_name = ComponentRegistry::type_name_of(type_id);
        throw InvalidOperation(format("Scene already supports component type '%s'", type_name.data()));
    }

    // Make sure the type id is a real type id
    if (!ComponentRegistry::is_registered_type_id(type_id))
    {
        throw InvalidOperation("Unknown component type id");
    }

    // Resize the component pool vector if needed
    while (type_id >= _component_pools.size())
    {
        size_t old_size = _component_pools.size();
        _component_pools.resize(std::max(old_size * 2, size_t(8)));
    }

    // Add the component pool
    auto component_pool = ComponentRegistry::create_pool(type_id, *this);
    _component_pools[type_id] = component_pool;
    _component_type_ids.push_back(type_id);
}

ComponentPoolBase& Scene::component_pool_of_type_id(ComponentTypeId type_id)
{
    if (!ComponentRegistry::is_registered_type_id(type_id))
    {
        throw InvalidOperation("Unknown component type id");
    }
    else if (type_id >= _component_pools.size() || !_component_pools[type_id])
    {
        Name type_name = ComponentRegistry::type_name_of(type_id);
        throw InvalidOperation(format("Scene does not support component type '%s'", type_name.data()));
    }
    else
    {
        return *_component_pools[type_id];
    }
}

Entity& Scene::clone_entity(const Entity& entity)
{
    Entity& cloned_entity = create_entity(entity.name());

    for (ComponentTypeId type_id : _component_type_ids)
    {
        ComponentPoolBase& component_pool = component_pool_of_type_id(type_id);
        component_pool.clone(entity, cloned_entity);
    }

    // Recursively clone all children
    for (const Entity& child : entity.children())
    {
        Entity& cloned_child = child.clone();
        cloned_entity.add_child(cloned_child);
    }

    return cloned_entity;
}

void Scene::destroy_entity(Entity& entity)
{
    if (!entity.in_pool())
    {
        throw InvalidOperation("Invalid entity");
    }

    // Dispatch the entity destroy event
    EntityEvent event;
    event.type = EntityEventType::Destroy;
    event.entity = entity.handle();
    _entity_pool.dispatch_event(event);

    // Destroy all children
    std::vector<EntityId> child_ids;
    for (Entity& child : entity.children())
    {
        child_ids.push_back(child._id);
    }
    for (EntityId child_id : child_ids)
    {
        _entity_pool.entity_with_id(child_id).destroy();
    }

    // Remove all components
    for (ComponentTypeId type_id : _component_type_ids)
    {
        ComponentPoolBase& component_pool = *_component_pools[type_id];
        if (component_pool.has(entity))
        {
            component_pool.remove(entity);
        }
    }

    if (entity.is_activated())
    {
        --_entity_count;
    }

    // If the entity had a parent then remove itself as a child
    Entity::Handle parent = entity.parent();
    if (parent)
    {
        parent->remove_child(entity);
    }

    _entity_pool.destroy(entity._id);
}

void Scene::activate_entity(Entity& entity)
{
    if (!entity.in_pool())
    {
        throw InvalidOperation("Invalid entity");
    }

    if (entity.is_activated())
    {
        throw InvalidOperation("Entity is already activated");
    }

    ++_entity_count;
    entity.set_flag(Entity::Flag::Activated, true);
    entity.set_flag(Entity::Flag::PendingActivation, false);

    for (ComponentTypeId type_id : _component_type_ids)
    {
        ComponentPoolBase& component_pool = *_component_pools[type_id];
        if (component_pool.has(entity))
        {
            component_pool.dispatch_event(ComponentEventType::Add, entity);
        }
    }

    // Dispatch the entity activate event
    EntityEvent event;
    event.type = EntityEventType::Activate;
    event.entity = entity.handle();
    _entity_pool.dispatch_event(event);
}

void Scene::pend_entity_destruction(Entity& entity)
{
    if (!entity.in_pool())
    {
        throw InvalidOperation("Invalid entity");
    }

    if (entity.is_pending_destruction())
    {
        throw InvalidOperation("Entity is already pending destruction");
    }

    entity.set_flag(Entity::Flag::PendingDestruction, true);
    _entities_pending_destruction.push_back(entity._id);
}

void Scene::pend_entity_activation(Entity& entity)
{
    if (!entity.in_pool())
    {
        throw InvalidOperation("Invalid entity");
    }

    if (entity.is_pending_activation())
    {
        throw InvalidOperation("Entity is already pending activation");
    }
    else if (entity.is_activated())
    {
        throw InvalidOperation("Entity is already activated");
    }

    entity.set_flag(Entity::Flag::PendingActivation, true);

    // Enqueue the entity to activate on the next refresh
    _entities_pending_activation.push_back(entity._id);
}

bool Scene::has_pending_entities() const
{
    return !_entities_pending_creation.empty() || !_entities_pending_activation.empty() || !_entities_pending_destruction.empty();
}

void Scene::dispatch_entity_creation_events()
{
    // Dispatch the entity creation event for all entities pending creation
    while (!_entities_pending_creation.empty())
    {
        const EntityId entity_id = _entities_pending_creation.front();
        _entities_pending_creation.pop_front();

        // Dispatch an entity create event
        EntityEvent event;
        event.type = EntityEventType::Create;
        event.entity = _entity_pool.with_id(entity_id).handle();
        _entity_pool.dispatch_event(event);
    }
}

void Scene::activate_pending_entities()
{
    // Activate all entities pending activation
    while (!_entities_pending_activation.empty())
    {
        const EntityId entity_id = _entities_pending_activation.front();
        _entities_pending_activation.pop_front();

        Entity& entity = _entity_pool.entity_with_id(entity_id);
        activate_entity(entity);
    }
}

void Scene::destroy_pending_entities()
{
    // Destroy all entities set pending destruction
    while (!_entities_pending_destruction.empty())
    {
        const EntityId entity_id = _entities_pending_destruction.front();
        _entities_pending_destruction.pop_front();

        Entity& entity = _entity_pool.entity_with_id(entity_id);
        destroy_entity(entity);
    }
}

void Scene::add_entity_component_base(Entity& entity, const ComponentBase& component)
{
    if (!entity.in_pool())
    {
        throw InvalidOperation("Invalid entity");
    }

    ComponentTypeId type_id = component.type_id();
    ComponentPoolBase& component_pool = component_pool_of_type_id(type_id);
    component_pool.add_base(entity, component);
}

void Scene::encode_components(const Entity& entity, Encoder& encoder)
{
    if (encoder.is_binary_stream())
    {
        WriteStream& stream = encoder.binary_stream();

        size_t component_count_position = stream.position();
        uint8_t component_count = 0;
        stream << component_count;

        for (ComponentTypeId type_id : _component_type_ids)
        {
            ComponentPoolBase& component_pool = *_component_pools[type_id];
            if (component_pool.has(entity))
            {
                ++component_count;

                const ComponentBase& component = component_pool.get_base(entity);
                stream << component.type_id();
                component.encode(encoder);
            }
        }

        size_t current_position = stream.position();
        stream.seek(component_count_position);
        stream << component_count;
        stream.seek(current_position);
    }
    else
    {
        encoder << begin_array("components");

        for (ComponentTypeId type_id : _component_type_ids)
        {
            ComponentPoolBase& component_pool = *_component_pools[type_id];
            if (component_pool.has(entity))
            {
                encoder << begin_object();

                const ComponentBase& component = component_pool.get_base(entity);
                Name type_name = Type::of(component).name();

                encoder << encode_value("component_type", type_name);
                component.encode(encoder);

                encoder << end_object();
            }
        }

        encoder << end_array();
    }
}

void Scene::decode_components(Entity& entity, Decoder& decoder)
{
    if (decoder.is_binary_stream())
    {
        ReadStream& stream = decoder.binary_stream();
        uint8_t component_count;
        stream >> component_count;
        for (uint8_t i = 0; i < component_count; ++i)
        {
            ComponentTypeId type_id;
            stream >> type_id;
            std::shared_ptr<ComponentBase> component = ComponentRegistry::create(type_id);
            component->decode(decoder);
            add_entity_component_base(entity, *component);
        }
    }
    else
    {
        if (decoder.select_member("components"))
        {
            decoder >> begin_array();
            while (decoder.has_more_elements())
            {
                decoder >> begin_object();

                std::string type_name;
                decoder >> decode_value("component_type", type_name);

                ComponentTypeId type_id = ComponentRegistry::type_id_of(type_name);

                // If the entity already has a component of this type
                ComponentPoolBase& component_pool = component_pool_of_type_id(type_id);
                if (component_pool.has(entity))
                {
                    // Re-decode the existing component
                    ComponentBase& component = component_pool.get_base(entity);
                    component.decode(decoder);
                }
                else
                {
                    // Add and decode a new component of the type
                    std::shared_ptr<ComponentBase> component = ComponentRegistry::create(type_id);
                    component->decode(decoder);
                    add_entity_component_base(entity, *component);
                }

                decoder >> end_object();
            }
            decoder >> end_array();
        }
    }
}

void Scene::receive_event(const EntityEvent& event)
{
    if (event.entity)
    {
        const char* entity_name = event.entity->name().as_string().data();
        const EntityId entity_id = event.entity->id();

        (void)entity_name;
        (void)entity_id;

        switch (event.type)
        {
        case EntityEventType::Create:
            HECT_TRACE(format("Created entity '%s' (id: 0x%08x)", entity_name, entity_id));
            break;
        case EntityEventType::Activate:
            HECT_TRACE(format("Activated entity '%s' (id: 0x%08x)", entity_name, entity_id));
            break;
        case EntityEventType::Destroy:
            HECT_TRACE(format("Destroyed entity '%s' (id: 0x%08x)", entity_name, entity_id));
            break;
        }
    }
}
