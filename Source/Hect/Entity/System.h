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

#include "Hect/Entity/Entity.h"

namespace hect
{

///
/// A system which handles entities with specific components.
class System
{
public:

    ///
    /// Adds an entity to the system.
    ///
    /// \param entity The entity.
    virtual void addEntity(const Entity& entity);

    ///
    /// Removes an entity from the system.
    ///
    /// \param entity The entity.
    virtual void removeEntity(const Entity& entity);

    ///
    /// Removes all from the system.
    void removeAllEntities();

    ///
    /// Returns whether the system includes the given entity.
    ///
    /// \remarks If this returns false for an entity, then addEntity() will
    /// not even be called for that entity.
    ///
    /// \param entity The entity to check if the system includes.
    virtual bool includesEntity(const Entity& entity) const;

    ///
    /// Returns the entities currently in the system.
    Entity::Array& entities();

    ///
    /// Returns the entities currently in the system.
    const Entity::Array& entities() const;

private:
    Entity::Array _entities;
};

}