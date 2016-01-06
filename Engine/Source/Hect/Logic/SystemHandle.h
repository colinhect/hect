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

#include "Hect/Logic/SystemBase.h"

namespace hect
{

///
/// A handle referring to a System
///
/// \note The prefered alias for this type is System::Handle.
template <typename T>
class SystemHandle
{
public:

    ///
    /// Constructs an invalid system handle.
    SystemHandle();

    ///
    /// Constructs a valid system handle.
    ///
    /// \param scene The scene.
    /// \param typeId The type id of the system.
    SystemHandle(Scene& scene, SystemTypeId typeId);

    ///
    /// Dereferences the handle to a reference to the system.
    ///
    /// \returns A reference to the system.
    ///
    /// \throws InvalidOperation If the handle is invalid or the scene does
    /// not support the system type.
    T& operator*() const;

    ///
    /// Dereferences the handle to a pointer to the system.
    ///
    /// \returns A reference to the system.
    ///
    /// \throws InvalidOperation If the handle is invalid or the scene does
    /// not support the system type.
    T* operator->() const;

    ///
    /// Returns whether the handle is equivalent to another.
    ///
    /// \param other The other handle.
    bool operator==(const SystemHandle& other) const;

    ///
    /// Returns whether the handle is different from another.
    ///
    /// \param other The other iterator.
    bool operator!=(const SystemHandle& other) const;

    ///
    /// Returns whether the handle is valid.
    operator bool() const;

private:
    Scene* _scene { nullptr };
    SystemTypeId _typeId { SystemTypeId(-1) };
};

///
/// A constant handle referring to a System
///
/// \note The prefered alias for this type is System::ConstHandle.
template <typename T>
class SystemConstHandle
{
public:

    ///
    /// Constructs an invalid system handle.
    SystemConstHandle();

    ///
    /// Constructs a valid system handle.
    ///
    /// \param scene The scene.
    /// \param typeId The type id of the system.
    SystemConstHandle(const Scene& scene, SystemTypeId typeId);

    ///
    /// Dereferences the handle to a reference to the system.
    ///
    /// \returns A reference to the system.
    ///
    /// \throws InvalidOperation If the handle is invalid or the scene does
    /// not support the system type.
    const T& operator*() const;

    ///
    /// Dereferences the handle to a pointer to the system.
    ///
    /// \returns A reference to the system.
    ///
    /// \throws InvalidOperation If the handle is invalid or the scene does
    /// not support the system type.
    const T* operator->() const;

    ///
    /// Returns whether the handle is equivalent to another.
    ///
    /// \param other The other handle.
    bool operator==(const SystemConstHandle& other) const;

    ///
    /// Returns whether the handle is different from another.
    ///
    /// \param other The other iterator.
    bool operator!=(const SystemConstHandle& other) const;

    ///
    /// Returns whether the handle is valid.
    operator bool() const;

private:
    const Scene* _scene
    {
        nullptr
    };
    SystemTypeId _typeId { SystemTypeId(-1) };
};

}
