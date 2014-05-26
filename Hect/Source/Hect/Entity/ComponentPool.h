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

#include <cstdint>
#include <vector>

#include "Hect/Core/IdPool.h"
#include "Hect/Core/Export.h"

namespace hect
{
    
typedef uint32_t EntityId;
typedef uint32_t ComponentId;

class ComponentPoolBase
{
public:
    virtual ~ComponentPoolBase() { }

    virtual bool remove(EntityId entityId) = 0;
    virtual void clone(EntityId sourceEntityId, EntityId destEntityId) = 0;
};

template <typename T>
class ComponentPool :
    public ComponentPoolBase
{
public:
    ComponentPool();

    T& add(EntityId entityId, const T& component);

    bool remove(EntityId entityId);
    void clone(EntityId sourceEntityId, EntityId destEntityId);

    bool has(EntityId entityId) const;

    T& get(EntityId entityId);

private:
    IdPool<ComponentId> _componentIdPool;
    std::vector<T> _components;
    std::vector<ComponentId> _entityIdToComponentId;
};

}

#include "ComponentPool.inl"
