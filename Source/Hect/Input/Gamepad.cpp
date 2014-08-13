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

using namespace hect;

GamepadEvent::GamepadEvent() :
    type(GamepadEventType_AxisMotion),
    gamepadIndex(0),
    buttonIndex(0),
    axisIndex(0),
    axisValue(0)
{
}

size_t Gamepad::buttonCount() const
{
    return _buttonStates.size();
}

bool Gamepad::isButtonDown(size_t buttonIndex) const
{
    if (buttonIndex < _buttonStates.size())
    {
        return _buttonStates[buttonIndex];
    }
    else
    {
        throw Error(format("Gamepad does not have button of index %i", buttonIndex));
    }
}

size_t Gamepad::axisCount() const
{
    return _axisStates.size();
}

Real Gamepad::axisValue(size_t axisIndex) const
{
    if (axisIndex < _axisStates.size())
    {
        return _axisStates[axisIndex];
    }
    else
    {
        throw Error(format("Gamepad does not have axis of index %i", axisIndex));
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
        _buttonStates[event.buttonIndex] = true;
    }
    else if (event.type == GamepadEventType_ButtonUp)
    {
        _buttonStates[event.buttonIndex] = false;
    }
    else if (event.type == GamepadEventType_AxisMotion)
    {
        _axisStates[event.axisIndex] = event.axisValue;
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