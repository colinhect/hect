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

namespace hect
{

///
/// A container which can hold any type of value.
class Any
{
public:

    ///
    /// Constructs a container without a value.
    Any();

    ///
    /// Constructs a container with a value.
    ///
    /// \param value The value.
    template <typename T>
    Any(const T& value);

    ///
    /// Constructs a copy of another container.
    ///
    /// \remarks The held value in the container itself is copied.
    ///
    /// \param any The container to copy.
    Any(const Any& any);

    ///
    /// Constructs a container moved from another container.
    ///
    /// \remarks The held value in the container itself is not copied.
    ///
    /// \param any The container to move.
    Any(Any&& any);

    ///
    /// Destructor.
    ~Any();

    ///
    /// Assigns a value to the container.
    ///
    /// \remarks If the container already had a value then that value is
    /// overwritten.
    ///
    /// \param value The new value.
    ///
    /// \returns A reference to the container.
    template <typename T>
    Any& operator=(const T& value);

    ///
    /// Assigns a value from another container to the container.
    ///
    /// \remarks If the container already had a value then that value is
    /// overwritten.
    ///
    /// \param any The container to assign the value from.
    ///
    /// \returns A reference to the container.
    Any& operator=(const Any& any);

    ///
    /// Returns whether the container holds a value of a certain type.
    template <typename T>
    bool isType() const;

    ///
    /// Returns whether the container holds any value at all.
    bool hasValue() const;

    ///
    /// Returns the value the container holds as a specific type.
    ///
    /// \throws Error If the container's value is not of the specific type.
    template <typename T>
    T& as() const;

private:
    class Container
    {
    public:
        virtual ~Container() { }
        virtual Container* clone() const = 0;
    };

    template <class T>
    class ContainerValue :
        public Container
    {
    public:
        ContainerValue(const T& value);
        virtual Container* clone() const;

        T held;

    private:
        ContainerValue& operator=(const ContainerValue&);
    };

    Container* _container;
};

}

#include "Any.inl"