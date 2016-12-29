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
    _entityPool(*this)
{
    // Register to entity events if trace-level logging is enabled
    if (isLogLevelEnabled(LogLevel::Trace))
    {
        _entityPool.registerListener(*this);
    }

    for (ComponentTypeId typeId : ComponentRegistry::typeIds())
    {
        addComponentType(typeId);
    }
}

Scene::~Scene()
{
}

SystemBase& Scene::systemOfTypeId(SystemTypeId typeId)
{
    if (!SystemRegistry::isRegisteredTypeId(typeId))
    {
        throw InvalidOperation("Unknown system type id");
    }
    else if (typeId >= _systems.size() || !_systems[typeId])
    {
        Name typeName = SystemRegistry::typeNameOf(typeId);
        throw InvalidOperation(format("Scene does not support system type '%s'", typeName.data()));
    }
    else
    {
        return *_systems[typeId];
    }
}

bool Scene::hasSystemOfTypeId(SystemTypeId typeId)
{
    return typeId < _systems.size() && _systems[typeId];
}

const SystemBase& Scene::systemOfTypeId(SystemTypeId typeId) const
{
    return const_cast<Scene*>(this)->systemOfTypeId(typeId);
}

bool Scene::active() const
{
    return _active;
}

void Scene::setActive(bool active)
{
    _active = active;
}

void Scene::refresh()
{
    if (!_systemsToInitialize.empty())
    {
        for (SystemBase* system : _systemsToInitialize)
        {
            system->initialize();
        }
        _systemsToInitialize.clear();
    }

    // Create/activate/destroy all pending entities and dispatch related
    while (hasPendingEntities())
    {
        dispatchEntityCreationEvents();
        activatePendingEntities();
        destroyPendingEntities();
    }
}

bool Scene::isInitialized() const
{
    return _initialized;
}

void Scene::initialize()
{
    _initialized = true;
}

Entity::Iterator Scene::createEntity(Name name)
{
    Entity::Iterator entity = _entityPool.create(name);
    _entitiesPendingCreation.push_back(entity->id());
    return entity;
}

Entity::Iterator Scene::loadEntity(const Path& path)
{
    Entity::Iterator entity = createEntity();

    AssetDecoder decoder(_engine->assetCache(), path);
    decoder >> decodeValue(*entity);

    return entity;
}

EntityPool& Scene::entities()
{
    return _entityPool;
}

const EntityPool& Scene::entities() const
{
    return _entityPool;
}

size_t Scene::entityCount() const
{
    return _entityCount;
}

void Scene::encode(Encoder& encoder) const
{
    std::type_index typeIndex(typeid(*this));
    SceneTypeId sceneTypeId = SceneRegistry::typeIdOf(typeIndex);
    if (encoder.isBinaryStream())
    {
        encoder << encodeValue(sceneTypeId);
    }
    else
    {
        Name typeName = SceneRegistry::typeNameOf(sceneTypeId);
        encoder << encodeValue("sceneType", typeName.asString());
    }

    // Systems
    encoder << beginArray("systems");
    for (auto& system : _systems)
    {
        if (system)
        {
            encoder << beginObject();

            if (encoder.isBinaryStream())
            {
                std::type_index typeIndex(typeid(*system));
                encoder << encodeValue(SystemRegistry::typeIdOf(typeIndex));
            }
            else
            {
                Name typeName = Type::of(*system).name();
                encoder << encodeValue("systemType", typeName);
            }

            system->encode(encoder);

            encoder << endObject();
        }
    }
    encoder << endArray();

    // Entities
    encoder << beginArray("entities");
    for (const Entity& entity : entities())
    {
        // Only encode the root entities (children are encoded recursively)
        if (!entity.parent())
        {
            encoder << encodeValue(entity);
        }
    }
    encoder << endArray();
}

void Scene::decode(Decoder& decoder)
{
    // Base
    if (!decoder.isBinaryStream())
    {
        if (decoder.selectMember("base"))
        {
            Path basePath;
            decoder >> decodeValue(basePath);

            try
            {
                AssetDecoder baseDecoder(decoder.assetCache(), basePath);
                baseDecoder >> beginObject() >> decodeValue(*this) >> endObject();
            }
            catch (const Exception& exception)
            {
                throw DecodeError(format("Failed to load base scene '%s': %s", basePath.asString().data(), exception.what()));
            }
        }
    }

    // Decode the scene type
    // Note that nothing is done with the resulting value since the scene has
    // already been constructed (obviously); this is only done to make encoding
    // and decoding reflective
    if (decoder.isBinaryStream())
    {
        SceneTypeId typeId;
        decoder >> decodeValue(typeId);
    }
    else
    {
        std::string typeName;
        decoder >> decodeValue("sceneType", typeName);
    }

    // Systems
    if (decoder.selectMember("systems"))
    {
        decoder >> beginArray();
        while (decoder.hasMoreElements())
        {
            decoder >> beginObject();

            SystemTypeId typeId;
            if (decoder.isBinaryStream())
            {
                ReadStream& stream = decoder.binaryStream();
                stream >> typeId;
            }
            else
            {
                std::string typeName;
                decoder >> decodeValue("systemType", typeName, true);
                typeId = SystemRegistry::typeIdOf(typeName);
            }

            SystemBase& system = systemOfTypeId(typeId);
            system.decode(decoder);

            decoder >> endObject();
        }
        decoder >> endArray();
    }

    // Entities
    if (decoder.selectMember("entities"))
    {
        decoder >> beginArray();
        while (decoder.hasMoreElements())
        {
            Entity::Iterator entity = createEntity();
            decoder >> decodeValue(*entity);
            entity->activate();
        }
        decoder >> endArray();
    }

    refresh();
}

Engine& Scene::engine() const
{
    assert(_engine);
    return *_engine;
}

void Scene::addSystemType(SystemTypeId typeId)
{
    // Make sure the system isn't already added
    if (typeId < _systems.size() && _systems[typeId])
    {
        Name typeName = SystemRegistry::typeNameOf(typeId);
        throw InvalidOperation(format("Scene already supports system type '%s'", typeName.data()));
    }

    // Make sure the type id is a real type id
    if (!SystemRegistry::isRegisteredTypeId(typeId))
    {
        throw InvalidOperation("Unknown system type id");
    }

    // Resize the systems vector if needed
    while (typeId >= _systems.size())
    {
        size_t oldSize = _systems.size();
        _systems.resize(std::max(oldSize * 2, size_t(8)));
    }

    // Add the system
    auto system = SystemRegistry::create(typeId, *_engine, *this);
    _systems[typeId] = system;
    _systemTypeIds.push_back(typeId);
    _systemsToInitialize.push_back(system.get());
}

void Scene::addComponentType(ComponentTypeId typeId)
{
    // Make sure the component type isn't already added
    if (typeId < _componentPools.size() && _componentPools[typeId])
    {
        Name typeName = ComponentRegistry::typeNameOf(typeId);
        throw InvalidOperation(format("Scene already supports component type '%s'", typeName.data()));
    }

    // Make sure the type id is a real type id
    if (!ComponentRegistry::isRegisteredTypeId(typeId))
    {
        throw InvalidOperation("Unknown component type id");
    }

    // Resize the component pool vector if needed
    while (typeId >= _componentPools.size())
    {
        size_t oldSize = _componentPools.size();
        _componentPools.resize(std::max(oldSize * 2, size_t(8)));
    }

    // Add the component pool
    auto componentPool = ComponentRegistry::createPool(typeId, *this);
    _componentPools[typeId] = componentPool;
    _componentTypeIds.push_back(typeId);
}

ComponentPoolBase& Scene::componentPoolOfTypeId(ComponentTypeId typeId)
{
    if (!ComponentRegistry::isRegisteredTypeId(typeId))
    {
        throw InvalidOperation("Unknown component type id");
    }
    else if (typeId >= _componentPools.size() || !_componentPools[typeId])
    {
        Name typeName = ComponentRegistry::typeNameOf(typeId);
        throw InvalidOperation(format("Scene does not support component type '%s'", typeName.data()));
    }
    else
    {
        return *_componentPools[typeId];
    }
}

Entity::Iterator Scene::cloneEntity(const Entity& entity)
{
    Entity::ConstIterator sourceEntity = entity.iterator();
    Entity::Iterator clonedEntity = createEntity(sourceEntity->name());

    for (ComponentTypeId typeId : _componentTypeIds)
    {
        ComponentPoolBase& componentPool = componentPoolOfTypeId(typeId);
        componentPool.clone(*sourceEntity, *clonedEntity);
    }

    // Recursively clone all children
    for (const Entity& child : sourceEntity->children())
    {
        Entity::Iterator clonedChild = child.clone();
        clonedEntity->addChild(*clonedChild);
    }

    return clonedEntity;
}

void Scene::destroyEntity(Entity& entity)
{
    if (!entity.inPool())
    {
        throw InvalidOperation("Invalid entity");
    }

    // Dispatch the entity destroy event
    EntityEvent event;
    event.type = EntityEventType::Destroy;
    event.entity = entity.iterator();
    _entityPool.dispatchEvent(event);

    // Destroy all children
    std::vector<EntityId> childIds;
    for (Entity& child : entity.children())
    {
        childIds.push_back(child._id);
    }
    for (EntityId childId : childIds)
    {
        _entityPool.entityWithId(childId).destroy();
    }

    // Remove all components
    for (ComponentTypeId typeId : _componentTypeIds)
    {
        ComponentPoolBase& componentPool = *_componentPools[typeId];
        if (componentPool.has(entity))
        {
            componentPool.remove(entity);
        }
    }

    if (entity.isActivated())
    {
        --_entityCount;
    }

    // If the entity had a parent then remove itself as a child
    Entity::Iterator parent = entity.parent();
    if (parent)
    {
        parent->removeChild(entity);
    }

    _entityPool.destroy(entity._id);
}

void Scene::activateEntity(Entity& entity)
{
    if (!entity.inPool())
    {
        throw InvalidOperation("Invalid entity");
    }

    if (entity.isActivated())
    {
        throw InvalidOperation("Entity is already activated");
    }

    ++_entityCount;
    entity.setFlag(Entity::Flag::Activated, true);
    entity.setFlag(Entity::Flag::PendingActivation, false);

    Entity::Iterator entityIterator = entity.iterator();

    for (ComponentTypeId typeId : _componentTypeIds)
    {
        ComponentPoolBase& componentPool = *_componentPools[typeId];
        if (componentPool.has(*entityIterator))
        {
            componentPool.dispatchEvent(ComponentEventType::Add, *entityIterator);
        }
    }

    // Dispatch the entity activate event
    EntityEvent event;
    event.type = EntityEventType::Activate;
    event.entity = entityIterator;
    _entityPool.dispatchEvent(event);
}

void Scene::pendEntityDestruction(Entity& entity)
{
    if (!entity.inPool())
    {
        throw InvalidOperation("Invalid entity");
    }

    if (entity.isPendingDestruction())
    {
        throw InvalidOperation("Entity is already pending destruction");
    }

    entity.setFlag(Entity::Flag::PendingDestruction, true);
    _entitiesPendingDestruction.push_back(entity._id);
}

void Scene::pendEntityActivation(Entity& entity)
{
    if (!entity.inPool())
    {
        throw InvalidOperation("Invalid entity");
    }

    if (entity.isPendingActivation())
    {
        throw InvalidOperation("Entity is already pending activation");
    }
    else if (entity.isActivated())
    {
        throw InvalidOperation("Entity is already activated");
    }

    entity.setFlag(Entity::Flag::PendingActivation, true);

    // Enqueue the entity to activate on the next refresh
    _entitiesPendingActivation.push_back(entity._id);
}

bool Scene::hasPendingEntities() const
{
    return !_entitiesPendingCreation.empty() || !_entitiesPendingActivation.empty() || !_entitiesPendingDestruction.empty();
}

void Scene::dispatchEntityCreationEvents()
{
    // Dispatch the entity creation event for all entities pending creation
    while (!_entitiesPendingCreation.empty())
    {
        const EntityId entityId = _entitiesPendingCreation.front();
        _entitiesPendingCreation.pop_front();

        // Dispatch an entity create event
        EntityEvent event;
        event.type = EntityEventType::Create;
        event.entity = EntityIterator(_entityPool, entityId);
        _entityPool.dispatchEvent(event);
    }
}

void Scene::activatePendingEntities()
{
    // Activate all entities pending activation
    while (!_entitiesPendingActivation.empty())
    {
        const EntityId entityId = _entitiesPendingActivation.front();
        _entitiesPendingActivation.pop_front();

        Entity& entity = _entityPool.entityWithId(entityId);
        activateEntity(entity);
    }
}

void Scene::destroyPendingEntities()
{
    // Destroy all entities set pending destruction
    while (!_entitiesPendingDestruction.empty())
    {
        const EntityId entityId = _entitiesPendingDestruction.front();
        _entitiesPendingDestruction.pop_front();

        Entity& entity = _entityPool.entityWithId(entityId);
        destroyEntity(entity);
    }
}

void Scene::addEntityComponentBase(Entity& entity, const ComponentBase& component)
{
    if (!entity.inPool())
    {
        throw InvalidOperation("Invalid entity");
    }

    ComponentTypeId typeId = component.typeId();
    ComponentPoolBase& componentPool = componentPoolOfTypeId(typeId);
    componentPool.addBase(entity, component);
}

void Scene::encodeComponents(const Entity& entity, Encoder& encoder)
{
    if (encoder.isBinaryStream())
    {
        WriteStream& stream = encoder.binaryStream();

        size_t componentCountPosition = stream.position();
        uint8_t componentCount = 0;
        stream << componentCount;

        for (ComponentTypeId typeId : _componentTypeIds)
        {
            ComponentPoolBase& componentPool = *_componentPools[typeId];
            if (componentPool.has(entity))
            {
                ++componentCount;

                const ComponentBase& component = componentPool.getBase(entity);
                stream << component.typeId();
                component.encode(encoder);
            }
        }

        size_t currentPosition = stream.position();
        stream.seek(componentCountPosition);
        stream << componentCount;
        stream.seek(currentPosition);
    }
    else
    {
        encoder << beginArray("components");

        for (ComponentTypeId typeId : _componentTypeIds)
        {
            ComponentPoolBase& componentPool = *_componentPools[typeId];
            if (componentPool.has(entity))
            {
                encoder << beginObject();

                const ComponentBase& component = componentPool.getBase(entity);
                Name typeName = Type::of(component).name();

                encoder << encodeValue("componentType", typeName);
                component.encode(encoder);

                encoder << endObject();
            }
        }

        encoder << endArray();
    }
}

void Scene::decodeComponents(Entity& entity, Decoder& decoder)
{
    if (decoder.isBinaryStream())
    {
        ReadStream& stream = decoder.binaryStream();
        uint8_t componentCount;
        stream >> componentCount;
        for (uint8_t i = 0; i < componentCount; ++i)
        {
            ComponentTypeId typeId;
            stream >> typeId;
            std::shared_ptr<ComponentBase> component = ComponentRegistry::create(typeId);
            component->decode(decoder);
            addEntityComponentBase(entity, *component);
        }
    }
    else
    {
        if (decoder.selectMember("components"))
        {
            decoder >> beginArray();
            while (decoder.hasMoreElements())
            {
                decoder >> beginObject();

                std::string typeName;
                decoder >> decodeValue("componentType", typeName);

                ComponentTypeId typeId = ComponentRegistry::typeIdOf(typeName);

                // If the entity already has a component of this type
                ComponentPoolBase& componentPool = componentPoolOfTypeId(typeId);
                if (componentPool.has(entity))
                {
                    // Re-decode the existing component
                    ComponentBase& component = componentPool.getBase(entity);
                    component.decode(decoder);
                }
                else
                {
                    // Add and decode a new component of the type
                    std::shared_ptr<ComponentBase> component = ComponentRegistry::create(typeId);
                    component->decode(decoder);
                    addEntityComponentBase(entity, *component);
                }

                decoder >> endObject();
            }
            decoder >> endArray();
        }
    }
}

void Scene::receiveEvent(const EntityEvent& event)
{
    if (event.entity)
    {
        const char* entityName = event.entity->name().asString().data();
        const EntityId entityId = event.entity->id();

        (void)entityName;
        (void)entityId;

        switch (event.type)
        {
        case EntityEventType::Create:
            HECT_TRACE(format("Created entity '%s' (id: 0x%08x)", entityName, entityId));
            break;
        case EntityEventType::Activate:
            HECT_TRACE(format("Activated entity '%s' (id: 0x%08x)", entityName, entityId));
            break;
        case EntityEventType::Destroy:
            HECT_TRACE(format("Destroyed entity '%s' (id: 0x%08x)", entityName, entityId));
            break;
        }
    }
}