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
#include "EntityChildIterator.h"

#include "Hect/Logic/EntityPool.h"

using namespace hect;

EntityChildIteratorBase::EntityChildIteratorBase()
{
}

EntityChildIteratorBase::EntityChildIteratorBase(EntityPool& pool, EntityId parentId, size_t index) :
    _pool(&pool),
    _parentId(parentId),
    _index(index)
{
}

Entity& EntityChildIteratorBase::dereference() const
{
    if (!isValid())
    {
        throw InvalidOperation("Invalid entity iterator");
    }

    Entity& entity = _pool->entityWithId(_parentId);
    EntityId id = entity._childIds[_index];
    return _pool->entityWithId(id);
}

void EntityChildIteratorBase::increment()
{
    if (isValid())
    {
        ++_index;
    }
}

bool EntityChildIteratorBase::isValid() const
{
    return _pool && _index < _pool->entityWithId(_parentId)._childIds.size();
}

bool EntityChildIteratorBase::equals(const EntityChildIteratorBase& other) const
{
    return _pool == other._pool && _parentId == other._parentId && _index == other._index;
}

EntityChildIterator::EntityChildIterator() :
    EntityChildIteratorBase()
{
}

EntityChildIterator::EntityChildIterator(EntityPool& pool, EntityId parentId, size_t index) :
    EntityChildIteratorBase(pool, parentId, index)
{
}

Entity& EntityChildIterator::operator*() const
{
    return dereference();
}

Entity* EntityChildIterator::operator->() const
{
    return &dereference();
}

EntityChildIterator& EntityChildIterator::operator++()
{
    increment();
    return *this;
}

bool EntityChildIterator::operator==(const EntityChildIterator& other) const
{
    return equals(other);
}

bool EntityChildIterator::operator!=(const EntityChildIterator& other) const
{
    return !equals(other);
}

EntityChildIterator::operator bool() const
{
    return isValid();
}

EntityChildConstIterator::EntityChildConstIterator() :
    EntityChildIteratorBase()
{
}

EntityChildConstIterator::EntityChildConstIterator(const EntityPool& pool, EntityId parentId, size_t index) :
    EntityChildIteratorBase(*const_cast<EntityPool*>(&pool), parentId, index)
{
}

const Entity& EntityChildConstIterator::operator*() const
{
    return dereference();
}

const Entity* EntityChildConstIterator::operator->() const
{
    return &dereference();
}

EntityChildConstIterator& EntityChildConstIterator::operator++()
{
    increment();
    return *this;
}

bool EntityChildConstIterator::operator==(const EntityChildConstIterator& other) const
{
    return equals(other);
}

bool EntityChildConstIterator::operator!=(const EntityChildConstIterator& other) const
{
    return !equals(other);
}

EntityChildConstIterator::operator bool() const
{
    return isValid();
}
