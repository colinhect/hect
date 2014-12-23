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

#include "Hect/Core/Format.h"
#include "Hect/Core/Logging.h"
#include "Hect/Math/Utilities.h"
#include "Hect/Runtime/Platform.h"

using namespace hect;

void InputAxisBinding::update(Platform& platform, Real timeStep)
{
    // Apply gravity
    modifyValue(_value < 0, gravity * timeStep);

    switch (type)
    {
    case InputAxisBindingType_MouseMoveX:
    {
        if (platform.hasMouse())
        {
            Mouse& mouse = platform.mouse();
            if (mouse.mode() == MouseMode_Relative)
            {
                modifyValue(false, mouse.cursorMovement().x * mouseSensitivity);
            }
        }
    }
    break;
    case InputAxisBindingType_MouseMoveY:
    {
        if (platform.hasMouse())
        {
            Mouse& mouse = platform.mouse();
            if (mouse.mode() == MouseMode_Relative)
            {
                modifyValue(false, mouse.cursorMovement().y * mouseSensitivity);
            }
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
                modifyValue(invert, acceleration * timeStep);
            }
        }
    }
    break;
    case InputAxisBindingType_Key:
    {
        if (platform.hasKeyboard())
        {
            Keyboard& keyboard = platform.keyboard();
            if (keyboard.isKeyDown(key))
            {
                modifyValue(invert, acceleration * timeStep);
            }
        }
    }
    break;
    case InputAxisBindingType_JoystickAxis:
    {
        if (platform.hasJoystick(joystickIndex))
        {
            Joystick& joystick = platform.joystick(joystickIndex);
            Real value = joystick.axisValue(joystickAxis);
            if (value > joystickAxisDeadZone.x && value < joystickAxisDeadZone.y)
            {
                value = affinity;
            }

            // Scale the value to a range from 0 to 1
            Real delta = value * 0.5 + 0.5;
            if (invert)
            {
                delta = 1.0 - delta;
            }

            _value = interpolate(range.x, range.y, delta);
        }
        else
        {
            _value = affinity;
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
                modifyValue(invert, acceleration * timeStep);
            }
        }
    }
    break;
    default:
        break;
    }

    _value = clamp(_value, range.x, range.y);
}

Real InputAxisBinding::value() const
{
    return _value;
}

void InputAxisBinding::modifyValue(bool invert, Real delta)
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
            << encodeValue("affinity", inputAxisBinding.affinity)
            << encodeValue("range", inputAxisBinding.range)
            << encodeValue("invert", inputAxisBinding.invert)
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
            >> decodeValue("affinity", inputAxisBinding.affinity)
            >> decodeValue("range", inputAxisBinding.range)
            >> decodeValue("invert", inputAxisBinding.invert)
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