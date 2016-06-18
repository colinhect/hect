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

#include <cstdint>

#include "Hect/Core/Export.h"

namespace hect
{

class Entity;
class EntityPool;

///
/// A numeric identifier for an Entity.
typedef uint32_t EntityId;

///
/// The base functionality for an entity iterator.
class HECT_EXPORT EntityIteratorBase
{
public:

    ///
    /// Constructs an invalid entity iterator.
    EntityIteratorBase();

    ///
    /// Constructs a valid entity iterator.
    ///
    /// \param pool The entity pool that the entity belongs to.
    /// \param id The id of the entity.
    EntityIteratorBase(EntityPool& pool, EntityId id);

    ///
    /// Invalidates the iterator.
    void invalidate();

protected:
    Entity& dereference() const;
    void increment();
    bool isValid() const;
    bool equals(const EntityIteratorBase& other) const;

private:
    mutable EntityPool* _pool { nullptr };
    EntityId _id { EntityId(-1) };
};

///
/// An iterator referring to a Entity in an EntityPool with a specific id.
///
/// \note The prefered alias for this type is Entity::Iterator or
/// Entity::ConstIterator.
///
/// An entity iterator will remain valid as entities are created and destroyed
/// in the entity pool.  However, if the referred enitity is destroy, a newly
/// added entity may re-use the same id, leaving the iterator valid but
/// referring to a new entity.  To hold a reference to an entity that is
/// guaranteed to refer to the same entity, use an Entity::Handle.  An iterator
/// can be created from an entity using Entity::iterator().
class HECT_EXPORT EntityIterator :
    public EntityIteratorBase
{
    friend class Entity;
public:

    ///
    /// Constructs an invalid entity iterator.
    EntityIterator();

    ///
    /// Constructs a valid entity iterator.
    ///
    /// \param pool The entity pool that the entity belongs to.
    /// \param id The id of the entity.
    EntityIterator(EntityPool& pool, EntityId id);

    ///
    /// Dereferences the iterator to a reference to the entity.
    ///
    /// \returns A reference to the entity.
    ///
    /// \throws InvalidOperation If the iterator is invalid.
    Entity& operator*() const;

    ///
    /// Dereferences the iterator to a pointer to the entity.
    ///
    /// \returns A reference to the entity.
    ///
    /// \throws InvalidOperation If the iterator is invalid.
    Entity* operator->() const;

    ///
    /// Moves to the next activated entity in the entity pool.
    ///
    /// \returns A reference to the iterator.
    EntityIterator& operator++();

    ///
    /// Returns whether the iterator is equivalent to another.
    ///
    /// \param other The other iterator.
    bool operator==(const EntityIterator& other) const;

    ///
    /// Returns whether the iterator is different from another.
    ///
    /// \param other The other iterator.
    bool operator!=(const EntityIterator& other) const;

    ///
    /// Returns whether the iterator is valid.
    operator bool() const;
};

///
/// \copydoc hect::EntityIterator
class HECT_EXPORT EntityConstIterator :
    public EntityIteratorBase
{
    friend class Entity;
public:

    ///
    /// Constructs an invalid entity iterator.
    EntityConstIterator();

    ///
    /// Constructs a valid entity iterator.
    ///
    /// \param pool The entity pool that the entity belongs to.
    /// \param id The id of the entity.
    EntityConstIterator(const EntityPool& pool, EntityId id);

    ///
    /// Dereferences the iterator to a reference to the entity.
    ///
    /// \returns A reference to the entity.
    ///
    /// \throws InvalidOperation If the iterator is invalid.
    const Entity& operator*() const;

    ///
    /// Dereferences the iterator to a pointer to the entity.
    ///
    /// \returns A reference to the entity.
    ///
    /// \throws InvalidOperation If the iterator is invalid.
    const Entity* operator->() const;

    ///
    /// Moves to the next activated entity in the entity pool.
    ///
    /// \returns A reference to the iterator.
    EntityConstIterator& operator++();

    ///
    /// Returns whether the iterator is equivalent to another.
    ///
    /// \param other The other iterator.
    bool operator==(const EntityConstIterator& other) const;

    ///
    /// Returns whether the iterator is different from another.
    ///
    /// \param other The other iterator.
    bool operator!=(const EntityConstIterator& other) const;

    ///
    /// Returns whether the iterator is valid.
    operator bool() const;
};

}
