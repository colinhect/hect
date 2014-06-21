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

#include <typeindex>

#include "Hect/Logic/ComponentBase.h"

#pragma warning (disable : 4505)

namespace hect
{

class Entity;

template <typename T>
class ComponentPool;

///
/// A component of an entity.
template <typename T>
class Component :
    public ComponentBase
{
private:
    class IterBase
    {
    public:
        IterBase();
        IterBase(ComponentPool<T>* pool, ComponentId id);

    protected:
        void _increment();
        bool _isValid() const;
        void _ensureValid() const;
        bool _equals(const IterBase& other) const;

        mutable ComponentPool<T>* _pool;
        ComponentId _id;
    };

public:
    class Iter :
        public IterBase
    {
    public:
        Iter();
        Iter(ComponentPool<T>* pool, ComponentId id);

        T& operator*() const;
        T* operator->() const;
        Iter& operator++();

        bool operator==(const Iter& other) const;
        bool operator!=(const Iter& other) const;

        operator bool() const;
    };

    class ConstIter :
        public IterBase
    {
    public:
        ConstIter();
        ConstIter(const ComponentPool<T>* pool, ComponentId id);

        const T& operator*() const;
        const T* operator->() const;
        ConstIter& operator++();

        bool operator==(const ConstIter& other) const;
        bool operator!=(const ConstIter& other) const;

        operator bool() const;
    };

    Component();

    void enterPool(ComponentPool<T>* pool, ComponentId id);
    void exitPool();

    bool inPool() const;

    ComponentPool<T>& pool();
    const ComponentPool<T>& pool() const;

    Entity& entity();
    const Entity& entity() const;

    ComponentId id() const;

    ///
    /// \copydoc ComponentBase::typeIndex()
    std::type_index typeIndex() const;

private:
    void _ensureInPool() const;

    ComponentPool<T>* _pool;
    ComponentId _id;
};

}

#include "Component.inl"
