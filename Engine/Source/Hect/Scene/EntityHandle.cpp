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
#include "EntityHandle.h"

#include "Hect/Scene/EntityPool.h"
#include "Hect/Scene/Scene.h"

using namespace hect;

EntityHandle::EntityHandle()
{
}

EntityHandle::EntityHandle(Scene& scene, const Path& entity_path)
{
    *this = scene.load_entity(entity_path).handle();
}

Entity& EntityHandle::operator*()
{
    ensure_valid();
    return *_entity;
}

const Entity& EntityHandle::operator*() const
{
    ensure_valid();
    return *_entity;
}

Entity* EntityHandle::operator->()
{
    ensure_valid();
    return _entity;
}

const Entity* EntityHandle::operator->() const
{
    ensure_valid();
    return _entity;
}

bool EntityHandle::operator==(const EntityHandle& other) const
{
    return _entity == other._entity;
}

bool EntityHandle::operator!=(const EntityHandle& other) const
{
    return _entity != other._entity;
}

EntityHandle::operator bool() const
{
    return is_valid();
}

EntityHandle::EntityHandle(Entity& entity) :
    _entity(&entity),
    _valid(std::make_shared<std::atomic_bool>(true))
{
}

bool EntityHandle::is_valid() const
{
    return _entity && _valid && *_valid;
}

void EntityHandle::ensure_valid() const
{
    if (!is_valid())
    {
        throw InvalidOperation("Invalid entity handle");
    }
}

void EntityHandle::invalidate()
{
    ensure_valid();
    _valid->store(false);
}
