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

namespace hect
{

///
/// A container which may hold an optional value.
template <typename T>
class Optional
{
public:

    ///
    /// Constructs an optional container without a value.
    Optional();

    ///
    /// Constructs an optional container with a value.
    ///
    /// \param value The value for the optional container to hold.
    Optional(const T& value);

    ///
    /// Contructs an optional container copied from another.
    ///
    /// \param optional The optional container to copy.
    Optional(const Optional& optional);

    ///
    /// Contructs an optional container moved from another.
    ///
    /// \param optional The optional container to move.
    Optional(Optional&& optional);

    ///
    /// Returns whether the optional container holds a value.
    bool has_value() const;

    ///
    /// Returns a reference to the value that the optional container holds.
    ///
    /// \throws InvalidOperation If the optional container has no value.
    T& value();

    ///
    /// \copydoc Optional::value()
    const T& value() const;

    ///
    /// \copydoc Optional::has_value()
    operator bool() const;

    ///
    /// \copydoc Optional::value()
    T& operator*();

    ///
    /// \copydoc Optional::value()
    const T& operator*() const;

    ///
    /// \copydoc Optional::value()
    T* operator->();

    ///
    /// \copydoc Optional::value()
    const T* operator->() const;

    ///
    /// Copies a value to the optional container.
    ///
    /// \param value The value to copy.
    Optional& operator=(const T& value);

    ///
    /// Moves a value to the optional container.
    ///
    /// \param value The value to move.
    Optional& operator=(T&& value);

    ///
    /// Assigns the optional container to a copy of another.
    ///
    /// \param optional The optional container to copy.
    Optional& operator=(const Optional<T>& optional);

    ///
    /// Assigns the optional container moved from another.
    ///
    /// \param optional The optional container to move.
    Optional& operator=(Optional<T>&& optional);

private:
    bool _has_value { false };
    T _value;
};

}

#include "Optional.inl"
