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
#include "InputAxisBinding.h"

#include "Hect/Runtime/Platform.h"

using namespace hect;

void InputAxisBinding::update(Platform& platform, Seconds time_step)
{
    switch (type)
    {
    case InputAxisBindingType::MouseMoveX:
    {
        _value = dead_value;
        if (platform.has_mouse())
        {
            Mouse& mouse = platform.mouse();
            modify_value(mouse.cursor_movement().x * mouse_sensitivity);
        }
    }
    break;
    case InputAxisBindingType::MouseMoveY:
    {
        _value = dead_value;
        if (platform.has_mouse())
        {
            Mouse& mouse = platform.mouse();
            modify_value(mouse.cursor_movement().y * mouse_sensitivity);
        }
    }
    break;
    case InputAxisBindingType::MouseButton:
    {
        if (platform.has_mouse())
        {
            Mouse& mouse = platform.mouse();
            if (mouse.is_button_down(mouse_button))
            {
                modify_value(acceleration * time_step.value);
            }
            else
            {
                apply_gravity(time_step);
            }
        }
    }
    break;
    case InputAxisBindingType::MouseScroll:
    {
    }
    break;
    case InputAxisBindingType::Key:
    {
        if (platform.has_keyboard())
        {
            Keyboard& keyboard = platform.keyboard();
            if (keyboard.is_key_down(key))
            {
                modify_value(acceleration * time_step.value);
            }
            else
            {
                apply_gravity(time_step);
            }
        }
    }
    break;
    case InputAxisBindingType::JoystickAxis:
    {
        _value = dead_value;
        if (platform.has_joystick(joystick_index))
        {
            Joystick& joystick = platform.joystick(joystick_index);
            double value = joystick.axis_value(joystick_axis);

            // If the value is outside of the dead zone
            if (value < joystick_axis_dead_zone.x || value > joystick_axis_dead_zone.y)
            {
                // Scale the value to be a delta for the range
                double delta = value * 0.5 + 0.5;
                if (invert)
                {
                    delta = 1.0 - delta;
                }

                // Interpolate between the range using the delta
                _value = (1 - delta) * range.x + delta * range.y;
            }
        }
    }
    break;
    case InputAxisBindingType::JoystickButton:
    {
        if (platform.has_joystick(joystick_index))
        {
            Joystick& joystick = platform.joystick(joystick_index);
            if (joystick.is_button_down(joystick_button))
            {
                modify_value(acceleration * time_step.value);
            }
            else
            {
                apply_gravity(time_step);
            }
        }
    }
    break;
    }

    // Clamp within the effective range
    _value = std::max(range.x, std::min(_value, range.y));
}

double InputAxisBinding::value() const
{
    return _value;
}

void InputAxisBinding::encode(Encoder& encoder) const
{
    encoder << encode_enum<InputAxisBindingType>("type", type)
            << encode_enum<MouseButton>("mouse_button", mouse_button)
            << encode_value("mouse_sensitivity", mouse_sensitivity)
            << encode_enum<Key>("key", key)
            << encode_value("joystick_index", joystick_index)
            << encode_enum<JoystickAxis>("joystick_axis", joystick_axis)
            << encode_value("joystick_axis_dead_zone", joystick_axis_dead_zone)
            << encode_enum<JoystickButton>("joystick_button", joystick_button)
            << encode_value("acceleration", acceleration)
            << encode_value("gravity", gravity)
            << encode_value("range", range)
            << encode_value("invert", invert)
            << encode_value("dead_value", dead_value);
}

void InputAxisBinding::decode(Decoder& decoder)
{
    decoder >> decode_enum<InputAxisBindingType>("type", type)
            >> decode_enum<MouseButton>("mouse_button", mouse_button)
            >> decode_value("mouse_sensitivity", mouse_sensitivity)
            >> decode_enum<Key>("key", key)
            >> decode_value("joystick_index", joystick_index)
            >> decode_enum<JoystickAxis>("joystick_axis", joystick_axis)
            >> decode_value("joystick_axis_dead_zone", joystick_axis_dead_zone)
            >> decode_enum<JoystickButton>("joystick_button", joystick_button)
            >> decode_value("acceleration", acceleration)
            >> decode_value("gravity", gravity)
            >> decode_value("range", range)
            >> decode_value("invert", invert)
            >> decode_value("dead_value", dead_value);

    // Make sure joystick dead zone is defined in the correct order
    if (joystick_axis_dead_zone.x > joystick_axis_dead_zone.y)
    {
        std::swap(joystick_axis_dead_zone.x, joystick_axis_dead_zone.y);
    }

    // Make sure range is defined in the correct order
    if (range.x > range.y)
    {
        std::swap(range.x, range.y);
    }
}

void InputAxisBinding::apply_gravity(Seconds time_step)
{
    if (_value != 0)
    {
        double delta = gravity * time_step.value;
        if (_value > 0)
        {
            _value -= delta;
        }
        else
        {
            _value += delta;
        }

        if (delta > std::abs(_value))
        {
            _value = 0;
        }
    }
}

void InputAxisBinding::modify_value(double delta)
{
    if (invert)
    {
        _value -= delta;
    }
    else
    {
        _value += delta;
    }
}
