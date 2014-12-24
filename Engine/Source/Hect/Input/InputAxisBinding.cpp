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
#include "InputAxisBinding.h"

#include "Hect/Math/Utilities.h"
#include "Hect/Runtime/Platform.h"

using namespace hect;

void InputAxisBinding::update(Platform& platform, Real timeStep)
{
    switch (type)
    {
    case InputAxisBindingType_MouseMoveX:
    {
        _value = deadValue;
        if (platform.hasMouse())
        {
            Mouse& mouse = platform.mouse();
            modifyValue(mouse.cursorMovement().x * mouseSensitivity);
        }
    }
    break;
    case InputAxisBindingType_MouseMoveY:
    {
        _value = deadValue;
        if (platform.hasMouse())
        {
            Mouse& mouse = platform.mouse();
            modifyValue(mouse.cursorMovement().y * mouseSensitivity);
        }
    }
    break;
    case InputAxisBindingType_MouseButton:
    {
        if (platform.hasMouse())
        {
            Mouse& mouse = platform.mouse();
            if (mouse.isButtonDown(mouseButton))
            {
                modifyValue(acceleration * timeStep);
            }
            else
            {
                applyGravity(timeStep);
            }
        }
    }
    break;
    case InputAxisBindingType_MouseScroll:
    {
    }
    break;
    case InputAxisBindingType_Key:
    {
        if (platform.hasKeyboard())
        {
            Keyboard& keyboard = platform.keyboard();
            if (keyboard.isKeyDown(key))
            {
                modifyValue(acceleration * timeStep);
            }
            else
            {
                applyGravity(timeStep);
            }
        }
    }
    break;
    case InputAxisBindingType_JoystickAxis:
    {
        _value = deadValue;
        if (platform.hasJoystick(joystickIndex))
        {
            Joystick& joystick = platform.joystick(joystickIndex);
            Real value = joystick.axisValue(joystickAxis);

            // If the value is outside of the dead zone
            if (value < joystickAxisDeadZone.x && value > joystickAxisDeadZone.y)
            {
                // Scale the value to be a delta for the range
                Real delta = value * Real(0.5) + Real(0.5);
                if (invert)
                {
                    delta = 1.0 - delta;
                }

                // Interpolate between the range using the delta
                _value = interpolate(range.x, range.y, delta);
            }
        }
    }
    break;
    case InputAxisBindingType_JoystickButton:
    {
        if (platform.hasJoystick(joystickIndex))
        {
            Joystick& joystick = platform.joystick(joystickIndex);
            if (joystick.isButtonDown(joystickButton))
            {
                modifyValue(acceleration * timeStep);
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
    _value = clamp(_value, range.x, range.y);
}

Real InputAxisBinding::value() const
{
    return _value;
}

void InputAxisBinding::applyGravity(Real timeStep)
{
    if (_value != 0)
    {
        Real delta = gravity * timeStep;
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

void InputAxisBinding::modifyValue(Real delta)
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

namespace hect
{

Encoder& operator<<(Encoder& encoder, const InputAxisBinding& inputAxisBinding)
{
    encoder << beginObject()
            << encodeEnum<InputAxisBindingType>("type", inputAxisBinding.type)
            << encodeEnum<MouseButton>("mouseButton", inputAxisBinding.mouseButton)
            << encodeValue("mouseSensitivity", inputAxisBinding.mouseSensitivity)
            << encodeEnum<Key>("key", inputAxisBinding.key)
            << encodeValue("joystickIndex", inputAxisBinding.joystickIndex)
            << encodeEnum<JoystickAxis>("joystickAxis", inputAxisBinding.joystickAxis)
            << encodeValue("joystickAxisDeadZone", inputAxisBinding.joystickAxisDeadZone)
            << encodeEnum<JoystickButton>("joystickButton", inputAxisBinding.joystickButton)
            << encodeValue("acceleration", inputAxisBinding.acceleration)
            << encodeValue("gravity", inputAxisBinding.gravity)
            << encodeValue("range", inputAxisBinding.range)
            << encodeValue("invert", inputAxisBinding.invert)
            << encodeValue("deadValue", inputAxisBinding.deadValue)
            << endObject();

    return encoder;
}

Decoder& operator>>(Decoder& decoder, InputAxisBinding& inputAxisBinding)
{
    decoder >> beginObject()
            >> decodeEnum<InputAxisBindingType>("type", inputAxisBinding.type)
            >> decodeEnum<MouseButton>("mouseButton", inputAxisBinding.mouseButton)
            >> decodeValue("mouseSensitivity", inputAxisBinding.mouseSensitivity)
            >> decodeEnum<Key>("key", inputAxisBinding.key)
            >> decodeValue("joystickIndex", inputAxisBinding.joystickIndex)
            >> decodeEnum<JoystickAxis>("joystickAxis", inputAxisBinding.joystickAxis)
            >> decodeValue("joystickAxisDeadZone", inputAxisBinding.joystickAxisDeadZone)
            >> decodeEnum<JoystickButton>("joystickButton", inputAxisBinding.joystickButton)
            >> decodeValue("acceleration", inputAxisBinding.acceleration)
            >> decodeValue("gravity", inputAxisBinding.gravity)
            >> decodeValue("range", inputAxisBinding.range)
            >> decodeValue("invert", inputAxisBinding.invert)
            >> decodeValue("deadValue", inputAxisBinding.deadValue)
            >> endObject();

    // Make sure joystick dead zone is defined in the correct order
    if (inputAxisBinding.joystickAxisDeadZone.x > inputAxisBinding.joystickAxisDeadZone.y)
    {
        std::swap(inputAxisBinding.joystickAxisDeadZone.x, inputAxisBinding.joystickAxisDeadZone.y);
    }

    // Make sure range is defined in the correct order
    if (inputAxisBinding.range.x > inputAxisBinding.range.y)
    {
        std::swap(inputAxisBinding.range.x, inputAxisBinding.range.y);
    }

    return decoder;
}

}