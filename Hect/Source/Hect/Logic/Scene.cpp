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

#include <algorithm>

using namespace hect;

EntityData::EntityData() :
    exists(false),
    activated(false)
{
}

Scene::Scene() :
    _entityCount(0)
{
    // Register all built-in Hect components
    registerComponent<AmbientLight>("AmbientLight");
    registerComponent<DirectionalLight>("DirectionalLight");
    registerComponent<Camera>("Camera");
    registerComponent<Geometry>("Geometry");
    registerComponent<Transform>("Transform");
    registerComponent<RigidBody>("RigidBody");
}

Entity Scene::createEntity()
{
    EntityId entityId = _entityIdPool.create();

    while (entityId >= _entityData.size())
    {
        _entityData.resize(std::max(_entityData.size() * 2, (size_t)8));
    }

    EntityData& entityData = _entityData[entityId];
    entityData.exists = true;
    entityData.activated = false;

    return Entity(*this, entityId);
}

bool Scene::entityIsActivated(EntityId entityId) const
{
    bool entityActivated = false;
    if (entityId < _entityData.size())
    {
        const EntityData& entityData = _entityData[entityId];
        entityActivated = entityData.activated;
    }

    return entityActivated;
}

size_t Scene::entityCount() const
{
    return _entityCount;
}

void Scene::encode(ObjectEncoder& encoder) const
{
    encoder;
}

void Scene::decode(ObjectDecoder& decoder, AssetCache& assetCache)
{
    ArrayDecoder entitiesDecoder = decoder.decodeArray("entities");
    while (entitiesDecoder.hasMoreElements())
    {
        Entity entity = createEntity();

        ObjectDecoder entityDecoder = entitiesDecoder.decodeObject();
        {
            ArrayDecoder componentsDecoder = entityDecoder.decodeArray("components");
            while (componentsDecoder.hasMoreElements())
            {
                ObjectDecoder componentDecoder = componentsDecoder.decodeObject();

                std::string componentName = componentDecoder.decodeString("type");
                std::unique_ptr<ComponentBase> component(_createComponentByName(componentName));
                component->decode(componentDecoder, assetCache);

                entity.addComponentBase(*component);
            }
        }

        entity.activate();
    }
}

EntityId Scene::_cloneEntity(EntityId entityId)
{
    EntityId clonedEntityId = createEntity().id();

    for (auto& pair : _componentPools)
    {
        pair.second->clone(entityId, clonedEntityId);
    }

    return clonedEntityId;
}

void Scene::_destroyEntity(EntityId entityId)
{
    bool entityExisted = false;
    if (entityId < _entityData.size())
    {
        EntityData& entityData = _entityData[entityId];
        if (entityData.exists)
        {
            for (auto& pair : _componentPools)
            {
                pair.second->remove(entityId);
            }
            entityExisted = true;
            entityData.exists = false;
            if (entityData.activated)
            {
                --_entityCount;
                entityData.activated = false;
            }
            _entityIdPool.destroy(entityId);
        }
    }
}

void Scene::_activateEntity(EntityId entityId)
{
    if (entityId >= _entityData.size())
    {
        throw Error("Entity does not exist");
    }

    EntityData& entityData = _entityData[entityId];
    if (!entityData.exists)
    {
        throw Error("Entity does not exist");
    }

    if (entityData.activated)
    {
        throw Error("Entity is already activated");
    }

    for (auto& pair : _componentPools)
    {
        auto componentPool = pair.second;
        if (componentPool->has(entityId))
        {
            componentPool->dispatcher().notifyEvent(ComponentPoolEvent(ComponentPoolEventType::Add, entityId));
        }
    }

    entityData.activated = true;
    ++_entityCount;
}

bool Scene::_entityExists(EntityId entityId) const
{
    bool entityExists = false;
    if (entityId < _entityData.size())
    {
        const EntityData& entityData = _entityData[entityId];
        entityExists = entityData.exists;
    }

    return entityExists;
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