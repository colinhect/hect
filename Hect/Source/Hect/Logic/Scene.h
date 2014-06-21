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

#include "Hect/IO/Encodable.h"
#include "Hect/Logic/ComponentPool.h"
#include "Hect/Logic/EntityPool.h"

namespace hect
{

class Scene :
    public Uncopyable,
    public Encodable
{
public:
    Scene();

    Entity::Iter createEntity(const std::string& name = std::string());
    
    Entity::Iter cloneEntity(const Entity& entity, const std::string& name = std::string());

    void destroyEntity(Entity& entity);
    void activateEntity(Entity& entity);

    void addEntityComponentBase(Entity& entity, const ComponentBase& component);

    void encodeComponents(const Entity& entity, ObjectEncoder& encoder);
    void decodeComponents(Entity& entity, ObjectDecoder& decoder, AssetCache& assetCache);

    ///
    /// Registers a component type.
    ///
    /// \param componentName The type name of the component.
    template <typename T>
    void registerComponent(const std::string& componentName);

    template <typename T>
    ComponentPool<T>& components();

    template <typename T>
    const ComponentPool<T>& components() const;

    EntityPool& entities();
    const EntityPool& entities() const;

    size_t entityCount() const;
    
    void encode(ObjectEncoder& encoder) const;
    void decode(ObjectDecoder& decoder, AssetCache& assetCache);

private:
    ComponentBase* _createComponentByName(const std::string& componentName);

    size_t _entityCount;
    EntityPool _entityPool;

    std::map<std::type_index, std::shared_ptr<ComponentPoolBase>> _componentPools;
    std::map<std::type_index, std::string> _componentTypeNames;
    std::map<std::string, std::function<ComponentBase*(void)>> _componentConstructors;
};

}

#include "Scene.inl"