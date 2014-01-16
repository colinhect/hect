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

class Scene;

///
/// Provides functionality for registering component serializers to a component
/// type and for serializing/deserializing entities.
class EntitySerializer
{
public:

    ///
    /// Constructs an entity serializer with all built-in components registered
    /// with their serializers.
    EntitySerializer();

    ///
    /// Serializes an entity's components to a data value.
    ///
    /// \param entity The entity.
    /// \param dataValue The data value.
    ///
    /// \throws Error If the entity is null.
    void save(const Entity& entity, DataValue& dataValue);

    ///
    /// Serializes an entity's components to a binary stream.
    ///
    /// \param entity The entity.
    /// \param stream The stream to write to.
    ///
    /// \throws Error If the entity is null.
    void save(const Entity& entity, WriteStream& stream);

    ///
    /// Deserializes and adds components to an entity from a data value.
    ///
    /// \param entity The entity.
    /// \param dataValue The data value.
    /// \param assetCache The asset cache to use to load referenced assets.
    ///
    /// \throws Error If the entity is null or activated.
    void load(const Entity& entity, const DataValue& dataValue, AssetCache& assetCache);

    ///
    /// Deserializes and adds components to an entity from a binary stream.
    ///
    /// \param entity The entity.
    /// \param stream The stream to read from.
    /// \param assetCache The asset cache to use to load referenced assets.
    ///
    /// \throws Error If the entity is null or activated.
    void load(const Entity& entity, ReadStream& stream, AssetCache& assetCache);

    ///
    /// Registers a component with its serializer.
    ///
    /// \param componentTypeName The type name of the component.
    ///
    /// \throws Error If the component type is already registered.
    template <typename T, typename S>
    void registerComponent(const std::string& componentTypeName);

private:
    ComponentTypeId _typeId(const std::string& typeName) const;
    const std::string& _typeName(ComponentTypeId typeId) const;
    const BaseComponentSerializer& _serializer(ComponentTypeId typeId) const;
    BaseComponent* _constructComponent(ComponentTypeId typeId) const;

    // Component types mapped to component type names
    std::map<ComponentTypeId, std::string> _componentTypeNames;

    // Component type names mapped to component types
    std::map<std::string, ComponentTypeId> _componentTypeIds;

    // Component types mapped to registered component serializers
    std::map<ComponentTypeId, BaseComponentSerializer::Ref> _componentSerializers;

    // Component types mapped to component constructors
    std::map<ComponentTypeId, std::function<BaseComponent*()>> _componentConstructors;
};

}

#include "EntitySerializer.inl"