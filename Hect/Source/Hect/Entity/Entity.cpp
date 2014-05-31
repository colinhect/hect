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
#include "Entity.h"

#include "Hect/Entity/Scene.h"

using namespace hect;

Entity::Entity() :
    _scene(nullptr),
    _id((EntityId)-1)
{
}

Entity::Entity(Scene& scene, EntityId id) :
_scene(&scene),
_id(id)
{
}

void Entity::addComponentBase(const ComponentBase& component)
{
    _ensureExists();

    std::type_index typeIndex = component.typeIndex();
    auto it = _scene->_componentPools.find(typeIndex);
    if (it != _scene->_componentPools.end())
    {
        it->second->add(_id, component);
    }
}

Entity Entity::clone() const
{
    _ensureExists();
    EntityId clonedEntityId = _scene->_cloneEntity(_id);
    return Entity(*_scene, clonedEntityId);
}

void Entity::destroy()
{
    _ensureExists();
    _scene->_destroyEntity(_id);
}

bool Entity::exists() const
{
    if (_scene)
    {
        return _scene->_entityExists(_id);
    }

    return false;
}

Scene& Entity::scene()
{
    _ensureExists();
    return *_scene;    
}

const Scene& Entity::scene() const
{
    _ensureExists();
    return *_scene;
}

EntityId Entity::id() const
{
    return _id;
}

bool Entity::operator == (const Entity& entity) const
{
    return _scene == entity._scene && _id == entity._id;
}

bool Entity::operator!=(const Entity& entity) const
{
    return _scene != entity._scene || _id != entity._id;
}

Entity::operator bool() const
{
    return exists();
}

void Entity::_ensureExists() const
{
    if (!exists())
    {
        throw Error("Entity does not exist");
    }
}