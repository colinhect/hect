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

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <typeindex>
#include <vector>

#include "Hect/Core/Export.h"
#include "Hect/Logic/Component.h"
#include "Hect/Logic/ComponentPool.h"

namespace hect
{

///
/// Provides the ability to dynamically create Component%s and ComponentPool%s
/// based on type information.
class HECT_EXPORT ComponentRegistry
{
public:

    ///
    /// Creates a Component of the specified type.
    ///
    /// \param typeId The type id representing the type of component to create.
    ///
    /// \throws InvalidOperation If the specified type id does not correspond
    /// to a registered component type.
    static std::shared_ptr<ComponentBase> create(ComponentTypeId typeId);

    ///
    /// Creates a ComponentPool of the specified type.
    ///
    /// \param typeId The type id representing the type of component pool to
    /// create.
    /// \param scene The scene that the component pool is being created for.
    ///
    /// \throws InvalidOperation If the specified type id does not correspond
    /// to a registered component type.
    static std::shared_ptr<ComponentPoolBase> createPool(ComponentTypeId typeId, Scene& scene);

    ///
    /// Returns the type id for the specified Component type index.
    ///
    /// \param typeIndex The type index of the component type to get the id of.
    ///
    /// \throws InvalidOperation If the specified type index does not
    /// correspond to a registered component type.
    static ComponentTypeId typeIdOf(std::type_index typeIndex);

    ///
    /// Returns the type id with the specified Component type name.
    ///
    /// \param typeName The type name of the component type to get the id of.
    ///
    /// \throws InvalidOperation If the specified type name does not correspond
    /// to a registered component type.
    static ComponentTypeId typeIdOf(const std::string& typeName);

    ///
    /// Returns the Component type name with the specified type id.
    ///
    /// \param typeId The type id of the component type to get the type name of.
    static const std::string& typeNameOf(ComponentTypeId typeId);

    ///
    /// Returns whether the specified type id is registered with a
    /// Component type.
    ///
    /// \param typeId The type id to check.
    ///
    /// \returns True if the type id is a registered component type id; false
    /// otherwise.
    static bool isRegisteredTypeId(ComponentTypeId typeId);

    ///
    /// Registers a Component type.
    ///
    /// \warning The type must be registered with Type.
    template <typename T>
    static void registerType();

    ///
    /// Returns the type id for the specified Component type.
    ///
    /// \throws InvalidOperation If the specified type is not a registered
    /// component type.
    template <typename T>
    static ComponentTypeId typeIdOf();

private:
    ComponentRegistry();

    static std::map<ComponentTypeId, std::string> _typeIdToName;
    static std::map<std::string, ComponentTypeId> _typeNameToId;
    static std::map<std::type_index, ComponentTypeId> _typeIndexToId;

    typedef std::function<std::shared_ptr<ComponentBase>(void)> ComponentConstructor;
    typedef std::function<std::shared_ptr<ComponentPoolBase>(Scene&)> ComponentPoolConstructor;

    static std::vector<ComponentConstructor> _componentConstructors;
    static std::vector<ComponentPoolConstructor> _componentPoolConstructors;
};

}

#include "ComponentRegistry.inl"