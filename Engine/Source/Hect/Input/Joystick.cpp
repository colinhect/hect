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
#include "Joystick.h"

#include <cassert>

#include "Hect/Core/Exception.h"
#include "Hect/Core/Format.h"
#include "Hect/Reflection/Enum.h"

using namespace hect;

Joystick::Joystick(Name name, size_t button_count, size_t axis_count) :
    _name(name),
    _button_states(button_count, false),
    _axis_states(axis_count, 0)
{
}

bool Joystick::is_button_down(JoystickButton button) const
{
    if (static_cast<size_t>(button) < _button_states.size())
    {
        return _button_states[static_cast<size_t>(button)];
    }
    else
    {
        throw InvalidOperation(format("Joystick does not have button '%s'", Enum::to_string(button).data()));
    }
}

double Joystick::axis_value(JoystickAxis axis) const
{
    if (static_cast<size_t>(axis) < _axis_states.size())
    {
        return _axis_states[static_cast<size_t>(axis)];
    }
    else
    {
        throw InvalidOperation(format("Joystick does not have axis '%s'", Enum::to_string(axis).data()));
    }
}

void Joystick::enqueue_event(const JoystickEvent& event)
{
    if (event.type == JoystickEventType::ButtonDown)
    {
        assert(static_cast<size_t>(event.button) <= _button_states.size());
        _button_states[static_cast<size_t>(event.button)] = true;
    }
    else if (event.type == JoystickEventType::ButtonUp)
    {
        assert(static_cast<size_t>(event.button) <= _button_states.size());
        _button_states[static_cast<size_t>(event.button)] = false;
    }
    else if (event.type == JoystickEventType::AxisMotion)
    {
        assert(static_cast<size_t>(event.axis) <= _axis_states.size());
        _axis_states[static_cast<size_t>(event.axis)] = event.axis_value;
    }
    _events.push_back(event);
}

void Joystick::dispatch_events()
{
    for (const JoystickEvent& event : _events)
    {
        dispatch_event(event);
    }
    _events.clear();
}
