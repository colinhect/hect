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

#include "Hect/IO/Encodable.h"

namespace hect
{

///
/// A numeric entity identifier.
typedef uint32_t EntityId;

///
/// A numeric component identifier.
typedef uint32_t ComponentId;

///
/// A numeric component type identifier.
typedef uint32_t ComponentTypeId;

///
/// Base entity component.
class HECT_API ComponentBase :
    public Encodable
{
public:

    ///
    /// Returns the type index for the component's type.
    virtual std::type_index typeIndex() const = 0;
};

template <typename T>
class ComponentPool;

///
/// A component of an entity.
template <typename T>
class Component :
    public ComponentBase
{
    friend class ComponentPool<T>;
public:
    Component();

    ///
    /// Returns the id of the entity that the component belongs to.
    EntityId entityId() const;

    ///
    /// \copydoc ComponentBase::typeIndex()
    std::type_index typeIndex() const;

private:
    EntityId _entityId;
};

}

#include "Component.inl"
