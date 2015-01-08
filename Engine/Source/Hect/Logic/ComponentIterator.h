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

namespace hect
{

///
/// A numeric identifier for a Component.
typedef uint32_t ComponentId;

template <typename T>
class ComponentPool;

template <typename T>
class ComponentIteratorBase
{
public:
    ComponentIteratorBase();
    ComponentIteratorBase(ComponentPool<T>& pool, ComponentId id);

protected:
    void increment();
    bool isValid() const;
    void ensureValid() const;
    bool equals(const ComponentIteratorBase& other) const;

    mutable ComponentPool<T>* _pool { nullptr };
    ComponentId _id { ComponentId(-1) };
};

///
/// A component iterator.
template <typename T>
class ComponentIterator :
    public ComponentIteratorBase<T>
{
public:

    ///
    /// Constructs an invalid component iterator.
    ComponentIterator();

    ComponentIterator(ComponentPool<T>& pool, ComponentId id);

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
/// A constant component iterator.
template <typename T>
class ConstComponentIterator :
    public ComponentIteratorBase<T>
{
public:

    ///
    /// Constructs an invalid component iterator.
    ConstComponentIterator();

    ConstComponentIterator(const ComponentPool<T>& pool, ComponentId id);

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
    ConstComponentIterator& operator++();

    ///
    /// Returns whether the iterator is equivalent to another.
    ///
    /// \param other The other iterator.
    bool operator==(const ConstComponentIterator& other) const;

    ///
    /// Returns whether the iterator is different from another.
    ///
    /// \param other The other iterator.
    bool operator!=(const ConstComponentIterator& other) const;

    ///
    /// Returns whether the iterator is valid.
    operator bool() const;
};

}
