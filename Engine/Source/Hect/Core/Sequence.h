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

#include <cstddef>

#include "Hect/Core/Uncopyable.h"

namespace hect
{

///
/// A wrapper around an STL container providing iteration access without
/// providing the ability to modify the container itself.
template <typename T, typename Container>
class Sequence :
    public Uncopyable
{
public:

    ///
    /// Constructs a sequence.
    ///
    /// \param container The underlying container.
    Sequence(const Container& container);

    Sequence(Sequence&& sequence);

    ///
    /// Returns an iterator to the beginning of the sequence.
    typename Container::iterator begin();

    ///
    /// Returns an iterator to the end of the sequence.
    typename Container::iterator end();

    ///
    /// Returns a constant iterator to the beginning of the sequence.
    typename Container::const_iterator begin() const;

    ///
    /// Returns a constant iterator to the end of the sequence.
    typename Container::const_iterator end() const;

    ///
    /// Returns the number of items in the sequence.
    size_t size() const;

    ///
    /// Returns whether the sequence has no items.
    bool empty() const;

    ///
    /// Returns the item in the sequence at the given index.
    ///
    /// \param index The index of which item to access.
    T& operator[](size_t index);

    ///
    /// Returns the item in the sequence at the given index.
    ///
    /// \param index The index of which item to access.
    const T& operator[](size_t index) const;

private:
    Container& _container;
};

}

#include "Sequence.inl"
