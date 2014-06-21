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
/// A component of an Entity.
template <typename T>
class Component :
    public ComponentBase
{
    friend class ComponentPool<T>;
private:
    class IterBase
    {
    public:
        IterBase();
        IterBase(ComponentPool<T>& pool, ComponentId id);

    protected:
        void _increment();
        bool _isValid() const;
        void _ensureValid() const;
        bool _equals(const IterBase& other) const;

        mutable ComponentPool<T>* _pool;
        ComponentId _id;
    };

public:

    ///
    /// A component iterator.
    class Iter :
        public IterBase
    {
    public:

        ///
        /// An array of component iterators.
        typedef std::vector<Iter> Array;

        ///
        /// Constructs an invalid component iterator.
        Iter();

        ///
        /// Constructs a component iterator given the pool and the id of the
        /// component.
        ///
        /// \param pool The component pool that the component belongs to.
        /// \param id The id of the component.
        Iter(ComponentPool<T>& pool, ComponentId id);

        ///
        /// Dereferences the iterator to a reference to the component.
        ///
        /// \returns A reference to the component.
        ///
        /// \throws Error If the iterator is invalid.
        T& operator*() const;

        ///
        /// Dereferences the iterator to a pointer to the component.
        ///
        /// \returns A reference to the component.
        ///
        /// \throws Error If the iterator is invalid.
        T* operator->() const;

        ///
        /// Moves to the next component with an activated entity in the
        /// component pool.
        ///
        /// \returns A reference to the iterator.
        Iter& operator++();

        ///
        /// Returns whether the component is equivalent to another.
        ///
        /// \param other The other iterator.
        bool operator==(const Iter& other) const;

        ///
        /// Returns whether the component is different from another.
        ///
        /// \param other The other iterator.
        bool operator!=(const Iter& other) const;

        ///
        /// Returns whether the iterator is valid.
        operator bool() const;
    };

    ///
    /// A constant component iterator.
    class ConstIter :
        public IterBase
    {
    public:

        ///
        /// An array of component iterators.
        typedef std::vector<ConstIter> Array;

        ///
        /// Constructs an invalid component iterator.
        ConstIter();

        ///
        /// Constructs a component iterator given the pool and the id of the
        /// component.
        ///
        /// \param pool The component pool that the component belongs to.
        /// \param id The id of the component.
        ConstIter(const ComponentPool<T>& pool, ComponentId id);

        ///
        /// Dereferences the iterator to a reference to the component.
        ///
        /// \returns A reference to the component.
        ///
        /// \throws Error If the iterator is invalid.
        const T& operator*() const;

        ///
        /// Dereferences the iterator to a pointer to the component.
        ///
        /// \returns A reference to the component.
        ///
        /// \throws Error If the iterator is invalid.
        const T* operator->() const;

        ///
        /// Moves to the next component with an activated entity in the
        /// component pool.
        ///
        /// \returns A reference to the iterator.
        ConstIter& operator++();

        ///
        /// Returns whether the component is equivalent to another.
        ///
        /// \param other The other iterator.
        bool operator==(const ConstIter& other) const;

        ///
        /// Returns whether the component is different from another.
        ///
        /// \param other The other iterator.
        bool operator!=(const ConstIter& other) const;

        ///
        /// Returns whether the iterator is valid.
        operator bool() const;
    };

    Component();

    ///
    /// Returns the pool that the component is in.
    ///
    /// \throws Error If the component is not in a pool.
    ComponentPool<T>& pool();

    ///
    /// Returns the pool that the component is in.
    ///
    /// \throws Error If the component is not in a pool.
    const ComponentPool<T>& pool() const;

    ///
    /// Returns the entity of the component.
    ///
    /// \throws Error If the component is not in a pool or the component does
    /// belong to an entity.
    Entity& entity();

    ///
    /// Returns the entity of the component.
    ///
    /// \throws Error If the component is not in a pool or the component does
    /// belong to an entity.
    const Entity& entity() const;

    ///
    /// Returns the id of the component.
    ComponentId id() const;

    ///
    /// \copydoc ComponentBase::typeIndex()
    std::type_index typeIndex() const;

private:
    void _enterPool(ComponentPool<T>& pool, ComponentId id);
    void _exitPool();
    bool _inPool() const;
    void _ensureInPool() const;

    ComponentPool<T>* _pool;
    ComponentId _id;
};

}

#include "Component.inl"
