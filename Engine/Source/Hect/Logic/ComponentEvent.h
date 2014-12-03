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

#include "Hect/Core/Event.h"

namespace hect
{

///
/// A component-related event type.
enum ComponentEventType
{
    ///
    /// A component was added to an entity.
    ///
    /// This event type is triggered when an entity pending activation is
    /// activated during Scene::refresh().  It also may be triggered
    /// immediately if an activated entity has a component added or replaced.
    ComponentEventType_Add,

    ///
    /// A component was removed from an entity.
    ///
    /// This event type is triggered when an entity pending destruction is
    /// destroyed during Scene::refresh().  It also may be triggered
    /// immediately if an activated entity has a component removed or replaced.
    ComponentEventType_Remove
};

class Entity;

///
/// A component-related event.
template <typename T>
class ComponentEvent
{
public:
    ComponentEvent(ComponentEventType type, Entity& entity);

    ///
    /// The event type.
    ComponentEventType type;

    ///
    /// Returns a reference to the entity that the event is for.
    ///
    /// \warning This reference is only guaranteed to be valid for the duration
    /// of the call to Listener::receiveEvent() when the event is dispatched.
    Entity& entity() const;

private:
    mutable Entity* _entity;
};

}

#include "ComponentEvent.inl"
