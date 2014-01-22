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

#include "Hect/Entity/Components/AmbientLight.h"
#include "Hect/Entity/Components/Camera.h"
#include "Hect/Entity/Components/DirectionalLight.h"
#include "Hect/Entity/Components/Geometry.h"
#include "Hect/Entity/Components/Transform.h"
#include "Hect/Entity/Components/RigidBody.h"

using namespace hect;

Scene::Scene() :
    _activatedEntityCount(0),
    _entityData(InitialPoolSize),
    _entityComponents(InitialPoolSize)
{
    registerComponent<AmbientLight>("AmbientLight");
    registerComponent<Camera>("Camera");
    registerComponent<DirectionalLight>("DirectionalLight");
    registerComponent<Geometry>("Geometry");
    registerComponent<Transform>("Transform");
    registerComponent<RigidBody>("RigidBody");
}

Scene::~Scene()
{
    // All entities in the scene will be invalidated when the scene is
    // destroyed, so we remove the entities from all of the scenes first
    for (System* system : _systems)
    {
        system->removeAllEntities();
    }
}

void Scene::refresh()
{
    // Add all activated entities to systems that include them
    for (Entity::Id id : _activatedEntities)
    {
        Entity entity = entityWithId(id);
        for (System* system : _systems)
        {
            if (system->includesEntity(entity))
            {
                system->addEntity(entity);
            }
        }

        // Maintain the activated entity count
        ++_activatedEntityCount;
    }
    _activatedEntities.clear();

    // Remove all destroyed entities from systems that include them
    for (Entity::Id id : _destroyedEntities)
    {
        Entity entity = entityWithId(id);
        for (System* system : _systems)
        {
            if (system->includesEntity(entity))
            {
                system->removeEntity(entity);
            }
        }

        // Maintain the activated entity count
        if (entity.isActivated())
        {
            --_activatedEntityCount;
        }

        // Clear entity data
        _entityComponents[id].clear();
        _entityData[id] = EntityData();

        // Re-use this id
        _entityIdPool.destroy(id);
    }
    _destroyedEntities.clear();
}

void Scene::addSystem(System& system)
{
    // Add the system to the scene
    _systems.push_back(&system);

    // Add any entities the systems is filtered for
    for (Entity::Id id = 0; id < _entityData.size(); ++id)
    {
        Entity entity = entityWithId(id);
        if (entity && entity.isActivated() && system.includesEntity(entity))
        {
            system.addEntity(entity);
        }
    }
}

void Scene::removeSystem(System& system)
{
    // Remove all entities from the system
    system.removeAllEntities();

    // Remove the system from the scene
    _systems.erase(std::remove(_systems.begin(), _systems.end(), &system), _systems.end());
}

Entity Scene::createEntity()
{
    Entity::Id id = _entityIdPool.create();

    // Resize the pool if needed
    if (id >= _entityComponents.size())
    {
        size_t size = _entityComponents.size() * 2;
        _entityData.resize(size);
        _entityComponents.resize(size);
    }

    // Set the entity as no longer being null; the rest of the data is
    // properly initialized
    _entityData[id].setNull(false);
    return Entity(*this, id);
}

Entity Scene::entityWithId(Entity::Id id) const
{
    if (id >= _entityData.size())
    {
        return Entity();
    }
    return Entity(*const_cast<Scene*>(this), id);
}

void Scene::save(ObjectWriter& writer) const
{
    ArrayWriter entities = writer.writeArray("entities");

    for (Entity::Id id = 0; id < _entityData.size(); ++id)
    {
        Entity entity = entityWithId(id);
        if (entity && entity.isActivated() && entity.isSerializable())
        {
            ObjectWriter entityWriter = entities.writeObject();
            entity.save(entityWriter);
        }
    }
}

void Scene::load(ObjectReader& reader, AssetCache& assetCache)
{
    ArrayReader entities = reader.readArray("entities");
    while (entities.hasMoreElements())
    {
        ObjectReader entityReader = entities.readObject();
        Entity entity = createEntity();
        entity.load(entityReader, assetCache);
        entity.activate();
    }
}

Entity Scene::_cloneEntity(const Entity& entity)
{
    // Create the cloned entity
    Entity clone = createEntity();

    // For each component in the source entity
    auto& components = _entityComponents[entity._id];
    for (auto& pair : components)
    {
        BaseComponent* component = pair.second.get();

        // Clone the component
        BaseComponent::Ref clonedComponent(component->clone());

        // Add the clone component to the cloned entity
        _addComponentWithoutReturn(clone, clonedComponent);
    }

    return clone;
}

void Scene::_serializeEntity(const Entity& entity, ObjectWriter& writer) const
{
    if (!entity)
    {
        throw Error("Entity is null");
    }

    ArrayWriter components = writer.writeArray("components");

    for (BaseComponent* component : entity.components())
    {
        ComponentTypeId typeId = component->componentTypeId();
        const std::string& typeName = _typeName(typeId);

        ObjectWriter componentWriter = components.writeObject();
        componentWriter.writeString("type", typeName);
        component->save(componentWriter);
    }
}

void Scene::_deserializeEntity(const Entity& entity, ObjectReader& reader, AssetCache& assetCache)
{
    if (!entity)
    {
        throw Error("Entity is null");
    }

    ArrayReader components = reader.readArray("components");
    while (components.hasMoreElements())
    {
        ObjectReader componentReader = components.readObject();

        std::string typeName = componentReader.readString("type");
        ComponentTypeId typeId = _typeId(typeName);
        BaseComponent* component = _constructComponent(typeId);

        component->load(componentReader, assetCache);
        entity.addComponent(component);
    }
}

void Scene::_destroyEntity(const Entity& entity)
{
    Entity::Id id = entity._id;
    EntityData& data = _entityData[id];

    if (data.isNull())
    {
        throw Error("Entity is null");
    }
    else if (data.isDestroyed())
    {
        throw Error("Entity is already destroyed");
    }

    // Set the entity as destroyed and enqueue it to be removed from the
    // systems
    data.setDestroyed(true);
    _destroyedEntities.push_back(id);
}

void Scene::_activateEntity(const Entity& entity)
{
    Entity::Id id = entity._id;
    EntityData& data = _entityData[id];

    if (data.isNull())
    {
        throw Error("Entity is null");
    }
    else if (data.isActivated())
    {
        throw Error("Entity is already activated");
    }

    // Set the entity as activated and enqueue it to be removed from the
    // systems
    data.setActivated(true);
    _activatedEntities.push_back(id);
}

bool Scene::_isActivated(const Entity& entity) const
{
    // Check the entity data to see if the entity is activated
    Entity::Id id = entity._id;
    const EntityData& data = _entityData[id];

    if (data.isNull())
    {
        throw Error("Entity is null");
    }

    return data.isActivated();
}

bool Scene::_isNull(const Entity& entity) const
{
    // Check the entity data to see if the entity is null
    Entity::Id id = entity._id;
    const EntityData& data = _entityData[id];
    return data.isNull();
}

bool Scene::_isSerializable(const Entity& entity) const
{
    // Check the entity data to see if the entity is serializable
    Entity::Id id = entity._id;
    const EntityData& data = _entityData[id];

    if (data.isNull())
    {
        throw Error("Entity is null");
    }

    return data.isSerializable();
}

void Scene::_setSerializable(const Entity& entity, bool serializable)
{
    Entity::Id id = entity._id;
    EntityData& data = _entityData[id];

    if (data.isNull())
    {
        throw Error("Entity is null");
    }

    data.setSerializable(serializable);
}

void Scene::_addComponentWithoutReturn(const Entity& entity, const BaseComponent::Ref& component)
{
    Entity::Id id = entity._id;
    EntityData& data = _entityData[id];

    ComponentTypeId typeId = component->componentTypeId();

    if (data.isNull())
    {
        throw Error("Entity is null");
    }
    else if (data.isActivated())
    {
        throw Error("Entity is activated");
    }
    else if (data.hasComponent(typeId))
    {
        throw Error(format("Entity already has a component with type id '%d'", typeId));
    }

    // Set the flag in the entity data to include the component type
    data.setHasComponent(typeId, true);
    _entityComponents[id][typeId] = component;
}

ComponentTypeId Scene::_typeId(const std::string& typeName) const
{
    auto it = _componentTypeIds.find(typeName);
    if (it == _componentTypeIds.end())
    {
        throw Error(format("No writer registered for component type name '%s'", typeName.c_str()));
    }
    return (*it).second;
}

const std::string& Scene::_typeName(ComponentTypeId typeId) const
{
    auto it = _componentTypeNames.find(typeId);
    if (it == _componentTypeNames.end())
    {
        throw Error(format("No writer registered for component type id '%d'", typeId));
    }
    return (*it).second;
}

BaseComponent* Scene::_constructComponent(ComponentTypeId typeId) const
{
    auto it = _componentConstructors.find(typeId);
    if (it == _componentConstructors.end())
    {
        throw Error(format("No writer registered for component type id '%d'", typeId));
    }
    return (*it).second();
}