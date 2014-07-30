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
#include <typeinfo>
#include <typeindex>

#include "Hect/Core/Engine.h"
#include "Hect/IO/Encodable.h"
#include "Hect/Logic/ComponentPool.h"
#include "Hect/Logic/EntityPool.h"

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
    /// A function which registers a collection of components to a scene.
    typedef void (*ComponentRegistration)(Scene& scene);

    ///
    /// Constructs an empty scene.
    Scene();

    ///
    /// Constructs an empty scene.
    ///
    /// \param engine The engine.
    /// \param componentRegisteration The component registration function.
    Scene(Engine& engine, ComponentRegistration componentRegisteration);

    ///
    /// Creates a new entity.
    ///
    /// \returns An iterator to the new entity.
    Entity::Iter createEntity();

    ///
    /// Creates a new entity from an asset.
    ///
    /// \param entityPath The path to the entity asset.
    /// \param assetCache The asset cache.
    ///
    /// \returns An iterator to the new entity.
    Entity::Iter createEntity(const Path& entityPath, AssetCache& assetCache);

    ///
    /// Registers a component type.
    ///
    /// \param componentName The type name of the component.
    template <typename T>
    void registerComponent(const std::string& componentName);

    ///
    /// Returns the pool of components of a specific type.
    template <typename T>
    ComponentPool<T>& components();

    ///
    /// Returns the pool of components of a specific type.
    template <typename T>
    const ComponentPool<T>& components() const;

    ///
    /// Returns the pool of entities.
    EntityPool& entities();

    ///
    /// Returns the pool of entities.
    const EntityPool& entities() const;

    ///
    /// Returns the number of active entities in the scene.
    size_t entityCount() const;

    FileSystem& fileSystem();
    InputSystem& inputSystem();

    Window& window();
    Renderer& renderer();

    AssetCache& assetCache();

    void encode(ObjectEncoder& encoder) const;
    void decode(ObjectDecoder& decoder, AssetCache& assetCache);

private:
    Entity::Iter _cloneEntity(const Entity& entity);

    void _destroyEntity(Entity& entity);
    void _activateEntity(Entity& entity);

    void _addEntityComponentBase(Entity& entity, const ComponentBase& component);

    void _encodeComponents(const Entity& entity, ObjectEncoder& encoder);
    void _decodeComponents(Entity& entity, ObjectDecoder& decoder, AssetCache& assetCache);
    
    Engine* _engine;

    size_t _entityCount;
    EntityPool _entityPool;

    std::map<std::type_index, std::shared_ptr<ComponentPoolBase>> _componentPools;

    std::map<std::type_index, std::string> _componentTypeNames;
    std::map<std::string, std::function<ComponentBase*(void)>> _componentConstructors;
};

}

#include "Scene.inl"