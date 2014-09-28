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
#include "World.h"

#include <algorithm>

using namespace hect;

World::World() :
    _entityCount(0),
    _entityPool(*this)
{
    _componentPoolMap = ComponentRegistry::createComponentPoolMap(*this);
    _systemMap = SystemRegistry::createSystemMap(*this);

    // Add all systems to the tick order
    for (auto& pair : _systemMap)
    {
        addSystemToTickOrder(*pair.second);
    }
}

void World::tick(TimeSpan timeStep)
{
    Real timeStepInSeconds = timeStep.seconds();
    for (System* system : _systemTickOrder)
    {
        system->tick(timeStepInSeconds);
    }
}

Entity::Iterator World::createEntity()
{
    Entity::Iterator entity = _entityPool.create();

    // Dispatch the entity create event
    EntityEvent event(EntityEventType_Create, *entity);
    _entityPool.dispatchEvent(event);

    return entity;
}

EntityPool& World::entities()
{
    return _entityPool;
}

const EntityPool& World::entities() const
{
    return _entityPool;
}

size_t World::entityCount() const
{
    return _entityCount;
}

Entity::Iterator World::cloneEntity(const Entity& entity)
{
    Entity::ConstIterator sourceEntity = entity.iterator();
    Entity::Iterator clonedEntity = createEntity();

    for (auto& pair : _componentPoolMap)
    {
        ComponentPoolBase& componentPool = *pair.second;
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

void World::destroyEntity(Entity& entity)
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
    for (auto& pair : _componentPoolMap)
    {
        ComponentPoolBase& componentPool = *pair.second;
        if (componentPool.has(entity))
        {
            componentPool.remove(entity);
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

void World::activateEntity(Entity& entity)
{
    if (!entity.inPool())
    {
        throw Error("Invalid entity");
    }

    if (entity._activated)
    {
        throw Error("Entity is already activated");
    }

    for (auto& pair : _componentPoolMap)
    {
        auto componentPool = pair.second;
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

void World::addEntityComponentBase(Entity& entity, const ComponentBase& component)
{
    if (!entity.inPool())
    {
        throw Error("Invalid entity");
    }

    std::type_index typeIndex = component.typeIndex();
    auto it = _componentPoolMap.find(typeIndex);
    if (it == _componentPoolMap.end())
    {
        throw Error("Type of base component is unregistered");
    }
    ComponentPoolBase& componentPool = *it->second;
    componentPool.addBase(entity, component);
}

void World::encode(Encoder& encoder) const
{
    encoder << beginArray("entities");

    for (const Entity& entity : entities())
    {
        // Only encode the root entities (children are encoded recursively)
        if (!entity.parent())
        {
            encoder << beginObject()
                << encodeValue(entity)
                << endObject();
        }
    }

    encoder << endArray();
}

void World::decode(Decoder& decoder)
{
    decoder >> beginArray("entities");
    while (decoder.hasMoreElements())
    {
        Entity::Iterator entity = createEntity();

        decoder >> beginObject()
            >> decodeValue(*entity)
            >> endObject();

        entity->activate();
    }
    decoder >> endArray();
}

void World::encodeComponents(const Entity& entity, Encoder& encoder)
{
    encoder << beginArray("components");

    for (auto& pair : _componentPoolMap)
    {
        auto componentPool = pair.second;
        if (componentPool->has(entity))
        {
            const ComponentBase& component = componentPool->getBase(entity);
            std::string typeName = Type::of(component).name();

            encoder << beginObject()
                    << encodeValue("type", typeName);

            component.encode(encoder);

            encoder << endObject();
        }
    }

    encoder << endArray();
}

void World::decodeComponents(Entity& entity, Decoder& decoder)
{
    if (decoder.selectMember("components"))
    {
        decoder >> beginArray();
        while (decoder.hasMoreElements())
        {
            decoder >> beginObject();

            std::string typeName;
            decoder >> decodeValue("type", typeName);

            ComponentBase::Pointer component = ComponentRegistry::createComponent(typeName);
            component->decode(decoder);

            addEntityComponentBase(entity, *component);

            decoder >> endObject();
        }
        decoder >> endArray();
    }
}

void World::addSystemToTickOrder(System& system)
{
    // If the system is not in the tick order
    if (std::find(_systemTickOrder.begin(), _systemTickOrder.end(), &system) == _systemTickOrder.end())
    {
        // Add each system it depends on first
        for (std::type_index typeIndex : system.tickDependencies())
        {
            addSystemToTickOrder(*_systemMap[typeIndex]);
        }

        // Add the system
        _systemTickOrder.push_back(&system);
    }
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const World& world)
{
    world.encode(encoder);
    return encoder;
}

Decoder& operator>>(Decoder& decoder, World& world)
{
    world.decode(decoder);
    return decoder;
}

}