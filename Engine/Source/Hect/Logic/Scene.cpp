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

using namespace hect;

Scene::Scene(Engine& engine) :
    _engine(engine),
    _entityCount(0),
    _entityPool(*this)
{
    _componentPoolMap = ComponentRegistry::createPoolMap(*this);
}

Engine& Scene::engine()
{
    return _engine;
}

const Engine& Scene::engine() const
{
    return _engine;
}

void Scene::tick(TimeSpan timeStep)
{
    Real timeStepInSeconds = timeStep.seconds();
    for (auto& system : _systemTickOrder)
    {
        system->tick(timeStepInSeconds);
    }
}

Entity::Iterator Scene::createEntity()
{
    Entity::Iterator entity = _entityPool.create();

    // Dispatch the entity create event
    EntityEvent event(EntityEventType_Create, *entity);
    _entityPool.dispatchEvent(event);

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

System& Scene::addOrGetSystem(SystemTypeId typeId, bool& added)
{
    // If the system is already added then return it
    if (typeId < _systems.size() && _systems[typeId])
    {
        added = false;
        return *_systems[typeId];
    }

    // Otherwise, attempt to add the system
    else
    {
        added = true;

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

        return *system;
    }
}

Entity::Iterator Scene::cloneEntity(const Entity& entity)
{
    Entity::ConstIterator sourceEntity = entity.iterator();
    Entity::Iterator clonedEntity = createEntity();

    for (std::shared_ptr<ComponentPoolBase>& componentPool : _componentPoolMap)
    {
        componentPool->clone(*sourceEntity, *clonedEntity);
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
    for (std::shared_ptr<ComponentPoolBase>& componentPool : _componentPoolMap)
    {
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

    for (std::shared_ptr<ComponentPoolBase>& componentPool : _componentPoolMap)
    {
        if (componentPool->has(entity))
        {
            componentPool->dispatchEvent(ComponentEventType_Add, entity);
        }
    }

    ++_entityCount;
    entity._activated = true;

    // Dispatch the entity activate event
    EntityEvent event(EntityEventType_Activate, entity);
    _entityPool.dispatchEvent(event);

    // Activate all children
    for (Entity& child : entity.children())
    {
        child.activate();
    }
}

void Scene::addEntityComponentBase(Entity& entity, const ComponentBase& component)
{
    if (!entity.inPool())
    {
        throw Error("Invalid entity");
    }

    ComponentTypeId typeId = component.typeId();
    std::shared_ptr<ComponentPoolBase>& componentPool = _componentPoolMap[typeId];
    componentPool->addBase(entity, component);
}

void Scene::encode(Encoder& encoder) const
{
    encoder << beginObject();

    // Systems
    encoder << beginArray("systems");
    for (auto& system : _systemTickOrder)
    {
        encoder << beginObject();

        std::string typeName = Type::of(*system).name();
        if (encoder.isBinaryStream())
        {
            WriteStream& stream = encoder.binaryStream();
            stream << SystemRegistry::typeIdOf(typeName);
        }
        else
        {
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

            bool added;
            System& system = addOrGetSystem(typeId, added);
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

        for (std::shared_ptr<ComponentPoolBase>& componentPool : _componentPoolMap)
        {
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

        for (std::shared_ptr<ComponentPoolBase>& componentPool : _componentPoolMap)
        {
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