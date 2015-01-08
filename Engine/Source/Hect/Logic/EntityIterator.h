///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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

class HECT_EXPORT EntityIteratorBase
{
public:
    EntityIteratorBase();
    EntityIteratorBase(EntityPool& pool, EntityId id);

protected:
    void increment();
    bool isValid() const;
    void ensureValid() const;
    bool equals(const EntityIteratorBase& other) const;

    mutable EntityPool* _pool { nullptr };
    EntityId _id { EntityId(-1) };
};

///
/// A Entity iterator.
class HECT_EXPORT EntityIterator :
    public EntityIteratorBase
{
    friend class Entity;
public:

    ///
    /// Constructs an invalid entity iterator.
    EntityIterator();

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
/// A constant Entity iterator.
class HECT_EXPORT ConstEntityIterator :
    public EntityIteratorBase
{
    friend class Entity;
public:

    ///
    /// Constructs an invalid entity iterator.
    ConstEntityIterator();

    ConstEntityIterator(const EntityPool& pool, EntityId id);

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
    ConstEntityIterator& operator++();

    ///
    /// Returns whether the iterator is equivalent to another.
    ///
    /// \param other The other iterator.
    bool operator==(const ConstEntityIterator& other) const;

    ///
    /// Returns whether the iterator is different from another.
    ///
    /// \param other The other iterator.
    bool operator!=(const ConstEntityIterator& other) const;

    ///
    /// Returns whether the iterator is valid.
    operator bool() const;
};

}
