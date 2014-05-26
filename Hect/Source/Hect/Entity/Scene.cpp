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

using namespace hect;

EntityData::EntityData() :
    exists(false)
{
}

Scene::Scene() :
    _entityData(4096),
    _entityCount(0)
{
}

Scene::~Scene()
{
}

EntityId Scene::createEntity()
{
    EntityId entityId = _entityIdPool.create();

    while (entityId >= _entityData.size())
    {
        _entityData.resize(_entityData.size() * 2);
    }
    _entityData[entityId].exists = true;
    ++_entityCount;

    return entityId;
}

EntityId Scene::cloneEntity(EntityId entityId)
{
    if (!entityExists(entityId))
    {
        throw Error("Attempt to clone an entity which does not exist");
    }

    EntityId clonedEntityId = createEntity();

    for (auto& pair : _componentPools)
    {
        pair.second->clone(entityId, clonedEntityId);
    }

    return clonedEntityId;
}

bool Scene::destroyEntity(EntityId entityId)
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
            _entityIdPool.destroy(entityId);
            --_entityCount;
        }
    }
    
    return entityExisted;
}

bool Scene::entityExists(EntityId entityId) const
{
    bool entityExists = false;
    if (entityId < _entityData.size())
    {
        const EntityData& entityData = _entityData[entityId];
        entityExists = entityData.exists;
    }

    return entityExists;
}

size_t Scene::entityCount() const
{
    return _entityCount;
}

void Scene::addEntityComponent(EntityId entityId, const ComponentBase& component)
{
    // Implement this
}

void Scene::encode(ObjectEncoder& encoder) const
{
    encoder;
}

void Scene::decode(ObjectDecoder& decoder, AssetCache& assetCache)
{
    decoder;
    assetCache;
}