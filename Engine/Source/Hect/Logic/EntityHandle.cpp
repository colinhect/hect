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
#include "EntityHandle.h"

#include "Hect/Logic/EntityPool.h"

using namespace hect;

EntityHandle::EntityHandle()
{
}

EntityHandle::EntityHandle(const EntityHandle& handle) :
    _context(handle._context)
{
}

EntityHandle::EntityHandle(EntityHandle&& handle) :
    _context(std::move(handle._context))
{
}

Entity& EntityHandle::operator*()
{
    ensureValid();
    return _context->pool->entityWithId(_context->id);
}

const Entity& EntityHandle::operator*() const
{
    ensureValid();
    return _context->pool->entityWithId(_context->id);
}

Entity* EntityHandle::operator->()
{
    ensureValid();
    return &_context->pool->entityWithId(_context->id);
}

const Entity* EntityHandle::operator->() const
{
    ensureValid();
    return &_context->pool->entityWithId(_context->id);
}

EntityHandle& EntityHandle::operator=(const EntityHandle& handle)
{
    _context = handle._context;
    return *this;
}

EntityHandle& EntityHandle::operator=(EntityHandle&& handle)
{
    _context = std::move(handle._context);
    return *this;
}

bool EntityHandle::operator==(const EntityHandle& other) const
{
    return _context == other._context;
}

bool EntityHandle::operator!=(const EntityHandle& other) const
{
    return _context != other._context;
}

EntityHandle::operator bool() const
{
    return isValid();
}

EntityHandle::EntityHandle(EntityPool& pool, EntityId id) :
    _context(std::make_shared<EntityHandle::Context>(pool, id))
{
}

bool EntityHandle::isValid() const
{
    return _context && _context->valid;
}

void EntityHandle::ensureValid() const
{
    if (!isValid())
    {
        throw InvalidOperation("Invalid entity handle");
    }
}

EntityHandle::Context::Context(EntityPool& pool, EntityId id) :
    pool(&pool),
    id(id)
{
    pool.addListener(*this);
}

EntityHandle::Context::~Context()
{
    pool->removeListener(*this);
}

void EntityHandle::Context::receiveEvent(const EntityEvent& event)
{
    if (valid && event.entity->id() == id)
    {
        if (event.type == EntityEventType_Destroy)
        {
            valid = false;
        }
    }
}
