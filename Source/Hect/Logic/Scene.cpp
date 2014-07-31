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

#include <algorithm>

#include "Hect/IO/JsonDecoder.h"

using namespace hect;

Scene::Scene() :
    _entityCount(0),
    _entityPool(*this)
{
}

Entity::Iter Scene::createEntity()
{
    Entity::Iter entity = _entityPool.create();

    // Dispatch the entity create event
    EntityEvent event(EntityEventType_Create, *entity);
    _entityPool._dispatcher.dispatchEvent(event);

    return entity;
}

Entity::Iter Scene::createEntity(const Path& entityPath, AssetCache& assetCache)
{
    JsonValue& jsonValue = assetCache.get<JsonValue>(entityPath);

    assetCache.pushPreferredDirectory(entityPath.parentDirectory());

    Entity::Iter entity = createEntity();
    entity->decodeFromJsonValue(jsonValue, assetCache);

    assetCache.popPreferredDirectory();

    return entity;
}

void Scene::encode(ObjectEncoder& encoder) const
{
    ArrayEncoder entitiesEncoder = encoder.encodeArray("entities");
    for (const Entity& entity : entities())
    {
        // Only encode the root entities (children are encoded recursively)
        if (!entity.parent())
        {
            ObjectEncoder entityEncoder = entitiesEncoder.encodeObject();
            entity.encode(entityEncoder);
        }
    }
}

void Scene::decode(ObjectDecoder& decoder, AssetCache& assetCache)
{
    ArrayDecoder entitiesDecoder = decoder.decodeArray("entities");
    while (entitiesDecoder.hasMoreElements())
    {
        Entity::Iter entity = createEntity();

        ObjectDecoder entityDecoder = entitiesDecoder.decodeObject();
        entity->decode(decoder, assetCache);

        entity->activate();
    }
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

Entity::Iter Scene::cloneEntity(const Entity& entity)
{
    Entity::ConstIter sourceEntity = entity.iter();
    Entity::Iter clonedEntity = createEntity();

    for (auto& pair : _componentPools)
    {
        ComponentPoolBase& componentPool = *pair.second;
        componentPool.clone(*sourceEntity, *clonedEntity);
    }

    // Recursively clone all children
    for (const Entity& child : sourceEntity->children())
    {
        Entity::Iter clonedChild = child.clone();
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
    _entityPool._dispatcher.dispatchEvent(event);

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
    for (auto& pair : _componentPools)
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
    Entity::Iter parent = entity.parent();
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

    for (auto& pair : _componentPools)
    {
        auto componentPool = pair.second;
        if (componentPool->has(entity))
        {
            componentPool->notifyEvent(ComponentEventType_Add, entity);
        }
    }

    ++_entityCount;
    entity._activated = true;

    // Dispatch the entity activate event
    EntityEvent event(EntityEventType_Activate, entity);
    _entityPool._dispatcher.dispatchEvent(event);

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

    std::type_index typeIndex = component.typeIndex();
    auto it = _componentPools.find(typeIndex);
    if (it != _componentPools.end())
    {
        it->second->addBase(entity, component);
    }
}

void Scene::encodeComponents(const Entity& entity, ObjectEncoder& encoder)
{
    ArrayEncoder componentsEncoder = encoder.encodeArray("components");

    for (auto& pair : _componentPools)
    {
        auto componentPool = pair.second;
        if (componentPool->has(entity))
        {
            const ComponentBase& component = componentPool->getBase(entity);
            std::string typeName = _componentTypeNames[component.typeIndex()];

            ObjectEncoder componentEncoder = componentsEncoder.encodeObject();
            componentEncoder.encodeString("type", typeName);
            component.encode(componentEncoder);
        }
    }
}

void Scene::decodeComponents(Entity& entity, ObjectDecoder& decoder, AssetCache& assetCache)
{
    if (decoder.hasMember("components"))
    {
        ArrayDecoder componentsDecoder = decoder.decodeArray("components");
        while (componentsDecoder.hasMoreElements())
        {
            ObjectDecoder componentDecoder = componentsDecoder.decodeObject();

            std::string componentName = componentDecoder.decodeString("type"); 

            auto it = _componentConstructors.find(componentName);
            if (it == _componentConstructors.end())
            {
                throw Error(format("Unregistered component type '%s'", componentName.c_str()));
            }

            std::unique_ptr<ComponentBase> component(it->second());
            component->decode(componentDecoder, assetCache);

            addEntityComponentBase(entity, *component);
        }
    }
}