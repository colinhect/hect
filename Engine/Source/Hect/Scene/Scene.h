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
#include "Hect/Graphics/RenderTarget.h"
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
    template <typename ComponentType> friend class Component;
public:

    ///
    /// Constructs an empty scene.
    ///
    /// \param engine The engine.
    Scene(Engine& engine);

    virtual ~Scene();

    ///
    /// Registers a system instance with the scene.
    ///
    /// \param system The system to register.
    ///
    /// \throws InvalidOperation If the a system of the same type is already
    /// registered with the scene.
    void add_system(SystemBase& system);

    ///
    /// Returns the pool of Component%s of a specific type.
    ///
    /// \throws InvalidOperation If the component type is unknown.
    template <typename ComponentType>
    ComponentPool<ComponentType>& components();

    ///
    /// Returns the pool of Component%s of a specific type.
    ///
    /// \throws InvalidOperation If the component type is unknown.
    template <typename ComponentType>
    const ComponentPool<ComponentType>& components() const;

    ///
    /// Returns whether the scene is active.
    bool active() const;

    ///
    /// Sets whether the scene is active.
    ///
    /// \param active True if the scene is active; false otherwise.
    void set_active(bool active);

    ///
    /// Dispatches pending entity events, activates entities pending
    /// activation, and destroys entities pending destruction.
    ///
    /// \note This is always called at the beginning and end of Scene::tick().
    void refresh();

    ///
    /// Returns whether the scene is initialized.
    bool is_initialized() const;

    ///
    /// Perform initialization.
    ///
    /// \note This is always called after the Scene contructor and prior to
    /// the first invocation of Scene::tick().
    virtual void initialize();

    ///
    /// Ticks all of the Systems in the scene.
    ///
    /// \param time_step The duration of time for the tick to simulate.
    virtual void tick(Seconds time_step) = 0;

    ///
    /// Renders all of the Systems in the scene.
    ///
    /// \param target The target to render to.
    virtual void render(RenderTarget& target) = 0;

    ///
    /// Creates a new Entity.
    ///
    /// \note The entity will have no effect on the scene until it is activated.
    ///
    /// \param name The name of the entity.
    ///
    /// \returns A reference to the new entity.
    Entity& create_entity(Name name = Name::Unnamed);

    ///
    /// Creates a new Entity with the specified Component%s.
    ///
    /// \note The entity will have no effect on the scene until it is activated.
    ///
    /// \param name The name of the entity.
    ///
    /// \returns A reference to the new entity.
    template <typename ...ComponentTypes>
    Entity& create_entity_with(Name name = Name::Unnamed);

    ///
    /// Loads an Entity from an asset.
    ///
    /// \note The entity will have no effect on the scene until it is activated.
    ///
    /// \param path The path to the entity asset.
    ///
    /// \returns An iterator to the new entity.
    ///
    /// \throws DecodeError If the entity failed to decode.
    Entity& load_entity(const Path& path);

    ///
    /// Clears all entities and components from the scene.
    void destroy_all_entities();

    ///
    /// Returns the pool of \link Entity Entities \endlink.
    EntityPool& entities();

    ///
    /// Returns the pool of \link Entity Entities \endlink.
    const EntityPool& entities() const;

    ///
    /// Returns the number of active \link Entity Entities \endlink in the
    /// scene.
    size_t entity_count() const;

    ///
    /// Clears the state of the scene and loads a scene from an asset.
    ///
    /// \note The scene asset must be of the same scene type as the scene.
    ///
    /// \param path The path to the scene asset.
    ///
    /// \throws DecodeError If the scene failed to decode.
    /// \throws InvalidOperation If the scene is already intialized.
    void load(const Path& path);

    void encode(Encoder& encoder) const override;
    void decode(Decoder& decoder) override;

protected:

    ///
    /// Returns the engine.
    Engine& engine() const;

private:
    void add_component_type(ComponentTypeId type_id);
    ComponentPoolBase& component_pool_of_type_id(ComponentTypeId type_id);

    Entity& clone_entity(const Entity& entity);

    void destroy_entity(Entity& entity);
    void activate_entity(Entity& entity);

    void pend_entity_destruction(Entity& entity);
    void pend_entity_activation(Entity& entity);

    // Returns whether the scene has any entities pending creation, activation,
    // or destruction
    bool has_pending_entities() const;

    void dispatch_entity_creation_events();
    void activate_pending_entities();
    void destroy_pending_entities();

    void add_entity_component_base(Entity& entity, const ComponentBase& component);

    void encode_components(const Entity& entity, Encoder& encoder);
    void decode_components(Entity& entity, Decoder& decoder);

    void receive_event(const EntityEvent& event) override;

    mutable Engine* _engine { nullptr };

    size_t _entity_count { 0 };
    EntityPool _entity_pool;

    bool _initialized { false };
    bool _active { true };

    std::deque<EntityId> _entities_pending_creation;
    std::deque<EntityId> _entities_pending_activation;
    std::deque<EntityId> _entities_pending_destruction;

    std::vector<ComponentTypeId> _component_type_ids;
    std::vector<std::shared_ptr<ComponentPoolBase>> _component_pools;

    std::vector<SystemBase*> _systems;
};

}

#include "Entity.inl"
#include "Component.inl"
#include "ComponentIterator.inl"
#include "ComponentPool.inl"
#include "Scene.inl"
