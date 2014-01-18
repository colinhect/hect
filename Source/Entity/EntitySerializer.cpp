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
#include "EntitySerializer.h"

#include "Entity/Entity.h"
#include "Entity/Components/AmbientLight.h"
#include "Entity/Components/Camera.h"
#include "Entity/Components/DirectionalLight.h"
#include "Entity/Components/Geometry.h"
#include "Entity/Components/Transform.h"
#include "Entity/Components/RigidBody.h"

using namespace hect;

EntitySerializer::EntitySerializer()
{
    // Register all hect components
    registerComponent<AmbientLight>("AmbientLight");
    registerComponent<Camera>("Camera");
    registerComponent<DirectionalLight>("DirectionalLight");
    registerComponent<Geometry>("Geometry");
    registerComponent<Transform>("Transform");
    registerComponent<RigidBody>("RigidBody");
}

void EntitySerializer::save(const Entity& entity, DataValue& dataValue)
{
    if (!entity)
    {
        throw Error("Entity is null");
    }

    dataValue = DataValue(DataValueType::Object);

    // For each component in the entity
    for (BaseComponent* component : entity.components())
    {
        ComponentTypeId typeId = component->componentTypeId();
        const std::string& typeName = _typeName(typeId);

        // Serialize
        DataValue componentDataValue;
        component->serialize(componentDataValue);

        // Save the resulting data value from the writer to the member data
        // value
        dataValue.addMember(typeName, componentDataValue);
    }
}

void EntitySerializer::save(const Entity& entity, WriteStream& stream)
{
    if (!entity)
    {
        throw Error("Entity is null");
    }

    // Write the number of components as a byte
    std::vector<BaseComponent*> components = entity.components();
    stream.writeByte((uint8_t)components.size());

    // For each component in the entity
    for (BaseComponent* component : components)
    {
        ComponentTypeId typeId = component->componentTypeId();

        // Write the type id as a byte
        stream.writeByte((uint8_t)typeId);

        // Serialize
        component->serialize(stream);
    }
}

void EntitySerializer::load(const Entity& entity, const DataValue& dataValue, AssetCache& assetCache)
{
    if (!entity)
    {
        throw Error("Entity is null");
    }

    // For each component type name
    for (const std::string& typeName : dataValue.memberNames())
    {
        ComponentTypeId typeId = _typeId(typeName);

        // Create component
        BaseComponent* component = _constructComponent(typeId);

        // Deserialize
        component->deserialize(dataValue[typeName], assetCache);
        entity.addComponent(component);
    }
}

void EntitySerializer::load(const Entity& entity, ReadStream& stream, AssetCache& assetCache)
{
    if (!entity)
    {
        throw Error("Entity is null");
    }

    // Read the number of components
    uint8_t componentCount = stream.readByte();

    // For each component
    for (uint8_t i = 0; i < componentCount; ++i)
    {
        ComponentTypeId typeId = stream.readByte();

        // Create component
        BaseComponent* component = _constructComponent(typeId);

        // Deserialize
        component->deserialize(stream, assetCache);
        entity.addComponent(component);
    }
}

ComponentTypeId EntitySerializer::_typeId(const std::string& typeName) const
{
    auto it = _componentTypeIds.find(typeName);
    if (it == _componentTypeIds.end())
    {
        throw Error(format("No serializer registered for component type name '%s'", typeName.c_str()));
    }
    return (*it).second;
}

const std::string& EntitySerializer::_typeName(ComponentTypeId typeId) const
{
    auto it = _componentTypeNames.find(typeId);
    if (it == _componentTypeNames.end())
    {
        throw Error(format("No serializer registered for component type id '%d'", typeId));
    }
    return (*it).second;
}

BaseComponent* EntitySerializer::_constructComponent(ComponentTypeId typeId) const
{
    auto it = _componentConstructors.find(typeId);
    if (it == _componentConstructors.end())
    {
        throw Error(format("No serializer registered for component type id '%d'", typeId));
    }
    return (*it).second();
}