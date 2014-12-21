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
#include "Scene.h"

#include "Hect/IO/AssetDecoder.h"
#include "Hect/Runtime/Engine.h"

using namespace hect;

Scene::Scene(Engine& engine) :
    _engine(engine),
    _entityPool(*this)
{
}

Engine& Scene::engine()
{
    return _engine;
}

const Engine& Scene::engine() const
{
    return _engine;
}

void Scene::refresh()
{
    // Create all entities pending creation
    for (EntityId entityId : _entitiesPendingCreation)
    {
        Entity& entity = _entityPool.entityWithId(entityId);
        EntityEvent event(EntityEventType_Create, entity);
        _entityPool.dispatchEvent(event);
    }
    _entitiesPendingCreation.clear();

    // Activate all entities pending activation
    for (EntityId entityId : _entitiesPendingActivation)
    {
        Entity& entity = _entityPool.entityWithId(entityId);
        activateEntity(entity);
    }
    _entitiesPendingActivation.clear();

    // Destroy all entities set pending destruction
    for (EntityId entityId : _entitiesPendingDestruction)
    {
        Entity& entity = _entityPool.entityWithId(entityId);
        destroyEntity(entity);
    }
    _entitiesPendingDestruction.clear();
}

void Scene::tick(TimeSpan timeStep)
{
    refresh();

    // Tick all systems in order
    Real timeStepInSeconds = timeStep.seconds();
    for (auto& system : _systemTickOrder)
    {
        system->tick(timeStepInSeconds);
    }
}

Entity::Iterator Scene::createEntity()
{
    Entity::Iterator entity = _entityPool.create();

    _entitiesPendingCreation.push_back(entity->id());

    return entity;
}

Entity::Iterator Scene::createEntity(const Path& path)
{
    Entity::Iterator entity = createEntity();

    AssetDecoder decoder(_engine.assetCache(), path);
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

void Scene::addSystemType(SystemTypeId typeId)
{
    // Make sure the system isn't already added
    if (typeId < _systems.size() && _systems[typeId])
    {
        const std::string typeName = SystemRegistry::typeNameOf(typeId);
        throw Error(format("Scene already supports system type '%s'", typeName.c_str()));
    }

    // Make sure the type id is a real type id
    if (!SystemRegistry::isRegisteredTypeId(typeId))
    {
        throw Error("Unknown system type id");
    }

    // Resize the systems vector if needed
    while (typeId >= _systems.size())
    {
        size_t oldSize = _systems.size();
        _systems.resize(std::max(oldSize * 2, size_t(8)));
    }

    // Add the system
    auto system = SystemRegistry::create(typeId, *this);
    _systems[typeId] = system;
    _systemTickOrder.push_back(system.get());
    _systemTypeIds.push_back(typeId);
}

System& Scene::systemOfTypeId(SystemTypeId typeId)
{
    if (!SystemRegistry::isRegisteredTypeId(typeId))
    {
        throw Error("Unknown system type id");
    }
    else if (typeId >= _systems.size() || !_systems[typeId])
    {
        const std::string& typeName = SystemRegistry::typeNameOf(typeId);
        throw Error(format("Scene does not support system type '%s'", typeName.c_str()));
    }
    else
    {
        return *_systems[typeId];
    }
}

void Scene::addComponentType(ComponentTypeId typeId)
{
    // Make sure the component type isn't already added
    if (typeId < _componentPools.size() && _componentPools[typeId])
    {
        const std::string typeName = ComponentRegistry::typeNameOf(typeId);
        throw Error(format("Scene already supports component type '%s'", typeName.c_str()));
    }

    // Make sure the type id is a real type id
    if (!ComponentRegistry::isRegisteredTypeId(typeId))
    {
        throw Error("Unknown component type id");
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
        throw Error("Unknown component type id");
    }
    else if (typeId >= _componentPools.size() || !_componentPools[typeId])
    {
        const std::string& typeName = ComponentRegistry::typeNameOf(typeId);
        throw Error(format("Scene does not support component type '%s'", typeName.c_str()));
    }
    else
    {
        return *_componentPools[typeId];
    }
}

Entity::Iterator Scene::cloneEntity(const Entity& entity)
{
    Entity::ConstIterator sourceEntity = entity.iterator();
    Entity::Iterator clonedEntity = createEntity();

    for (ComponentTypeId typeId : _componentTypeIds)
    {
        auto& componentPool = componentPoolOfTypeId(typeId);
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
        throw Error("Invalid entity");
    }

    // Dispatch the entity destroy event
    EntityEvent event(EntityEventType_Destroy, entity);
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
        auto& componentPool = _componentPools[typeId];
        if (componentPool->has(entity))
        {
            componentPool->remove(entity);
        }
    }

    if (entity._activated)
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
        throw Error("Invalid entity");
    }

    if (entity._activated)
    {
        throw Error("Entity is already activated");
    }

    for (ComponentTypeId typeId : _componentTypeIds)
    {
        auto& componentPool = _componentPools[typeId];
        if (componentPool->has(entity))
        {
            componentPool->dispatchEvent(ComponentEventType_Add, entity);
        }
    }

    ++_entityCount;
    entity._activated = true;
    entity._pendingActivation = false;

    // Dispatch the entity activate event
    EntityEvent event(EntityEventType_Activate, entity);
    _entityPool.dispatchEvent(event);
}

void Scene::pendEntityDestruction(Entity& entity)
{
    if (!entity.inPool())
    {
        throw Error("Invalid entity");
    }

    if (entity._pendingDestruction)
    {
        throw Error("Entity is already pending destruction");
    }

    entity._pendingDestruction = true;
    _entitiesPendingDestruction.push_back(entity._id);
}

void Scene::pendEntityActivation(Entity& entity)
{
    if (!entity.inPool())
    {
        throw Error("Invalid entity");
    }

    if (entity._pendingActivation)
    {
        throw Error("Entity is already pending activation");
    }
    else if (entity._activated)
    {
        throw Error("Entity is already activated");
    }

    entity._pendingActivation = true;
    _entitiesPendingActivation.push_back(entity._id);
}

void Scene::addEntityComponentBase(Entity& entity, const ComponentBase& component)
{
    if (!entity.inPool())
    {
        throw Error("Invalid entity");
    }

    ComponentTypeId typeId = component.typeId();
    auto& componentPool = componentPoolOfTypeId(typeId);
    componentPool.addBase(entity, component);
}

void Scene::encode(Encoder& encoder) const
{
    encoder << beginObject();

    // Component types
    encoder << beginArray("componentTypes");
    for (ComponentTypeId typeId : _componentTypeIds)
    {
        if (encoder.isBinaryStream())
        {
            encoder << encodeValue(typeId);
        }
        else
        {
            const std::string& typeName = ComponentRegistry::typeNameOf(typeId);
            encoder << encodeValue(typeName);
        }
    }
    encoder << endArray();

    // System types
    encoder << beginArray("systemTypes");
    for (SystemTypeId typeId : _systemTypeIds)
    {
        if (encoder.isBinaryStream())
        {
            encoder << encodeValue(typeId);
        }
        else
        {
            const std::string& typeName = SystemRegistry::typeNameOf(typeId);
            encoder << encodeValue(typeName);
        }
    }
    encoder << endArray();

    // Systems
    encoder << beginArray("systems");
    for (auto& system : _systemTickOrder)
    {
        encoder << beginObject();

        if (encoder.isBinaryStream())
        {
            std::type_index typeIndex(typeid(*system));
            encoder << encodeValue(SystemRegistry::typeIdOf(typeIndex));
        }
        else
        {
            std::string typeName = Type::of(*system).name();
            encoder << encodeValue("type", typeName);
        }

        system->encode(encoder);

        encoder << endObject();
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

    encoder << endObject();
}

void Scene::decode(Decoder& decoder)
{
    decoder >> beginObject();

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
            catch (Error& error)
            {
                throw Error(format("Failed to load base scene '%s': %s", basePath.asString().c_str(), error.what()));
            }
        }
    }

    // Component types
    if (decoder.selectMember("componentTypes"))
    {
        decoder >> beginArray();
        while (decoder.hasMoreElements())
        {
            ComponentTypeId typeId;
            if (decoder.isBinaryStream())
            {
                decoder >> decodeValue(typeId);
            }
            else
            {
                std::string typeName;
                decoder >> decodeValue(typeName);
                typeId = ComponentRegistry::typeIdOf(typeName);
            }

            addComponentType(typeId);
        }
        decoder >> endArray();
    }

    // System types
    if (decoder.selectMember("systemTypes"))
    {
        decoder >> beginArray();
        while (decoder.hasMoreElements())
        {
            SystemTypeId typeId;
            if (decoder.isBinaryStream())
            {
                decoder >> decodeValue(typeId);
            }
            else
            {
                std::string typeName;
                decoder >> decodeValue(typeName);
                typeId = SystemRegistry::typeIdOf(typeName);
            }

            addSystemType(typeId);
        }
        decoder >> endArray();
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
                decoder >> decodeValue("type", typeName, true);
                typeId = SystemRegistry::typeIdOf(typeName);
            }

            System& system = systemOfTypeId(typeId);
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

    decoder >> endObject();
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
            auto& componentPool = _componentPools[typeId];
            if (componentPool->has(entity))
            {
                ++componentCount;

                const ComponentBase& component = componentPool->getBase(entity);
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
            auto& componentPool = _componentPools[typeId];
            if (componentPool->has(entity))
            {
                encoder << beginObject();

                const ComponentBase& component = componentPool->getBase(entity);
                std::string typeName = Type::of(component).name();

                encoder << encodeValue("type", typeName);
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
                decoder >> decodeValue("type", typeName);

                ComponentTypeId typeId = ComponentRegistry::typeIdOf(typeName);
                std::shared_ptr<ComponentBase> component = ComponentRegistry::create(typeId);
                component->decode(decoder);

                addEntityComponentBase(entity, *component);

                decoder >> endObject();
            }
            decoder >> endArray();
        }
    }
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const Scene& scene)
{
    scene.encode(encoder);
    return encoder;
}

Decoder& operator>>(Decoder& decoder, Scene& scene)
{
    scene.decode(decoder);
    return decoder;
}

}