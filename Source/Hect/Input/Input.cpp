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
#include "Input.h"

#include <SDL.h>
#include <algorithm>

#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"

using namespace hect;

Input::Input()
{
    Dispatcher<MouseEvent>& mouseDispatcher = _mouse.dispatcher();
    mouseDispatcher.addListener(*this);

    for (int i = 0; i < SDL_NumJoysticks(); i++)
    {
        SDL_Joystick* joystick = SDL_JoystickOpen(i);
        if (joystick)
        {
            _sdlJoysticks.push_back(joystick);

            std::string name = SDL_JoystickName(joystick);
            size_t buttonCount = SDL_JoystickNumButtons(joystick);
            size_t axisCount = SDL_JoystickNumAxes(joystick);

            SDL_Haptic* haptic = SDL_HapticOpenFromJoystick(joystick);
            if (haptic)
            {
                if (SDL_HapticRumbleInit(haptic) == 0)
                {
                    _sdlHaptics[joystick] = haptic;
                }
                else
                {
                    SDL_HapticClose(haptic);
                }
            }

            HECT_INFO(format("Detected joystick '%s' with %i buttons and %i axes", name.c_str(), buttonCount, axisCount));

            _joysticks.push_back(Joystick(*this, i, name, buttonCount, axisCount));
        }
    }
}

Input::~Input()
{
    // Close all haptic devices
    for (auto& pair : _sdlHaptics)
    {
        SDL_HapticClose(pair.second);
    }
    _sdlHaptics.clear();

    // Close all joysticks
    for (SDL_Joystick* joystick : _sdlJoysticks)
    {
        SDL_JoystickClose(joystick);
    }
    _sdlJoysticks.clear();
}

void Input::addAxis(const InputAxis& axis)
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

const InputAxis& Input::axis(const std::string& name) const
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

bool Input::axisExists(const std::string& name) const
{
    for (const InputAxis& axis : _axes)
    {
        if (axis.name() == name)
        {
            return true;
        }
    }

    return false;
}

void Input::updateAxes(Real timeStep)
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
        else if (axis.source() == InputAxisSource_JoystickButton)
        {
            size_t joystickIndex = axis.joystickIndex();
            if (joystickIndex < joystickCount())
            {
                Joystick& sourceJoystick = joystick(joystickIndex);
                if (sourceJoystick.isButtonDown(axis.positiveJoystickButtonIndex()))
                {
                    axis.setValue(value + acceleration * timeStep);
                }

                if (sourceJoystick.isButtonDown(axis.negativeJoystickButtonIndex()))
                {
                    axis.setValue(value - acceleration * timeStep);
                }
            }
        }
        else if (axis.source() == InputAxisSource_JoystickAxis)
        {
            size_t joystickIndex = axis.joystickIndex();
            if (joystickIndex < joystickCount())
            {
                Joystick& sourceJoystick = joystick(joystickIndex);
                Real value = sourceJoystick.axisValue(axis.joystickAxisIndex());
                if (std::abs(value) < axis.joystickAxisDeadZone())
                {
                    value = 0;
                }
                if (axis.joystickAxisInverted())
                {
                    value = -value;
                }
                axis.setValue(value);
            }
        }
    }
}

Mouse& Input::mouse()
{
    return _mouse;
}

Keyboard& Input::keyboard()
{
    return _keyboard;
}

size_t Input::joystickCount() const
{
    return _joysticks.size();
}

Joystick& Input::joystick(size_t joystickIndex)
{
    if (joystickIndex < _joysticks.size())
    {
        return _joysticks[joystickIndex];
    }
    else
    {
        throw Error(format("System does not have joystick of index %i", joystickIndex));
    }
}

void Input::receiveEvent(const MouseEvent& event)
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

void Input::enqueueEvent(const JoystickEvent& event)
{
    joystick(event.joystickIndex).enqueueEvent(event);
}

void Input::enqueueEvent(const MouseEvent& event)
{
    _mouse.enqueueEvent(event);
}

void Input::enqueueEvent(const KeyboardEvent& event)
{
    _keyboard.enqueueEvent(event);
}

void Input::dispatchEvents()
{
    _mouse.dispatchEvents();
    _keyboard.dispatchEvents();
}

void Input::hapticRumble(Joystick& joystick, Real strength, TimeSpan duration)
{
    auto it = _sdlHaptics.find(_sdlJoysticks[joystick._index]);
    if (it != _sdlHaptics.end())
    {
        SDL_HapticRumblePlay(it->second, (float)strength, (uint32_t)duration.milliseconds());
    }
}