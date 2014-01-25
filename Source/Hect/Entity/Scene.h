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
#pragma once

#include <functional>

#include "Hect/Core/IdPool.h"
#include "Hect/Entity/Entity.h"
#include "Hect/Entity/EntityData.h"
#include "Hect/Entity/System.h"
#include "Hect/IO/Encodable.h"

namespace hect
{

///
/// A scene of entities.
class Scene :
    public Uncopyable,
    public Encodable
{
    friend class Entity;
public:

    ///
    /// Constructs a scene with no entities.
    Scene();

    ///
    /// Removes all entities from all systems.
    ~Scene();

    ///
    /// Refreshes the scene.
    ///
    /// \note All entities activated since the last call to refresh() are
    /// added to the systems that include them.  All entities destroyed since
    /// the last call to refresh are removed from the systems that include
    /// them.
    void refresh();

    ///
    /// Adds a system to the scene.
    ///
    /// \note All activated entities that are included in the system are
    /// added to the system.
    ///
    /// \param system The system to add.
    void addSystem(System& system);

    ///
    /// Removes a system from the scene.
    ///
    /// \note All entities included in the system are removed from the
    /// system.
    ///
    /// \param system The system to remove.
    void removeSystem(System& system);

    ///
    /// Creates a new entity using the next available id.
    ///
    /// \returns The new entity.
    Entity createEntity();

    ///
    /// Returns the entity of the given id.
    Entity entityWithId(Entity::Id id) const;

    ///
    /// Registers a component.
    ///
    /// \param componentTypeName The type name of the component.
    ///
    /// \throws Error If the component type is already registered.
    template <typename T>
    void registerComponent(const std::string& componentTypeName);

    void encode(ObjectEncoder& encoder) const;
    void decode(ObjectDecoder& decoder, AssetCache& assetCache);

private:
    enum
    {
        InitialPoolSize = 128
    };

    Entity _cloneEntity(const Entity& entity);

    void _encodeEntity(const Entity& entity, ObjectEncoder& encoder) const;
    void _decodeEntity(const Entity& entity, ObjectDecoder& decoder, AssetCache& assetCache);

    void _destroyEntity(const Entity& entity);
    void _activateEntity(const Entity& entity);

    bool _isActivated(const Entity& entity) const;
    bool _isNull(const Entity& entity) const;

    bool _isEncodable(const Entity& entity) const;
    void _setEncodable(const Entity& entity, bool encodable);

    template <typename T>
    bool _hasComponent(const Entity& entity) const;

    template <typename T>
    T& _addComponent(const Entity& entity, const BaseComponent::Ref& component);
    void _addComponentWithoutReturn(const Entity& entity, const BaseComponent::Ref& component);

    template <typename T>
    T& _component(const Entity& entity);

    ComponentTypeId _typeId(const std::string& typeName) const;
    const std::string& _typeName(ComponentTypeId typeId) const;
    BaseComponent* _constructComponent(ComponentTypeId typeId) const;

    // The number of activated entities
    size_t _activatedEntityCount;

    // The entity id pool
    IdPool<Entity::Id> _entityIdPool;

    // Data for each entity
    std::vector<EntityData> _entityData;

    // Components for each entity
    std::vector<std::map<ComponentTypeId, BaseComponent::Ref>> _entityComponents;

    // Entities activated since the last call to refresh()
    std::vector<Entity::Id> _activatedEntities;

    // Entities destroyed since the last call to refresh()
    std::vector<Entity::Id> _destroyedEntities;

    // Systems involved in the scene
    std::vector<System*> _systems;

    // Component types mapped to component type names
    std::map<ComponentTypeId, std::string> _componentTypeNames;

    // Component type names mapped to component types
    std::map<std::string, ComponentTypeId> _componentTypeIds;

    // Component types mapped to component constructors
    std::map<ComponentTypeId, std::function<BaseComponent*()>> _componentConstructors;
};

}

#include "Scene.inl"