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

#include "Hect/Input/InputDevices.h"
#include "Hect/Logic/Scene.h"

using namespace hect;

InputSystem::InputSystem(Scene& scene, InputDevices& inputDevices) :
    System(scene),
    _inputDevices(&inputDevices)
{
}

void InputSystem::addAxis(const InputAxis& axis)
{
    // Make sure an axis with the name does not already exist
    for (const InputAxis& existingAxis : _axes)
    {
        if (existingAxis.name() == axis.name())
        {
            throw Error(format("Multiple inputDevices axes with name '%s'", axis.name().c_str()));
        }
    }

    _axes.push_back(axis);
}

Real InputSystem::axisValue(const std::string& name) const
{
    for (const InputAxis& axis : _axes)
    {
        if (axis.name() == name)
        {
            return axis.value();
        }
    }

    return 0;
}

void InputSystem::update()
{
    Real timeStepInSeconds = scene().timeStep().seconds();

    for (InputAxis& axis : _axes)
    {
        Real value = axis.value();
        Real acceleration = axis.acceleration();
        Real gravity = axis.gravity();

        // Gravitate towards zero
        if (gravity != 0)
        {
            axis.setValue(value - value * std::min<Real>(1, gravity * timeStepInSeconds));
        }

        if (axis.source() == InputAxisSource_Key)
        {
            Keyboard& keyboard = _inputDevices->keyboard();
            if (keyboard.isKeyDown(axis.positiveKey()))
            {
                axis.setValue(value + acceleration * timeStepInSeconds);
            }

            if (keyboard.isKeyDown(axis.negativeKey()))
            {
                axis.setValue(value - acceleration * timeStepInSeconds);
            }
        }
        else if (axis.source() == InputAxisSource_MouseButton)
        {
            Mouse& mouse = _inputDevices->mouse();
            if (mouse.isButtonDown(axis.positiveMouseButton()))
            {
                axis.setValue(value + acceleration * timeStepInSeconds);
            }

            if (mouse.isButtonDown(axis.negativeMouseButton()))
            {
                axis.setValue(value - acceleration * timeStepInSeconds);
            }
        }
        else if (axis.source() == InputAxisSource_JoystickButton)
        {
            size_t joystickIndex = axis.joystickIndex();
            if (joystickIndex < _inputDevices->joystickCount())
            {
                Joystick& sourceJoystick = _inputDevices->joystick(joystickIndex);
                if (sourceJoystick.isButtonDown(axis.positiveJoystickButtonIndex()))
                {
                    axis.setValue(value + acceleration * timeStepInSeconds);
                }

                if (sourceJoystick.isButtonDown(axis.negativeJoystickButtonIndex()))
                {
                    axis.setValue(value - acceleration * timeStepInSeconds);
                }
            }
        }
        else if (axis.source() == InputAxisSource_JoystickAxis)
        {
            size_t joystickIndex = axis.joystickIndex();
            if (joystickIndex < _inputDevices->joystickCount())
            {
                Joystick& sourceJoystick = _inputDevices->joystick(joystickIndex);
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