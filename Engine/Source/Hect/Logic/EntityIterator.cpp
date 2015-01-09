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
#include "EntityIterator.h"

#include "Hect/Logic/Entity.h"
#include "Hect/Logic/EntityPool.h"

using namespace hect;

EntityIteratorBase::EntityIteratorBase()
{
}

EntityIteratorBase::EntityIteratorBase(EntityPool& pool, EntityId id) :
    _pool(&pool),
    _id(id)
{
}

Entity& EntityIteratorBase::dereference() const
{
    if (!isValid())
    {
        throw InvalidOperation("Invalid entity iterator");
    }

    return _pool->entityWithId(_id);
}

void EntityIteratorBase::increment()
{
    ++_id;
    size_t maxEntityId = _pool->maxId();
    while (_id < maxEntityId)
    {
        if (isValid() && _pool->entityWithId(_id).isActivated())
        {
            break;
        }

        ++_id;
    }
}

bool EntityIteratorBase::isValid() const
{
    return _pool && _pool->entityIsValid(_id);
}

bool EntityIteratorBase::equals(const EntityIteratorBase& other) const
{
    return _pool == other._pool && _id == other._id;
}

EntityIterator::EntityIterator() :
    EntityIteratorBase()
{
}

EntityIterator::EntityIterator(EntityPool& pool, EntityId id) :
    EntityIteratorBase(pool, id)
{
}

Entity& EntityIterator::operator*() const
{
    return dereference();
}

Entity* EntityIterator::operator->() const
{
    return &dereference();
}

EntityIterator& EntityIterator::operator++()
{
    increment();
    return *this;
}

bool EntityIterator::operator==(const EntityIterator& other) const
{
    return equals(other);
}

bool EntityIterator::operator!=(const EntityIterator& other) const
{
    return !equals(other);
}

EntityIterator::operator bool() const
{
    return isValid();
}

EntityConstIterator::EntityConstIterator() :
    EntityIteratorBase()
{
}

EntityConstIterator::EntityConstIterator(const EntityPool& pool, EntityId id) :
    EntityIteratorBase(*const_cast<EntityPool*>(&pool), id)
{
}

const Entity& EntityConstIterator::operator*() const
{
    return dereference();
}

const Entity* EntityConstIterator::operator->() const
{
    return &dereference();
}

EntityConstIterator& EntityConstIterator::operator++()
{
    increment();
    return *this;
}

bool EntityConstIterator::operator==(const EntityConstIterator& other) const
{
    return equals(other);
}

bool EntityConstIterator::operator!=(const EntityConstIterator& other) const
{
    return !equals(other);
}

EntityConstIterator::operator bool() const
{
    return isValid();
}