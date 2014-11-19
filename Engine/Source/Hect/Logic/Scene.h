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
#include <typeindex>
#include <typeinfo>

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
/// A scene of entities, components, and systems.
class Scene :
    public Uncopyable
{
    friend class Entity;
public:

    ///
    /// Constructs an empty scene.
    ///
    /// \param engine The engine.
    Scene(Engine& engine);

    ///
    /// Returns the engine.
    Engine& engine();

    ///
    /// Returns the engine.
    const Engine& engine() const;

    ///
    /// Adds a system type to the scene that is updated discretely.
    ///
    /// \note The order in which systems are added dictates the order they tick
    /// in.
    ///
    /// \throws Error If the system type is already added to the scene.
    template <typename T>
    void addDiscreteSystem();

    ///
    /// Returns the system of a specific type.
    ///
    /// \throws Error If the scene does not have the system of the specified
    /// type.
    template <typename T>
    T& system();

    ///
    /// Returns the pool of components of a specific type.
    ///
    /// \throws Error If the component type is unknown.
    template <typename T>
    ComponentPool<T>& components();

    ///
    /// Returns the pool of components of a specific type.
    ///
    /// \throws Error If the component type is unknown.
    template <typename T>
    const ComponentPool<T>& components() const;

    ///
    /// Ticks all of the systems in the scene.
    ///
    /// \param timeStep The duration of time for the tick to simulate.
    void tick(TimeSpan timeStep);

    ///
    /// Creates a new entity.
    ///
    /// \returns An iterator to the new entity.
    Entity::Iterator createEntity();

    ///
    /// Returns the pool of entities.
    EntityPool& entities();

    ///
    /// Returns the pool of entities.
    const EntityPool& entities() const;

    ///
    /// Returns the number of active entities in the scene.
    size_t entityCount() const;

    friend Encoder& operator<<(Encoder& encoder, const Scene& scene);
    friend Decoder& operator>>(Decoder& decoder, Scene& scene);

private:
    System& addOrGetSystem(SystemTypeId typeId, bool& added);

    Entity::Iterator cloneEntity(const Entity& entity);

    void destroyEntity(Entity& entity);
    void activateEntity(Entity& entity);

    void addEntityComponentBase(Entity& entity, const ComponentBase& component);

    void encode(Encoder& encoder) const;
    void decode(Decoder& decoder);

    void encodeComponents(const Entity& entity, Encoder& encoder);
    void decodeComponents(Entity& entity, Decoder& decoder);

    Engine& _engine;

    size_t _entityCount;
    EntityPool _entityPool;

    ComponentPoolMap _componentPoolMap;

    std::vector<std::shared_ptr<System>> _systems;
    std::vector<System*> _systemTickOrder;
};

}

#include "Entity.inl"
#include "Component.inl"
#include "ComponentPool.inl"
#include "Scene.inl"
