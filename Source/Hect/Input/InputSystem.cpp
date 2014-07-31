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
#include "InputSystem.h"

#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"

#include <algorithm>

using namespace hect;

InputSystem::InputSystem()
{
    Dispatcher<MouseEvent>& mouseDispatcher = _mouse.dispatcher();
    mouseDispatcher.addListener(*this);
}

void InputSystem::addAxis(const InputAxis& axis)
{
    // Make sure an axis with the name does not already exist
    for (const InputAxis& existingAxis : _axes)
    {
        if (existingAxis.name() == axis.name())
        {
            throw Error(format("Multiple input axes with name '%s'", axis.name().c_str()));
        }
    }

    _axes.push_back(axis);
}

const InputAxis& InputSystem::axisWithName(const std::string& name) const
{
    for (const InputAxis& axis : _axes)
    {
        if (axis.name() == name)
        {
            return axis;
        }
    }

    throw Error(format("No input axis with name '%s'", name.c_str()));
}

void InputSystem::updateAxes(Real timeStep)
{
    for (InputAxis& axis : _axes)
    {
        Real value = axis.value();
        Real acceleration = axis.acceleration();
        Real gravity = axis.gravity();

        // Gravitate towards zero
        if (gravity != 0)
        {
            axis.setValue(value - value * std::min<Real>(1, gravity * timeStep));
        }

        if (axis.source() == InputAxisSource_Key)
        {
            if (_keyboard.isKeyDown(axis.positiveKey()))
            {
                axis.setValue(value + acceleration * timeStep);
            }

            if (_keyboard.isKeyDown(axis.negativeKey()))
            {
                axis.setValue(value - acceleration * timeStep);
            }
        }
        else if (axis.source() == InputAxisSource_MouseButton)
        {
            if (_mouse.isButtonDown(axis.positiveMouseButton()))
            {
                axis.setValue(value + acceleration * timeStep);
            }

            if (_mouse.isButtonDown(axis.negativeMouseButton()))
            {
                axis.setValue(value - acceleration * timeStep);
            }
        }
    }
}

Mouse& InputSystem::mouse()
{
    return _mouse;
}

Keyboard& InputSystem::keyboard()
{
    return _keyboard;
}

void InputSystem::receiveEvent(const MouseEvent& event)
{
    if (event.type == MouseEventType_Movement)
    {
        Real movementX = (Real)event.cursorMovement.x;
        Real movementY = (Real)event.cursorMovement.y;

        for (InputAxis& axis : _axes)
        {
            if (axis.source() == InputAxisSource_MouseMoveX)
            {
                axis.setValue(axis.value() + movementX * axis.acceleration());
            }
            else if (axis.source() == InputAxisSource_MouseMoveY)
            {
                axis.setValue(axis.value() + movementY * axis.acceleration());
            }
        }
    }
    else if (event.type == MouseEventType_ScrollUp || event.type == MouseEventType_ScrollDown)
    {
        Real movement = event.type == MouseEventType_ScrollUp
                        ? (Real)1
                        : (Real)-1;

        for (InputAxis& axis : _axes)
        {
            if (axis.source() == InputAxisSource_MouseScroll)
            {
                axis.setValue(axis.value() + movement * axis.acceleration());
            }
        }
    }
}

void InputSystem::enqueueEvent(const MouseEvent& event)
{
    _mouse.enqueueEvent(event);
}

void InputSystem::enqueueEvent(const KeyboardEvent& event)
{
    _keyboard.enqueueEvent(event);
}

void InputSystem::dispatchEvents()
{
    _mouse.dispatchEvents();
    _keyboard.dispatchEvents();
}