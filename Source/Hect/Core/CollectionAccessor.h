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

#include <vector>

#include "Hect/Core/Uncopyable.h"

namespace hect
{

///
/// A wrapper around an STL container providing iteration and indexing access
/// without providing the ability to modify the container itself.
template <typename T, typename Container = std::vector<T>>
class CollectionAccessor :
    public Uncopyable
{
public:

    ///
    /// Constructs the collection given a reference to the underlying
    /// container.
    ///
    /// \param container The underlying container.
    CollectionAccessor(const Container& container);

    ///
    /// Returns an iterator to the beginning of the collection.
    typename Container::iterator begin();

    ///
    /// Returns a constant iterator to the beginning of the collection.
    typename Container::const_iterator begin() const;

    ///
    /// Returns an iterator to the end of the collection.
    typename Container::iterator end();

    ///
    /// Returns a constant iterator to the end of the collection.
    typename Container::const_iterator end() const;

    ///
    /// Returns the number of elements in the collection.
    size_t size() const;

    ///
    /// Returns a reference to the element at the given index in the
    /// collection.
    ///
    /// \param index The index of the element to access.
    T& operator[](typename Container::size_type index);

    ///
    /// Returns a constant reference to the element at the given index in the
    /// collection.
    ///
    /// \param index The index of the element to access.
    const T& operator[](typename Container::size_type index) const;

private:
    Container* _container;
};

}

#include "CollectionAccessor.inl"