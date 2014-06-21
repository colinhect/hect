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
    Entity::Iter entity = _entityPool.create();
    entity->setName(name);
    return entity;
}

Entity::Iter Scene::cloneEntity(const Entity& entity, const std::string& name)
{
    Entity::Iter clonedEntity = createEntity();

    for (auto& pair : _componentPools)
    {
        pair.second->clone(entity, *clonedEntity);
    }

    clonedEntity->setName(name);
    return clonedEntity;
}

void Scene::destroyEntity(Entity& entity)
{
    if (!entity.inPool())
    {
        throw Error("Invalid entity");
    }

    for (auto& pair : _componentPools)
    {
        ComponentPoolBase& componentPool = *pair.second;
        if (componentPool.has(entity))
        {
            componentPool.remove(entity);
        }
    }

    _entityPool.destroy(entity.id());
}

void Scene::activateEntity(Entity& entity)
{
    if (!entity.inPool())
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
        if (componentPool->has(entity))
        {
            componentPool->dispatcher().notifyEvent(ComponentPoolEvent(ComponentPoolEventType::Add, entity.id()));
        }
    }

    entity._activated = true;
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
            std::unique_ptr<ComponentBase> component(_createComponentByName(componentName));
            component->decode(componentDecoder, assetCache);

            entity.addComponentBase(*component);
        }
    }
}

void Scene::encode(ObjectEncoder& encoder) const
{
    ArrayEncoder entitiesEncoder = encoder.encodeArray("entities");
    //
}

void Scene::decode(ObjectDecoder& decoder, AssetCache& assetCache)
{
    ArrayDecoder entitiesDecoder = decoder.decodeArray("entities");
    while (entitiesDecoder.hasMoreElements())
    {
        Entity::Iter entity = createEntity();

        ObjectDecoder entityDecoder = entitiesDecoder.decodeObject();
        {
            if (entityDecoder.hasMember("archetype"))
            {
                std::string archetype = entityDecoder.decodeString("archetype");
                JsonValue& jsonValue = assetCache.get<JsonValue>(archetype);

                JsonDecoder jsonDecoder(jsonValue);
                ObjectDecoder archetypeDecoder = jsonDecoder.decodeObject();
                decodeComponents(*entity, archetypeDecoder, assetCache);
            }

            decodeComponents(*entity, entityDecoder, assetCache);
        }

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

ComponentBase* Scene::_createComponentByName(const std::string& componentName)
{
    auto it = _componentConstructors.find(componentName);
    if (it == _componentConstructors.end())
    {
        throw Error(format("Unknown component type '%s'", componentName.c_str()));
    }

    return it->second();
}