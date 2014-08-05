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
#include "Joystick.h"

#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"
#include "Hect/Input/InputDevices.h"

using namespace hect;

JoystickEvent::JoystickEvent() :
    type(JoystickEventType_AxisMotion),
    joystickIndex(0),
    buttonIndex(0),
    axisIndex(0),
    axisValue(0)
{
}

size_t Joystick::buttonCount() const
{
    return _buttonStates.size();
}

bool Joystick::isButtonDown(size_t buttonIndex) const
{
    if (buttonIndex < _buttonStates.size())
    {
        return _buttonStates[buttonIndex];
    }
    else
    {
        throw Error(format("Joystick does not have button of index %i", buttonIndex));
    }
}

size_t Joystick::axisCount() const
{
    return _axisStates.size();
}

Real Joystick::axisValue(size_t axisIndex) const
{
    if (axisIndex < _axisStates.size())
    {
        return _axisStates[axisIndex];
    }
    else
    {
        throw Error(format("Joystick does not have axis of index %i", axisIndex));
    }
}

void Joystick::hapticRumble(Real strength, TimeSpan duration)
{
    _inputDevices->hapticRumble(*this, strength, duration);
}

Dispatcher<JoystickEvent>& Joystick::dispatcher()
{
    return _dispatcher;
}

Joystick::Joystick(InputDevices& inputDevices, size_t index, const std::string& name, size_t buttonCount, size_t axisCount) :
    _inputDevices(&inputDevices),
    _index(index),
    _name(name),
    _buttonStates(buttonCount, false),
    _axisStates(axisCount, 0)
{
}

void Joystick::enqueueEvent(const JoystickEvent& event)
{
    if (event.type == JoystickEventType_ButtonDown)
    {
        _buttonStates[event.buttonIndex] = true;
    }
    else if (event.type == JoystickEventType_ButtonUp)
    {
        _buttonStates[event.buttonIndex] = false;
    }
    else if (event.type == JoystickEventType_AxisMotion)
    {
        _axisStates[event.axisIndex] = event.axisValue;
    }
    _events.push_back(event);
}

void Joystick::dispatchEvents()
{
    for (const JoystickEvent& event : _events)
    {
        _dispatcher.dispatchEvent(event);
    }

    _events.clear();
}