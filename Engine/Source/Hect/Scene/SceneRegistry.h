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
#include "Hect/Scene/Scene.h"

namespace hect
{

///
/// Provides the ability to dynamically create Scene%s based on type
/// information.
class HECT_EXPORT SceneRegistry
{
public:

    ///
    /// Creates a Scene of the specified type.
    ///
    /// \param type_id The type id representing the type of scene to create.
    ///
    /// \throws InvalidOperation If the specified type id does not correspond
    /// to a registered scene type.
    static std::shared_ptr<Scene> create(SceneTypeId type_id);

    ///
    /// Returns the type id for the specified Scene type index.
    ///
    /// \param type_index The type index of the scene type to get the id of.
    ///
    /// \throws InvalidOperation If the specified type index does not
    /// correspond to a registered scene type.
    static SceneTypeId type_id_of(std::type_index type_index);

    ///
    /// Returns the type id with the specified Scene type name.
    ///
    /// \param type_name The type name of the scene type to get the id of.
    ///
    /// \throws InvalidOperation If the specified type name does not correspond
    /// to a registered scene type.
    static SceneTypeId type_id_of(Name type_name);

    ///
    /// Returns the Scene type name with the specified type id.
    ///
    /// \param type_id The type id of the scene type to get the type name of.
    static Name type_name_of(SceneTypeId type_id);

    ///
    /// Returns whether the specified type id is registered with a
    /// Scene type.
    ///
    /// \param type_id The type id to check.
    ///
    /// \returns True if the type id is a registered scene type id; false
    /// otherwise.
    static bool is_registered_type_id(SceneTypeId type_id);

    ///
    /// Registers a Scene type.
    ///
    /// \warning The type must be registered with Type.
    template <typename T>
    static void register_type();

    ///
    /// Returns the type id for the specified Scene type.
    ///
    /// \throws InvalidOperation If the specified type is not a registered
    /// scene type.
    template <typename T>
    static SceneTypeId type_id_of();

private:
    SceneRegistry() = delete;

    static std::map<SceneTypeId, Name> _type_id_to_name;
    static std::map<Name, SceneTypeId> _type_name_to_id;
    static std::map<std::type_index, SceneTypeId> _type_index_to_id;

    typedef std::function<std::shared_ptr<Scene>()> SceneConstructor;

    static std::vector<SceneConstructor> _scene_constructors;
};

}

#include "SceneRegistry.inl"
