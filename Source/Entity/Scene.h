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

namespace hect
{

///
/// A scene of entities.
class Scene :
    public Uncopyable
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
    /// \remarks All entities activated since the last call to refresh() are
    /// added to the systems that include them.  All entities destroyed since
    /// the last call to refresh are removed from the systems that include
    /// them.
    void refresh();

    ///
    /// Adds a system to the scene.
    ///
    /// \remarks All activated entities that are included in the system are
    /// added to the system.
    ///
    /// \param system The system to add.
    void addSystem(System& system);

    ///
    /// Removes a system from the scene.
    ///
    /// \remarks All entities included in the system are removed from the
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
    /// Serializes all activated entities in the scene to a data value.
    ///
    /// \param dataValue The data value.
    void save(DataValue& dataValue) const;

    ///
    /// Serializes all activated entities in the scene to a binary stream.
    ///
    /// \param stream The stream to write to.
    void save(WriteStream& stream) const;

    ///
    /// Deserializes all entities from a data value and activates them in the
    /// scene.
    ///
    /// \param dataValue The data value.
    /// \param assetCache The asset cache to use to load referenced assets.
    void load(const DataValue& dataValue, AssetCache& assetCache);

    ///
    /// Deserializes all entities from a binary stream and activates them in
    /// the scene.
    ///
    /// \param stream The stream to read from.
    /// \param assetCache The asset cache to use to load referenced assets.
    void load(ReadStream& stream, AssetCache& assetCache);
    
    ///
    /// Returns the entity serializer;
    EntitySerializer& entitySerializer();

private:
    enum
    {
        InitialPoolSize = 128
    };

    Entity _cloneEntity(const Entity& entity);

    void _destroyEntity(const Entity& entity);
    void _activateEntity(const Entity& entity);

    bool _isActivated(const Entity& entity) const;
    bool _isNull(const Entity& entity) const;

    bool _isSerializable(const Entity& entity) const;
    void _setSerializable(const Entity& entity, bool serializable);

    template <typename T>
    bool _hasComponent(const Entity& entity) const;

    template <typename T>
    T& _addComponent(const Entity& entity, const BaseComponent::Ref& component);
    void _addComponentWithoutReturn(const Entity& entity, const BaseComponent::Ref& component);

    template <typename T>
    T& _component(const Entity& entity);

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

    // The entity serializer
    EntitySerializer _entitySerializer;
};

}

#include "Scene.inl"