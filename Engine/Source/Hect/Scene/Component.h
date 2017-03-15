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

#include <functional>

#include "Hect/Core/Export.h"
#include "Hect/IO/Encodable.h"
#include "Hect/IO/Encoder.h"
#include "Hect/IO/Decoder.h"
#include "Hect/Scene/ComponentHandle.h"
#include "Hect/Scene/ComponentIterator.h"
#include "Hect/Scene/EntityIterator.h"

namespace hect
{

///
/// A numeric identifier for a Component type.
typedef uint32_t ComponentTypeId;

///
/// Abstract base for Component.
class HECT_EXPORT ComponentBase :
    public Encodable
{
public:
    virtual ~ComponentBase() { }

    ///
    /// Returns the if of the component's type.
    virtual ComponentTypeId typeId() const = 0;
};

template <typename T>
class ComponentPool;

class Entity;

///
/// A component of an Entity.
///
/// \warning Raw references and pointers to a component may be invalidated as
/// new components are added to and removed from the entity pool.  To maintain
/// a valid reference to an component, use Component::Iterator.
template <typename T>
class Component :
    public ComponentBase
{
    friend class ComponentPool<T>;
public:

    ///
    /// \copydoc hect::ComponentHandle
    typedef ComponentHandle<T> Handle;

    ///
    /// \copydoc hect::ComponentIterator
    typedef ComponentIterator<T> Iterator;

    ///
    /// \copydoc hect::ComponentConstIterator
    typedef ComponentConstIterator<T> ConstIterator;

    ///
    /// Constructs a default component.
    Component();

    ///
    /// Constructs a component as a copy from another component.
    ///
    /// \param component The component to copy.
    Component(const Component& component);

    ///
    /// Constructs a component moved from another component.
    ///
    /// \param component The component to move.
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
    /// Returns an iterator to the entity of the component.
    ///
    /// \throws InvalidOperation If the component is not in a pool or the
    /// component does belong to an entity.
    Entity& entity();

    ///
    /// \copydoc hect::Component::entity()
    const Entity& entity() const;

    ///
    /// Returns a handle to the component.
    typename Component<T>::Handle handle() const;

    ///
    /// Creates a component iterator for the component.
    typename Component<T>::Iterator iterator();

    ///
    /// \copydoc hect::Component::iterator()
    typename Component<T>::ConstIterator iterator() const;

    ///
    /// Returns the id of the component.
    ComponentId id() const;

    ComponentTypeId typeId() const override;

    ///
    /// Assigns the component as a copy from another component.
    ///
    /// \param component The component to copy.
    Component<T>& operator=(const Component& component);

    ///
    /// Assigns the component moved from another component.
    ///
    /// \param component The component to move.
    Component<T>& operator=(Component&& component);

private:
    void enterPool(ComponentPool<T>& pool, ComponentId id);
    void exitPool();
    bool inPool() const;
    void ensureInPool() const;

    ComponentPool<T>* _pool { nullptr };
    ComponentId _id { ComponentId(-1) };
    mutable ComponentHandle<T> _handle;
};

}
