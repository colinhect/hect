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

#include "Hect/Core/Exception.h"
#include "Hect/Core/Export.h"
#include "Hect/Scene/EntityIterator.h"

namespace hect
{

///
/// The base functionality for an entity iterator.
class HECT_EXPORT EntityChildIteratorBase
{
public:

    ///
    /// Constructs an invalid entity child iterator.
    EntityChildIteratorBase();

    ///
    /// Constructs a valid entity child iterator.
    ///
    /// \param pool The entity pool that the entity belongs to.
    /// \param parentId The id of the parent entity.
    /// \param index The index of the child entity.
    EntityChildIteratorBase(EntityPool& pool, EntityId parentId, size_t index);

protected:
    Entity& dereference() const;
    void increment();
    bool isValid() const;
    bool equals(const EntityChildIteratorBase& other) const;

private:
    EntityPool* _pool { nullptr };
    EntityId _parentId { EntityId(-1) };
    size_t _index { 0 };
};

///
/// An iterator referring to a child Entity at a specific index.
///
/// \note The prefered alias for this type is Entity::Children::Iterator or
/// Entity::Children::ConstIterator.
///
/// An entity child iterator is only intended for iterating.  To maintain a
/// reference to a child entity create a standard iterator using
/// Entity::iterator().
class HECT_EXPORT EntityChildIterator :
    public EntityChildIteratorBase
{
public:

    ///
    /// Constructs an invalid entity child iterator.
    EntityChildIterator();

    ///
    /// Constructs a valid entity child iterator.
    ///
    /// \param pool The entity pool that the entity belongs to.
    /// \param parentId The id of the parent entity.
    /// \param index The index of the child entity.
    EntityChildIterator(EntityPool& pool, EntityId parentId, size_t index);

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
    EntityChildIterator& operator++();

    ///
    /// Returns whether the iterator is equivalent to another.
    ///
    /// \param other The other iterator.
    bool operator==(const EntityChildIterator& other) const;

    ///
    /// Returns whether the iterator is different from another.
    ///
    /// \param other The other iterator.
    bool operator!=(const EntityChildIterator& other) const;

    ///
    /// Returns whether the iterator is valid.
    operator bool() const;
};

///
/// \copydoc hect::EntityChildIterator
class HECT_EXPORT EntityChildConstIterator :
    public EntityChildIteratorBase
{
    friend class Entity;
public:

    ///
    /// Constructs an invalid entity iterator.
    EntityChildConstIterator();

    ///
    /// Constructs a valid entity child iterator.
    ///
    /// \param pool The entity pool that the entity belongs to.
    /// \param parentId The id of the parent entity.
    /// \param index The index of the child entity.
    EntityChildConstIterator(const EntityPool& pool, EntityId parentId, size_t index);

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
    EntityChildConstIterator& operator++();

    ///
    /// Returns whether the iterator is equivalent to another.
    ///
    /// \param other The other iterator.
    bool operator==(const EntityChildConstIterator& other) const;

    ///
    /// Returns whether the iterator is different from another.
    ///
    /// \param other The other iterator.
    bool operator!=(const EntityChildConstIterator& other) const;

    ///
    /// Returns whether the iterator is valid.
    operator bool() const;
};

}
