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
#include "SceneRegistry.h"

#include "Hect/Core/Exception.h"
#include "Hect/Core/Format.h"

using namespace hect;

std::shared_ptr<Scene> SceneRegistry::create(SceneTypeId type_id)
{
    if (!is_registered_type_id(type_id))
    {
        throw InvalidOperation("Unknown scene type id");
    }
    return _scene_constructors[type_id]();
}

SceneTypeId SceneRegistry::type_id_of(std::type_index type_index)
{
    auto it = _type_index_to_id.find(type_index);
    if (it == _type_index_to_id.end())
    {
        throw InvalidOperation("Unknown scene type");
    }
    return it->second;
}

SceneTypeId SceneRegistry::type_id_of(Name type_name)
{
    auto it = _type_name_to_id.find(type_name);
    if (it == _type_name_to_id.end())
    {
        throw InvalidOperation(format("Unknown scene type '%s'", type_name.data()));
    }
    return it->second;
}

Name SceneRegistry::type_name_of(SceneTypeId type_id)
{
    auto it = _type_id_to_name.find(type_id);
    if (it == _type_id_to_name.end())
    {
        throw InvalidOperation("Unknown scene type id");
    }
    return _type_id_to_name[type_id];
}

bool SceneRegistry::is_registered_type_id(SceneTypeId type_id)
{
    return type_id < _scene_constructors.size();
}

std::map<SceneTypeId, Name> SceneRegistry::_type_id_to_name;
std::map<Name, SceneTypeId> SceneRegistry::_type_name_to_id;
std::map<std::type_index, SceneTypeId> SceneRegistry::_type_index_to_id;
std::vector<SceneRegistry::SceneConstructor> SceneRegistry::_scene_constructors;
