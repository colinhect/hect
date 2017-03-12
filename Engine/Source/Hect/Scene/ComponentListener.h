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

#include "Hect/Core/EventListener.h"
#include "Hect/Scene/ComponentEvent.h"
#include "Hect/Scene/Scene.h"

namespace hect
{

///
/// Receives events when certain type of Component is added to and removed
/// from a Scene.
template <typename T>
class ComponentListener :
    public EventListener<ComponentEvent<T>>
{
public:

    ///
    /// Constructs a component listener.
    ///
    /// \param scene The scene to receive component events from.
    ComponentListener(Scene& scene);

    virtual ~ComponentListener() { }

    ///
    /// Invoked when an entity is activated with a component of the specified
    /// type or a component is added to an activated entity.
    ///
    /// \param component The component that was added.
    virtual void onComponentAdded(T& component);

    ///
    /// Invoked when an entity is destroyed with a component of the specified
    /// type or a component is removed from an activated entity.
    ///
    /// \param component The component that was removed.
    virtual void onComponentRemoved(T& component);

    void receiveEvent(const ComponentEvent<T>& event) override;
};

}

#include "ComponentListener.inl"
