///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2016 Colin Hill
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
#include <deque>
#include <functional>
#include <typeindex>
#include <typeinfo>

#include "Hect/Core/Export.h"
#include "Hect/IO/Asset.h"
#include "Hect/Timing/Timer.h"
#include "Hect/Scene/Component.h"
#include "Hect/Scene/ComponentPool.h"
#include "Hect/Scene/ComponentRegistry.h"
#include "Hect/Scene/Entity.h"
#include "Hect/Scene/EntityPool.h"
#include "Hect/Scene/SystemBase.h"
#include "Hect/Scene/SystemRegistry.h"

namespace hect
{

class Engine;

///
/// A numeric identifier for a Scene type.
typedef uint32_t SceneTypeId;

///
/// A dynamic collection of \link Entity Entities \endlink, Component%s, and
/// System%s.
///
/// A scene is the fundamental piece of the games state and logic.  The
/// object-specific game state is stored in entities and their components and
/// higher-level game state is stored in the systems.
class HECT_EXPORT Scene :
    public Uncopyable,
    public Asset<Scene>,
    public EventListener<EntityEvent>
{
    friend class Entity;
    template <typename T> friend class Component;
public:

    ///
    /// Constructs an empty scene.
    ///
    /// \param engine The engine.
    Scene(Engine& engine);

    virtual ~Scene();

    ///
    /// Creates a System of a specific type in the scene.
    ///
    /// \warning The type must be registered with SystemRegistry.
    ///
    /// \throws InvalidOperation If a system of the same type was already
    /// created for the scene.
    template <typename T>
    T& createSystem();

    ///
    /// Returns whether the scene supports the specified System type.
    template <typename T>
    bool hasSystemType();

    ///
    /// Returns a handle to the System of a specific type.
    ///
    /// \throws InvalidOperation If the system type is not supported by the
    /// scene.
    template <typename T>
    T& system();

    ///
    /// Returns a handle to the System of a specific type.
    ///
    /// \throws InvalidOperation If the system type is not supported by the
    /// scene.
    template <typename T>
    const T& system() const;

    ///
    /// Returns whether the scene supports the specified System type id.
    ///
    /// \param typeId The type id of the system.
    bool hasSystemOfTypeId(SystemTypeId typeId);

    ///
    /// Returns a reference to the System of a specific type id.
    ///
    /// \param typeId The type id of the system.
    ///
    /// \throws InvalidOperation If the specified system type is not supported
    /// by the scene.
    SystemBase& systemOfTypeId(SystemTypeId typeId);

    ///
    /// Returns a reference to the System of a specific type id.
    ///
    /// \param typeId The type id of the system.
    ///
    /// \throws InvalidOperation If the specified system type is not supported
    /// by the scene.
    const SystemBase& systemOfTypeId(SystemTypeId typeId) const;

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
    /// Returns whether the scene is active.
    bool active() const;

    ///
    /// Sets whether the scene is active.
    ///
    /// \param active True if the scene is active; false otherwise.
    void setActive(bool active);

    ///
    /// Dispatches pending entity events, activates entities pending
    /// activation, and destroys entities pending destruction.
    ///
    /// \note This is always called at the beginning and end of Scene::tick().
    void refresh();

    ///
    /// Returns whether the scene is initialized.
    bool isInitialized() const;

    ///
    /// Perform initialization.
    ///
    /// \note This is always called after the Scene contructor and prior to
    /// the first invocation of Scene::tick().
    virtual void initialize();

    ///
    /// Ticks all of the Systems in the scene.
    ///
    /// \param timeStep The duration of time in seconds for the tick to
    /// simulate.
    virtual void tick(double timeStep) = 0;

    ///
    /// Renders all of the Systems in the scene.
    ///
    /// \param target The target to render to.
    virtual void render(RenderTarget& target) = 0;

    ///
    /// Creates a new Entity.
    ///
    /// \param name The name of the entity.
    ///
    /// \returns An iterator to the new entity.
    Entity::Iterator createEntity(Name name = Name::Unnamed);

    ///
    /// Loads an Entity from an asset.
    ///
    /// \param path The path to the entity asset.
    ///
    /// \returns An iterator to the new entity.
    ///
    /// \throws FatalError If the entity failed to decode.
    Entity::Iterator loadEntity(const Path& path);

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

    void encode(Encoder& encoder) const override;
    void decode(Decoder& decoder) override;

protected:

    ///
    /// Returns the engine.
    Engine& engine() const;

private:
    void addSystemType(SystemTypeId typeId);

    void addComponentType(ComponentTypeId typeId);
    ComponentPoolBase& componentPoolOfTypeId(ComponentTypeId typeId);

    Entity::Iterator cloneEntity(const Entity& entity);

    void destroyEntity(Entity& entity);
    void activateEntity(Entity& entity);

    void pendEntityDestruction(Entity& entity);
    void pendEntityActivation(Entity& entity);

    // Returns whether the scene has any entities pending creation, activation,
    // or destruction
    bool hasPendingEntities() const;

    void dispatchEntityCreationEvents();
    void activatePendingEntities();
    void destroyPendingEntities();

    void addEntityComponentBase(Entity& entity, const ComponentBase& component);

    void encodeComponents(const Entity& entity, Encoder& encoder);
    void decodeComponents(Entity& entity, Decoder& decoder);

    void receiveEvent(const EntityEvent& event) override;

    mutable Engine* _engine { nullptr };

    size_t _entityCount { 0 };
    EntityPool _entityPool;

    bool _initialized { false };
    bool _active { true };

    std::deque<EntityId> _entitiesPendingCreation;
    std::deque<EntityId> _entitiesPendingActivation;
    std::deque<EntityId> _entitiesPendingDestruction;

    std::vector<ComponentTypeId> _componentTypeIds;
    std::vector<std::shared_ptr<ComponentPoolBase>> _componentPools;

    std::vector<SystemTypeId> _systemTypeIds;
    std::vector<std::shared_ptr<SystemBase>> _systems;
    std::vector<SystemBase*> _systemsToInitialize;
};

}

#include "Entity.inl"
#include "Component.inl"
#include "ComponentIterator.inl"
#include "ComponentPool.inl"
#include "Scene.inl"