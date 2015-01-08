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

#include <typeindex>
#include <functional>

#include "Hect/Core/Export.h"
#include "Hect/IO/Encoder.h"
#include "Hect/IO/Decoder.h"

namespace hect
{

///
/// A numeric identifier for an Entity.
typedef uint32_t EntityId;

///
/// A numeric identifier for a Component.
typedef uint32_t ComponentId;

///
/// A numeric identifier for a Component type.
typedef uint32_t ComponentTypeId;

///
/// Abstract base for Component.
class HECT_EXPORT ComponentBase
{
public:
    virtual ~ComponentBase() { }

    ///
    /// Encodes the component to an object.
    ///
    /// \param encoder The encoder to use.
    virtual void encode(Encoder& encoder) const;

    ///
    /// Decodes the component from an object.
    ///
    /// \param decoder The decoder to use.
    virtual void decode(Decoder& decoder);

    virtual ComponentTypeId typeId() const = 0;
};

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
    class IteratorBase
    {
    public:
        IteratorBase();
        IteratorBase(ComponentPool<T>& pool, ComponentId id);

    protected:
        void increment();
        bool isValid() const;
        void ensureValid() const;
        bool equals(const IteratorBase& other) const;

        mutable ComponentPool<T>* _pool { nullptr };
        ComponentId _id { ComponentId(-1) };
    };

public:

    ///
    /// A predicate for a component search or filter.
    typedef std::function<bool(const T&)> Predicate;

    ///
    /// A component iterator.
    class Iterator :
        public IteratorBase
    {
    public:

        ///
        /// Constructs an invalid component iterator.
        Iterator();

        Iterator(ComponentPool<T>& pool, ComponentId id);

        ///
        /// Dereferences the iterator to a reference to the component.
        ///
        /// \returns A reference to the component.
        ///
        /// \throws InvalidOperation If the iterator is invalid.
        T& operator*() const;

        ///
        /// Dereferences the iterator to a pointer to the component.
        ///
        /// \returns A reference to the component.
        ///
        /// \throws InvalidOperation If the iterator is invalid.
        T* operator->() const;

        ///
        /// Moves to the next component with an activated entity in the
        /// component pool.
        ///
        /// \returns A reference to the iterator.
        Iterator& operator++();

        ///
        /// Returns whether the iterator is equivalent to another.
        ///
        /// \param other The other iterator.
        bool operator==(const Iterator& other) const;

        ///
        /// Returns whether the iterator is different from another.
        ///
        /// \param other The other iterator.
        bool operator!=(const Iterator& other) const;

        ///
        /// Returns whether the iterator is valid.
        operator bool() const;
    };

    ///
    /// A constant component iterator.
    class ConstIterator :
        public IteratorBase
    {
    public:

        ///
        /// Constructs an invalid component iterator.
        ConstIterator();

        ConstIterator(const ComponentPool<T>& pool, ComponentId id);

        ///
        /// Dereferences the iterator to a reference to the component.
        ///
        /// \returns A reference to the component.
        ///
        /// \throws InvalidOperation If the iterator is invalid.
        const T& operator*() const;

        ///
        /// Dereferences the iterator to a pointer to the component.
        ///
        /// \returns A reference to the component.
        ///
        /// \throws InvalidOperation If the iterator is invalid.
        const T* operator->() const;

        ///
        /// Moves to the next component with an activated entity in the
        /// component pool.
        ///
        /// \returns A reference to the iterator.
        ConstIterator& operator++();

        ///
        /// Returns whether the iterator is equivalent to another.
        ///
        /// \param other The other iterator.
        bool operator==(const ConstIterator& other) const;

        ///
        /// Returns whether the iterator is different from another.
        ///
        /// \param other The other iterator.
        bool operator!=(const ConstIterator& other) const;

        ///
        /// Returns whether the iterator is valid.
        operator bool() const;
    };

    Component();
    Component(const Component& component);
    Component(Component&& component);

    ///
    /// Returns the pool that the component is in.
    ///
    /// \throws InvalidOperation If the component is not in a pool.
    ComponentPool<T>& pool();

    ///
    /// Returns the pool that the component is in.
    ///
    /// \throws InvalidOperation If the component is not in a pool.
    const ComponentPool<T>& pool() const;

    ///
    /// Returns the entity of the component.
    ///
    /// \throws InvalidOperation If the component is not in a pool or the
    /// component does belong to an entity.
    Entity& entity();

    ///
    /// Returns the entity of the component.
    ///
    /// \throws InvalidOperation If the component is not in a pool or the
    /// component does belong to an entity.
    const Entity& entity() const;

    ///
    /// Creates a component iterator for the component.
    typename Component<T>::Iterator iterator();

    ///
    /// Creates a constant component iterator for the component.
    typename Component<T>::ConstIterator iterator() const;

    ///
    /// Returns the id of the component.
    ComponentId id() const;

    ///
    /// Returns the id for the component's type.
    ComponentTypeId typeId() const override;

    Component<T>& operator=(const Component& component);
    Component<T>& operator=(Component&& component);

private:
    void enterPool(ComponentPool<T>& pool, ComponentId id);
    void exitPool();
    bool inPool() const;
    void ensureInPool() const;

    ComponentPool<T>* _pool { nullptr };
    ComponentId _id { ComponentId(-1) };
};

}
