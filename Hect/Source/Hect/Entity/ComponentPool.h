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

#include <algorithm>
#include <cstdint>
#include <vector>

#include "Hect/Core/IdPool.h"
#include "Hect/Entity/Component.h"

namespace hect
{

class ComponentPoolBase
{
    friend class Scene;
public:
    virtual ~ComponentPoolBase() { }

protected:
    virtual void _add(EntityId, const ComponentBase& component) = 0;
    virtual bool _remove(EntityId entityId) = 0;
    virtual void _clone(EntityId sourceEntityId, EntityId destEntityId) = 0;
};

template <typename T>
class ComponentIterator
{
    friend class ComponentPool<T>;
public:
    T& operator*() const;

    ComponentIterator& operator++();

    bool operator==(const ComponentIterator<T>& other) const;
    bool operator!=(const ComponentIterator<T>& other) const;

private:
    ComponentIterator(ComponentPool<T>* componentPool, size_t index);

    ComponentPool<T>* _componentPool;
    size_t _index;
};

template <typename T>
class ComponentPool :
    public ComponentPoolBase
{
    friend class Scene;
    friend class ComponentIterator<T>;
public:

    ComponentIterator<T> begin();
    ComponentIterator<T> end();
    
private:
    void _add(EntityId entityId, const ComponentBase& component);
    T& _add(EntityId entityId, const T& component);

    bool _remove(EntityId entityId);
    void _clone(EntityId sourceEntityId, EntityId destEntityId);

    bool _has(EntityId entityId) const;

    T& _get(EntityId entityId);

    IdPool<ComponentId> _componentIdPool;
    std::vector<T> _components;
    std::vector<ComponentId> _entityIdToComponentId;
};

}

#include "ComponentPool.inl"
