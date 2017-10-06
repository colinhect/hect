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
#include "Hect/Scene/ComponentPool.h"
#include "Hect/Scene/Scene.h"

namespace hect
{

template <typename ComponentType>
ComponentListener<ComponentType>::ComponentListener(Scene& scene)
{
    scene.components<ComponentType>().register_listener(*this);
}

template <typename ComponentType>
void ComponentListener<ComponentType>::on_component_added(ComponentType& component)
{
    (void)component;
}

template <typename ComponentType>
void ComponentListener<ComponentType>::on_component_removed(ComponentType& component)
{
    (void)component;
}

template <typename ComponentType>
void ComponentListener<ComponentType>::receive_event(const ComponentEvent<ComponentType>& event)
{
    if (event.entity)
    {
        // Copy the event to allow access to non-const component reference of
        // the entity
        ComponentEvent<ComponentType> copied_event = event;
        auto& component = copied_event.entity->template component<ComponentType>();

        switch (event.type)
        {
        case ComponentEventType::Add:
            on_component_added(component);
            break;
        case ComponentEventType::Remove:
            on_component_removed(component);
            break;
        default:
            break;
        }
    }
}

}
