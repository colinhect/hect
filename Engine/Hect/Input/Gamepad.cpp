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
#include "Gamepad.h"

#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"
#include "Hect/Reflection/Enum.h"

using namespace hect;

GamepadEvent::GamepadEvent() :
    type(GamepadEventType_AxisMotion),
    gamepadIndex(0),
    button(GamepadButton_Button0),
    axis(GamepadAxis_Axis0),
    axisValue(0)
{
}

bool Gamepad::isButtonDown(GamepadButton button) const
{
    if (button < _buttonStates.size())
    {
        return _buttonStates[button];
    }
    else
    {
        throw Error(format("Gamepad does not have button '%s'", Enum::toString(button).c_str()));
    }
}
Real Gamepad::axisValue(GamepadAxis axis) const
{
    if (axis < _axisStates.size())
    {
        return _axisStates[axis];
    }
    else
    {
        throw Error(format("Gamepad does not have axis '%s'", Enum::toString(axis).c_str()));
    }
}

Gamepad::Gamepad(const std::string& name, size_t buttonCount, size_t axisCount) :
    _name(name),
    _buttonStates(buttonCount, false),
    _axisStates(axisCount, 0)
{
}

void Gamepad::enqueueEvent(const GamepadEvent& event)
{
    if (event.type == GamepadEventType_ButtonDown)
    {
        _buttonStates[event.button] = true;
    }
    else if (event.type == GamepadEventType_ButtonUp)
    {
        _buttonStates[event.button] = false;
    }
    else if (event.type == GamepadEventType_AxisMotion)
    {
        _axisStates[event.axis] = event.axisValue;
    }
    _events.push_back(event);
}

void Gamepad::dispatchEvents()
{
    for (const GamepadEvent& event : _events)
    {
        dispatchEvent(event);
    }

    _events.clear();
}