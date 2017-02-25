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

void InputAxisBinding::update(Platform& platform, Seconds timeStep)
{
    switch (type)
    {
    case InputAxisBindingType::MouseMoveX:
    {
        _value = deadValue;
        if (platform.hasMouse())
        {
            Mouse& mouse = platform.mouse();
            modifyValue(mouse.cursorMovement().x * mouseSensitivity);
        }
    }
    break;
    case InputAxisBindingType::MouseMoveY:
    {
        _value = deadValue;
        if (platform.hasMouse())
        {
            Mouse& mouse = platform.mouse();
            modifyValue(mouse.cursorMovement().y * mouseSensitivity);
        }
    }
    break;
    case InputAxisBindingType::MouseButton:
    {
        if (platform.hasMouse())
        {
            Mouse& mouse = platform.mouse();
            if (mouse.isButtonDown(mouseButton))
            {
                modifyValue(acceleration * timeStep.value);
            }
            else
            {
                applyGravity(timeStep);
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
        if (platform.hasKeyboard())
        {
            Keyboard& keyboard = platform.keyboard();
            if (keyboard.isKeyDown(key))
            {
                modifyValue(acceleration * timeStep.value);
            }
            else
            {
                applyGravity(timeStep);
            }
        }
    }
    break;
    case InputAxisBindingType::JoystickAxis:
    {
        _value = deadValue;
        if (platform.hasJoystick(joystickIndex))
        {
            Joystick& joystick = platform.joystick(joystickIndex);
            double value = joystick.axisValue(joystickAxis);

            // If the value is outside of the dead zone
            if (value < joystickAxisDeadZone.x || value > joystickAxisDeadZone.y)
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
        if (platform.hasJoystick(joystickIndex))
        {
            Joystick& joystick = platform.joystick(joystickIndex);
            if (joystick.isButtonDown(joystickButton))
            {
                modifyValue(acceleration * timeStep.value);
            }
            else
            {
                applyGravity(timeStep);
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
    encoder << encodeEnum<InputAxisBindingType>("type", type)
            << encodeEnum<MouseButton>("mouseButton", mouseButton)
            << encodeValue("mouseSensitivity", mouseSensitivity)
            << encodeEnum<Key>("key", key)
            << encodeValue("joystickIndex", joystickIndex)
            << encodeEnum<JoystickAxis>("joystickAxis", joystickAxis)
            << encodeValue("joystickAxisDeadZone", joystickAxisDeadZone)
            << encodeEnum<JoystickButton>("joystickButton", joystickButton)
            << encodeValue("acceleration", acceleration)
            << encodeValue("gravity", gravity)
            << encodeValue("range", range)
            << encodeValue("invert", invert)
            << encodeValue("deadValue", deadValue);
}

void InputAxisBinding::decode(Decoder& decoder)
{
    decoder >> decodeEnum<InputAxisBindingType>("type", type)
            >> decodeEnum<MouseButton>("mouseButton", mouseButton)
            >> decodeValue("mouseSensitivity", mouseSensitivity)
            >> decodeEnum<Key>("key", key)
            >> decodeValue("joystickIndex", joystickIndex)
            >> decodeEnum<JoystickAxis>("joystickAxis", joystickAxis)
            >> decodeValue("joystickAxisDeadZone", joystickAxisDeadZone)
            >> decodeEnum<JoystickButton>("joystickButton", joystickButton)
            >> decodeValue("acceleration", acceleration)
            >> decodeValue("gravity", gravity)
            >> decodeValue("range", range)
            >> decodeValue("invert", invert)
            >> decodeValue("deadValue", deadValue);

    // Make sure joystick dead zone is defined in the correct order
    if (joystickAxisDeadZone.x > joystickAxisDeadZone.y)
    {
        std::swap(joystickAxisDeadZone.x, joystickAxisDeadZone.y);
    }

    // Make sure range is defined in the correct order
    if (range.x > range.y)
    {
        std::swap(range.x, range.y);
    }
}

void InputAxisBinding::applyGravity(Seconds timeStep)
{
    if (_value != 0)
    {
        double delta = gravity * timeStep.value;
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

void InputAxisBinding::modifyValue(double delta)
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
