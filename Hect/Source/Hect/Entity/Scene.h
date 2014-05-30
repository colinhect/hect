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

#include <typeinfo>
#include <typeindex>

#include "Hect/Core/IdPool.h"
#include "Hect/IO/Encodable.h"
#include "Hect/Entity/Component.h"
#include "Hect/Entity/ComponentPool.h"

namespace hect
{

struct HECT_API EntityData
{
    EntityData();

    bool exists;
};

///
/// A scene of entities.
class HECT_API Scene :
    public Uncopyable,
    public Encodable
{
    friend class EntityIterator;
public:

    ///
    /// Constructs an empty scene.
    Scene();

    ///
    /// Creates a new entity.
    ///
    /// \returns The id of the new entity.
    EntityId createEntity();

    ///
    /// Clones an existing entity.
    ///
    /// \param entityId The id of the entity to clone.
    ///
    /// \returns The id of the cloned entity.
    ///
    /// \throws Error If the entity id does not correspond to a valid entity.
    EntityId cloneEntity(EntityId entityId);

    ///
    /// Destroys an entity.
    ///
    /// \param entityId The id of the entity to destroy.
    ///
    /// \returns True if the entity was destroy; false if the entity id does
    /// not correspond to a valid entity.
    bool destroyEntity(EntityId entityId);

    ///
    /// Returns whether an entity exists with a certain id.
    ///
    /// \param entityId The id of the entity.
    bool entityExists(EntityId entityId) const;

    ///
    /// Returns the number of entities in the scene.
    size_t entityCount() const;

    ///
    /// Adds a component to an entity.
    ///
    /// \param entityId The id of the entity.
    /// \param component The component to add.
    void addEntityComponent(EntityId entityId, const ComponentBase& component);

    ///
    /// Adds a component to an entity.
    ///
    /// \remarks If the entity already has an entity of the given type then the
    /// existing component is overwritten.
    ///
    /// \param entityId The id of the entity.
    /// \param component The component to add.
    ///
    /// \returns An iterator to the added component.
    template <typename T>
    typename ComponentPool<T>::Iterator addEntityComponent(EntityId entityId, const T& component);

    ///
    /// Removes a component from an entity.
    ///
    /// \param entityId The id of the entity.
    ///
    /// \returns True if the component was removed from the entity; false if
    /// entity did not have the component.
    template <typename T>
    bool removeEntityComponent(EntityId entityId);

    ///
    /// Returns whether an entity has a component of a given type.
    ///
    /// \param entityId The id of the entity.
    template <typename T>
    bool entityHasComponent(EntityId entityId) const;

    ///
    /// Gets the component for an entity of a given type.
    ///
    /// \param entityId The id of the entity.
    template <typename T>
    typename ComponentPool<T>::Iterator entityComponent(EntityId entityId);

    ///
    /// Registers a component type.
    ///
    /// \param componentName The type name of the component.
    template <typename T>
    void registerComponent(const std::string& componentName);

    ///
    /// Returns the component pool of a specific component type.
    template <typename T>
    ComponentPool<T>& componentPool();

    ///
    /// Returns the component pool of a specific component type.
    template <typename T>
    const ComponentPool<T>& componentPool() const;

    void encode(ObjectEncoder& encoder) const;
    void decode(ObjectDecoder& decoder, AssetCache& assetCache);

private:
    ComponentBase& _addComponentByName(EntityId entityId, const std::string& componentName);

    IdPool<EntityId> _entityIdPool;
    std::vector<EntityData> _entityData;
    size_t _entityCount;

    std::map<std::type_index, std::shared_ptr<ComponentPoolBase>> _componentPools;
    std::map<std::string, std::function<ComponentBase*(Scene&, EntityId)>> _componentAdders;
};

}

#include "Scene.inl"