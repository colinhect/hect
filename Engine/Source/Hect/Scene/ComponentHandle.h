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

#include <atomic>
#include <memory>

namespace hect
{

template <typename T>
class Component;

///
/// A weak reference to a Component.
template <typename T>
class ComponentHandle
{
    friend class Component<T>;
public:

    ///
    /// Constructs an invalid handle.
    ComponentHandle();

    ///
    /// Dereferences the handle to a reference to the component.
    ///
    /// \returns A reference to the component.
    ///
    /// \throws InvalidOperation If the handle is invalid.
    T& operator*();

    ///
    /// Dereferences the handle to a reference to the component.
    ///
    /// \returns A reference to the component.
    ///
    /// \throws InvalidOperation If the handle is invalid.
    const T& operator*() const;

    ///
    /// Dereferences the handle to a pointer to the component.
    ///
    /// \returns A pointer to the component.
    ///
    /// \throws InvalidOperation If the handle is invalid.
    T* operator->();

    ///
    /// Dereferences the handle to a pointer to the component.
    ///
    /// \returns A pointer to the component.
    ///
    /// \throws InvalidOperation If the handle is invalid.
    const T* operator->() const;

    ///
    /// Returns whether the handle is equivalent to another.
    ///
    /// \param other The other iterator.
    bool operator==(const ComponentHandle& other) const;

    ///
    /// Returns whether the handle is different from another.
    ///
    /// \param other The other iterator.
    bool operator!=(const ComponentHandle& other) const;

    ///
    /// Returns whether the handle is valid.
    operator bool() const;

private:
    ComponentHandle(T& component);

    bool isValid() const;
    void ensureValid() const;
    void invalidate();

    T* _component { nullptr };
    std::shared_ptr<std::atomic_bool> _valid;
};

}

#include "ComponentHandle.inl"
