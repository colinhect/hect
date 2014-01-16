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
#include "Hect.h"

using namespace hect;

Entity::Entity() :
    _scene(nullptr),
    _id(0)
{
}

Scene& Entity::scene() const
{
    if (!_scene)
    {
        throw Error("Entity is null");
    }

    return *_scene;
}

Entity::Id Entity::id() const
{
    return _id;
}

void Entity::save(DataValue& dataValue) const
{
    if (!_scene)
    {
        throw Error("Entity is null");
    }

    _scene->entitySerializer().save(*this, dataValue);
}

void Entity::save(WriteStream& stream) const
{
    if (!_scene)
    {
        throw Error("Entity is null");
    }

    _scene->entitySerializer().save(*this, stream);
}

void Entity::load(const DataValue& dataValue, AssetCache& assetCache) const
{
    if (!_scene)
    {
        throw Error("Entity is null");
    }
    else if (isActivated())
    {
        throw Error("Entity is activated");
    }

    _scene->entitySerializer().load(*this, dataValue, assetCache);
}

void Entity::load(ReadStream& stream, AssetCache& assetCache) const
{
    if (!_scene)
    {
        throw Error("Entity is null");
    }
    else if (isActivated())
    {
        throw Error("Entity is activated");
    }

    _scene->entitySerializer().load(*this, stream, assetCache);
}

void Entity::destroy() const
{
    if (!_scene)
    {
        throw Error("Entity is null");
    }

    _scene->_destroyEntity(*this);
}

Entity Entity::clone() const
{
    if (!_scene)
    {
        throw Error("Entity is null");
    }

    return _scene->_cloneEntity(*this);
}

void Entity::activate() const
{
    if (!_scene)
    {
        throw Error("Entity is null");
    }

    _scene->_activateEntity(*this);
}

bool Entity::isActivated() const
{
    if (!_scene)
    {
        throw Error("Entity is null");
    }

    return _scene->_isActivated(*this);
}

bool Entity::isNull() const
{
    return !_scene || _scene->_isNull(*this);
}

bool Entity::isSerializable() const
{
    if (!_scene)
    {
        throw Error("Entity is null");
    }

    return _scene->_isSerializable(*this);
}

void Entity::setSerializable(bool serializable) const
{
    if (!_scene)
    {
        throw Error("Entity is null");
    }

    _scene->_setSerializable(*this, serializable);
}

void Entity::addComponent(BaseComponent* component) const
{
    if (!_scene)
    {
        throw Error("Entity is null");
    }

    _scene->_addComponentWithoutReturn(*this, BaseComponent::Ref(component));
}

std::vector<BaseComponent*> Entity::components() const
{
    if (!_scene)
    {
        throw Error("Entity is null");
    }

    std::vector<BaseComponent*> resultingComponents;
    auto& components = _scene->_entityComponents[_id];
    for (auto& pair : components)
    {
        resultingComponents.push_back(pair.second.get());
    }

    return resultingComponents;
}

Entity::operator bool() const
{
    return !isNull();
}

bool Entity::operator==(const Entity& entity) const
{
    return _scene == entity._scene && _id == entity._id;
}

bool Entity::operator!=(const Entity& entity) const
{
    return _scene != entity._scene || _id != entity._id;
}

Entity::Entity(Scene& scene, Id id) :
    _scene(&scene),
    _id(id)
{
}