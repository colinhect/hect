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
#include <typeindex>
#include <vector>

#include "Hect/Core/Export.h"
#include "Hect/Scene/Component.h"
#include "Hect/Scene/ComponentPool.h"

namespace hect
{

///
/// Provides the ability to dynamically create Component%s and ComponentPool%s
/// based on type information.
class HECT_EXPORT ComponentRegistry
{
    typedef std::vector<ComponentTypeId> ComponentTypeIdContainer;
public:

    ///
    /// A sequence of component type ids.
    typedef Sequence<ComponentTypeId, ComponentTypeIdContainer> ComponentTypeIdSequence;

    ///
    /// Creates a Component of the specified type.
    ///
    /// \param type_id The type id representing the type of component to create.
    ///
    /// \throws InvalidOperation If the specified type id does not correspond
    /// to a registered component type.
    static std::shared_ptr<ComponentBase> create(ComponentTypeId type_id);

    ///
    /// Creates a ComponentPool of the specified type.
    ///
    /// \param type_id The type id representing the type of component pool to
    /// create.
    /// \param scene The scene that the component pool is being created for.
    ///
    /// \throws InvalidOperation If the specified type id does not correspond
    /// to a registered component type.
    static std::shared_ptr<ComponentPoolBase> create_pool(ComponentTypeId type_id, Scene& scene);

    ///
    /// Returns the type id for the specified Component type index.
    ///
    /// \param type_index The type index of the component type to get the id of.
    ///
    /// \throws InvalidOperation If the specified type index does not
    /// correspond to a registered component type.
    static ComponentTypeId type_id_of(std::type_index type_index);

    ///
    /// Returns the type id with the specified Component type name.
    ///
    /// \param type_name The type name of the component type to get the id of.
    ///
    /// \throws InvalidOperation If the specified type name does not correspond
    /// to a registered component type.
    static ComponentTypeId type_id_of(Name type_name);

    ///
    /// Returns the Component type name with the specified type id.
    ///
    /// \param type_id The type id of the component type to get the type name of.
    static Name type_name_of(ComponentTypeId type_id);

    ///
    /// Returns whether the specified type id is registered with a
    /// Component type.
    ///
    /// \param type_id The type id to check.
    ///
    /// \returns True if the type id is a registered component type id; false
    /// otherwise.
    static bool is_registered_type_id(ComponentTypeId type_id);

    ///
    /// Returns a sequence of all of the registered type ids.
    static ComponentTypeIdSequence type_ids();

    ///
    /// Registers a Component type.
    ///
    /// \warning The type must be registered with Type.
    template <typename ComponentType>
    static void register_type();

    ///
    /// Returns the type id for the specified Component type.
    ///
    /// \throws InvalidOperation If the specified type is not a registered
    /// component type.
    template <typename ComponentType>
    static ComponentTypeId type_id_of();

private:
    ComponentRegistry() = delete;

    static std::map<ComponentTypeId, Name> _type_id_to_name;
    static std::map<Name, ComponentTypeId> _type_name_to_id;
    static std::map<std::type_index, ComponentTypeId> _type_index_to_id;
    static ComponentTypeIdContainer _type_ids;

    typedef std::function<std::shared_ptr<ComponentBase>()> ComponentConstructor;
    typedef std::function<std::shared_ptr<ComponentPoolBase>(Scene&)> ComponentPoolConstructor;

    static std::vector<ComponentConstructor> _component_constructors;
    static std::vector<ComponentPoolConstructor> _component_pool_constructors;
};

}

#include "ComponentRegistry.inl"
