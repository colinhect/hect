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

#include <memory>

#include "Hect/Core/EventListener.h"
#include "Hect/Core/Export.h"
#include "Hect/Scene/EntityEvent.h"
#include "Hect/Scene/EntityIterator.h"

namespace hect
{

///
/// A weak reference to an Entity.
class HECT_EXPORT EntityHandle
{
    friend class Entity;
public:

    ///
    /// Constructs an invalid handle.
    EntityHandle();

    ///
    /// Contructs a handle from an iterator.
    ///
    /// \param iterator The iterator.
    EntityHandle(const EntityIterator& iterator);

    ///
    /// Contructs a handle copied from another.
    ///
    /// \param handle The handle to copy.
    EntityHandle(const EntityHandle& handle);

    ///
    /// Contructs a handle moved from another.
    ///
    /// \param handle The handle to move.
    EntityHandle(EntityHandle&& handle);

    ///
    /// Dereferences the handle to a reference to the entity.
    ///
    /// \returns A reference to the entity.
    ///
    /// \throws InvalidOperation If the handle is invalid.
    Entity& operator*();

    ///
    /// Dereferences the handle to a reference to the entity.
    ///
    /// \returns A reference to the entity.
    ///
    /// \throws InvalidOperation If the handle is invalid.
    const Entity& operator*() const;

    ///
    /// Dereferences the handle to a pointer to the entity.
    ///
    /// \returns A pointer to the entity.
    ///
    /// \throws InvalidOperation If the handle is invalid.
    Entity* operator->();

    ///
    /// Dereferences the handle to a pointer to the entity.
    ///
    /// \returns A pointer to the entity.
    ///
    /// \throws InvalidOperation If the handle is invalid.
    const Entity* operator->() const;

    ///
    /// Sets the handle as a copy of another.
    ///
    /// \param handle The handle to copy.
    EntityHandle& operator=(const EntityHandle& handle);

    ///
    /// Sets the handle as being moved from another.
    ///
    /// \param handle The handle to move.
    EntityHandle& operator=(EntityHandle&& handle);

    ///
    /// Returns whether the handle is equivalent to another.
    ///
    /// \param other The other iterator.
    bool operator==(const EntityHandle& other) const;

    ///
    /// Returns whether the handle is different from another.
    ///
    /// \param other The other iterator.
    bool operator!=(const EntityHandle& other) const;

    ///
    /// Returns whether the handle is valid.
    operator bool() const;

private:
    EntityHandle(EntityPool& pool, EntityId id);

    bool isValid() const;
    void ensureValid() const;

    class Context :
        public EventListener<EntityEvent>
    {
    public:
        Context(EntityPool& pool, EntityId id);
        ~Context();

        void receiveEvent(const EntityEvent& event) override;

        EntityPool* pool;
        EntityId id;
        bool valid { true };
    };

    std::shared_ptr<Context> _context;
};

}
