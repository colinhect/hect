///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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

#include <array>
#include <functional>
#include <typeindex>
#include <typeinfo>

#include "Hect/Core/Export.h"
#include "Hect/IO/Asset.h"
#include "Hect/IO/Encoder.h"
#include "Hect/IO/Decoder.h"
#include "Hect/Logic/Component.h"
#include "Hect/Logic/ComponentPool.h"
#include "Hect/Logic/ComponentRegistry.h"
#include "Hect/Logic/Entity.h"
#include "Hect/Logic/EntityPool.h"
#include "Hect/Logic/System.h"
#include "Hect/Logic/SystemRegistry.h"
#include "Hect/Timing/Timer.h"
#include "Hect/Timing/TimeSpan.h"

namespace hect
{

class Engine;

///
/// A dynamic collection of \link Entity Entities \endlink, Component%s, and
/// System%s.
///
/// A scene is the fundumental piece of the games state and logic.  The
/// object-specific game state is stored in entities and their components and
/// higher-level game state is stored in the systems.
class HECT_EXPORT Scene :
    public Uncopyable,
    public Asset<Scene>
{
    friend class Entity;
    template <typename T> friend class Component;
public:

    ///
    /// Constructs an empty scene.
    ///
    /// \param engine The engine.
    Scene(Engine& engine);

    ///
    /// Adds support for a System type to the scene.
    ///
    /// \note The order in which systems are added dictates the order they tick
    /// in.
    ///
    /// \warning The type must be registered with SystemRegistry.
    ///
    /// \throws InvalidOperation If the system type is already supported by the
    /// scene.
    template <typename T>
    void addSystemType();

    ///
    /// Removes support for a System type to the scene.
    ///
    /// \warning The type must be registered with SystemRegistry.
    ///
    /// \throws InvalidOperation If the system type is not supported by the
    /// scene.
    template <typename T>
    void removeSystemType();

    ///
    /// Returns whether the scene supports the specified System type.
    template <typename T>
    bool hasSystemType();

    ///
    /// Returns the System of a specific type.
    ///
    /// \throws InvalidOperation If the scene does not have the system of the
    /// specified type.
    template <typename T>
    T& system();

    ///
    /// Adds support for a Component type to the scene.
    ///
    /// \warning The type must be registered with ComponentRegistry.
    ///
    /// \throws InvalidOperation If the component type is already supported by
    /// the scene.
    template <typename T>
    void addComponentType();

    ///
    /// Returns whether the scene supports the specified Component type.
    template <typename T>
    bool hasComponentType();

    ///
    /// Returns the pool of Component%s of a specific type.
    ///
    /// \throws InvalidOperation If the component type is unknown.
    template <typename T>
    ComponentPool<T>& components();

    ///
    /// Returns the pool of Component%s of a specific type.
    ///
    /// \throws InvalidOperation If the component type is unknown.
    template <typename T>
    const ComponentPool<T>& components() const;

    ///
    /// Creates entities pending creation, activates entities pending
    /// activation, and destroys entities pending destruction.
    ///
    /// \note This is always called at the beginning of Scene::tick().
    void refresh();

    ///
    /// Ticks all of the Systems in the scene.
    ///
    /// \param timeStep The duration of time in seconds for the tick to
    /// simulate.
    void tick(Real timeStep);

    ///
    /// Creates a new Entity.
    ///
    /// \returns An iterator to the new entity.
    Entity::Iterator createEntity();

    ///
    /// Creates a new Entity decoded from a file.
    ///
    /// \param path The path to the entity.
    ///
    /// \returns An iterator to the new entity.
    ///
    /// \throws FatalError If the entity failed to decode.
    Entity::Iterator createEntity(const Path& path);

    ///
    /// Returns the pool of \link Entity Entities \endlink.
    EntityPool& entities();

    ///
    /// Returns the pool of \link Entity Entities \endlink.
    const EntityPool& entities() const;

    ///
    /// Returns the number of active \link Entity Entities \endlink in the
    /// scene.
    size_t entityCount() const;

    friend HECT_EXPORT Encoder& operator<<(Encoder& encoder, const Scene& scene);
    friend HECT_EXPORT Decoder& operator>>(Decoder& decoder, Scene& scene);

private:
    void addSystemType(SystemTypeId typeId);
    System& systemOfTypeId(SystemTypeId typeId);

    void addComponentType(ComponentTypeId typeId);
    ComponentPoolBase& componentPoolOfTypeId(ComponentTypeId typeId);

    Entity::Iterator cloneEntity(const Entity& entity);

    void destroyEntity(Entity& entity);
    void activateEntity(Entity& entity);

    void pendEntityDestruction(Entity& entity);
    void pendEntityActivation(Entity& entity);

    void addEntityComponentBase(Entity& entity, const ComponentBase& component);

    void encode(Encoder& encoder) const;
    void decode(Decoder& decoder);

    void encodeComponents(const Entity& entity, Encoder& encoder);
    void decodeComponents(Entity& entity, Decoder& decoder);

    Engine& _engine;

    size_t _entityCount { 0 };
    EntityPool _entityPool;

    bool _refreshing { false };

    std::vector<EntityId> _entitiesPendingCreation;
    std::vector<EntityId> _entitiesPendingActivation;
    std::vector<EntityId> _entitiesPendingDestruction;

    std::vector<ComponentTypeId> _componentTypeIds;
    std::vector<std::shared_ptr<ComponentPoolBase>> _componentPools;

    std::vector<SystemTypeId> _systemTypeIds;
    std::vector<std::shared_ptr<System>> _systems;
    std::array<std::vector<SystemTypeId>, 3> _tickStages;
};

}

#include "Entity.inl"
#include "Component.inl"
#include "ComponentPool.inl"
#include "Scene.inl"
