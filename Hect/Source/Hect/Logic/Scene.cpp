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

#include "Hect/Graphics/Components/AmbientLight.h"
#include "Hect/Graphics/Components/DirectionalLight.h"
#include "Hect/Graphics/Components/Camera.h"
#include "Hect/Graphics/Components/Geometry.h"
#include "Hect/Graphics/Components/Transform.h"
#include "Hect/Physics/Components/RigidBody.h"
#include "Hect/IO/JsonDecoder.h"

#include <algorithm>

using namespace hect;

Scene::Scene() :
    _entityCount(0),
    _entityPool(*this)
{
    // Register all built-in Hect components
    registerComponent<AmbientLight>("AmbientLight");
    registerComponent<DirectionalLight>("DirectionalLight");
    registerComponent<Camera>("Camera");
    registerComponent<Geometry>("Geometry");
    registerComponent<Transform>("Transform");
    registerComponent<RigidBody>("RigidBody");
}

Entity::Iter Scene::createEntity(const std::string& name)
{
    Entity::Iter entity = _entityPool._create();
    entity->setName(name);
    return entity;
}

Entity::Iter Scene::cloneEntity(const Entity& entity, const std::string& name)
{
    Entity::Iter sourceEntity = Entity::Iter(_entityPool, entity.id());
    Entity::Iter clonedEntity = createEntity();

    for (auto& pair : _componentPools)
    {
        ComponentPoolBase& componentPool = *pair.second;
        componentPool._clone(*sourceEntity, *clonedEntity);
    }

    clonedEntity->setName(name);
    return clonedEntity;
}

void Scene::destroyEntity(Entity& entity)
{
    if (!entity._inPool())
    {
        throw Error("Invalid entity");
    }

    for (auto& pair : _componentPools)
    {
        ComponentPoolBase& componentPool = *pair.second;
        if (componentPool._has(entity))
        {
            componentPool._remove(entity);
        }
    }

    if (entity.isActivated())
    {
        --_entityCount;
    }

    _entityPool._destroy(entity.id());
}

void Scene::activateEntity(Entity& entity)
{
    if (!entity._inPool())
    {
        throw Error("Invalid entity");
    }

    if (entity.isActivated())
    {
        throw Error("Entity is already activated");
    }

    for (auto& pair : _componentPools)
    {
        auto componentPool = pair.second;
        if (componentPool->_has(entity))
        {
            componentPool->_notifyEvent(ComponentEventType::Add, entity);
        }
    }

    ++_entityCount;
    entity._activated = true;

    LOG_TRACE(format("Activated entity '%s'", entity.name().c_str()));
}

void Scene::addEntityComponentBase(Entity& entity, const ComponentBase& component)
{
    if (!entity._inPool())
    {
        throw Error("Invalid entity");
    }

    std::type_index typeIndex = component.typeIndex();
    auto it = _componentPools.find(typeIndex);
    if (it != _componentPools.end())
    {
        it->second->_addBase(entity, component);
    }
}

void Scene::encodeComponents(const Entity& entity, ObjectEncoder& encoder)
{
    ArrayEncoder componentsEncoder = encoder.encodeArray("components");

    for (auto& pair : _componentPools)
    {
        auto componentPool = pair.second;
        if (componentPool->_has(entity))
        {
            const ComponentBase& component = componentPool->_getBase(entity);
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

            entity._addComponentBase(*component);
        }
    }
}

void Scene::encode(ObjectEncoder& encoder) const
{
    ArrayEncoder entitiesEncoder = encoder.encodeArray("entities");
    for (const Entity& entity : entities())
    {
        ObjectEncoder entityEncoder = entitiesEncoder.encodeObject();
        entity.encode(entityEncoder);
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