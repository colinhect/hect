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

#include <cstdint>

namespace hect
{

///
/// A numeric identifier for a Component.
typedef uint32_t ComponentId;

template <typename ComponentType>
class ComponentPool;

///
/// The base functionality for a component iterator.
template <typename ComponentType>
class ComponentIteratorBase
{
public:

    ///
    /// Constructs an invalid component iterator.
    ComponentIteratorBase();

    ///
    /// Constructs a valid component iterator.
    ///
    /// \param pool The component pool that the component belongs to.
    /// \param id The id of the component.
    ComponentIteratorBase(ComponentPool<ComponentType>& pool, ComponentId id);

    ///
    /// Invalidates the iterator.
    void invalidate();

protected:
    ComponentType& dereference() const;
    void increment();
    bool is_valid() const;
    bool equals(const ComponentIteratorBase& other) const;

    mutable ComponentPool<ComponentType>* _pool { nullptr };
    ComponentId _id { ComponentId(-1) };
};

///
/// An iterator referring to a Component in a ComponentPool with a specific
/// id.
///
/// A component iterator will remain valid as components are added to and
/// removed from the component pool.  However, if the referred component is
/// removed, a newly added component may re-use the same id, leaving the
/// iterator valid but referring to a new component.  An iterator can be
/// created from a component using Component::iterator().
template <typename ComponentType>
class ComponentIterator :
    public ComponentIteratorBase<ComponentType>
{
    template <typename Type> friend class ComponentConstIterator;
public:

    ///
    /// Constructs an invalid component iterator.
    ComponentIterator();

    ///
    /// Constructs a valid component iterator.
    ///
    /// \param pool The component pool that the component belongs to.
    /// \param id The id of the component.
    ComponentIterator(ComponentPool<ComponentType>& pool, ComponentId id);

    ///
    /// Dereferences the iterator to a reference to the component.
    ///
    /// \returns A reference to the component.
    ///
    /// \throws InvalidOperation If the iterator is invalid.
    ComponentType& operator*() const;

    ///
    /// Dereferences the iterator to a pointer to the component.
    ///
    /// \returns A reference to the component.
    ///
    /// \throws InvalidOperation If the iterator is invalid.
    ComponentType* operator->() const;

    ///
    /// Moves to the next component with an activated entity in the
    /// component pool.
    ///
    /// \returns A reference to the iterator.
    ComponentIterator& operator++();

    ///
    /// Returns whether the iterator is equivalent to another.
    ///
    /// \param other The other iterator.
    bool operator==(const ComponentIterator& other) const;

    ///
    /// Returns whether the iterator is different from another.
    ///
    /// \param other The other iterator.
    bool operator!=(const ComponentIterator& other) const;

    ///
    /// Returns whether the iterator is valid.
    operator bool() const;
};

///
/// \copydoc hect::ComponentIterator
template <typename ComponentType>
class ComponentConstIterator :
    public ComponentIteratorBase<ComponentType>
{
public:

    ///
    /// Constructs an invalid component iterator.
    ComponentConstIterator();

    ///
    /// Constructs a valid component iterator.
    ///
    /// \param pool The component pool that the component belongs to.
    /// \param id The id of the component.
    ComponentConstIterator(const ComponentPool<ComponentType>& pool, ComponentId id);

    ///
    /// Constructs a valid component iterator.
    ///
    /// \param iterator The iterator to copy.
    ComponentConstIterator(const ComponentIterator<ComponentType>& iterator);

    ///
    /// Dereferences the iterator to a reference to the component.
    ///
    /// \returns A reference to the component.
    ///
    /// \throws InvalidOperation If the iterator is invalid.
    const ComponentType& operator*() const;

    ///
    /// Dereferences the iterator to a pointer to the component.
    ///
    /// \returns A reference to the component.
    ///
    /// \throws InvalidOperation If the iterator is invalid.
    const ComponentType* operator->() const;

    ///
    /// Moves to the next component with an activated entity in the
    /// component pool.
    ///
    /// \returns A reference to the iterator.
    ComponentConstIterator& operator++();

    ///
    /// Returns whether the iterator is equivalent to another.
    ///
    /// \param other The other iterator.
    bool operator==(const ComponentConstIterator& other) const;

    ///
    /// Returns whether the iterator is different from another.
    ///
    /// \param other The other iterator.
    bool operator!=(const ComponentConstIterator& other) const;

    ///
    /// Returns whether the iterator is valid.
    operator bool() const;
};

}
