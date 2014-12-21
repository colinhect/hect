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

void InputAxisBinding::update(Platform& platform, Real timeStepInSeconds)
{
    switch (type)
    {
    case InputAxisBindingType_MouseMoveX:
    {
        if (platform.hasMouse())
        {
            Mouse& mouse = platform.mouse();
            Real delta = mouse.cursorMovement().x;
            if (delta != 0)
            {
                _value += delta * acceleration * timeStepInSeconds;
            }
            else
            {
                _value = affinity;
            }
        }
    }
    break;
    case InputAxisBindingType_MouseMoveY:
    {
        if (platform.hasMouse())
        {
            Mouse& mouse = platform.mouse();
            Real delta = mouse.cursorMovement().y;
            if (delta != 0)
            {
                _value += delta * acceleration * timeStepInSeconds;
            }
            else
            {
                _value = affinity;
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
                _value += acceleration * timeStepInSeconds;
            }
            else
            {
                _value -= acceleration * timeStepInSeconds;
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
                _value += acceleration * timeStepInSeconds;
            }
            else
            {
                _value -= acceleration * timeStepInSeconds;
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
            Real min = std::min(joystickAxisDeadZone.x, joystickAxisDeadZone.y);
            Real max = std::max(joystickAxisDeadZone.x, joystickAxisDeadZone.y);
            if (value > min && value < max)
            {
                value = joystickAxisDeadValue;
            }

            // Scale the value to a range from 0 to 1
            Real delta = value * 0.5 + 0.5;
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
                _value += acceleration * timeStepInSeconds;
            }
            else
            {
                _value -= acceleration * timeStepInSeconds;
            }
        }
    }
    break;
    default:
        break;
    }

    Real min = std::min(range.x, range.y);
    Real max = std::max(range.x, range.y);
    _value = clamp(_value, min, max);
}

Real InputAxisBinding::value() const
{
    return _value;
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const InputAxisBinding& inputAxisBinding)
{
    encoder << beginObject()
            << encodeEnum<InputAxisBindingType>("type", inputAxisBinding.type)
            << encodeEnum<MouseButton>("mouseButton", inputAxisBinding.mouseButton)
            << encodeEnum<Key>("key", inputAxisBinding.key)
            << encodeValue("joystickIndex", inputAxisBinding.joystickIndex)
            << encodeEnum<JoystickAxis>("joystickAxis", inputAxisBinding.joystickAxis)
            << encodeValue("joystickAxisDeadZone", inputAxisBinding.joystickAxisDeadZone)
            << encodeValue("joystickAxisDeadValue", inputAxisBinding.joystickAxisDeadValue)
            << encodeEnum<JoystickButton>("joystickButton", inputAxisBinding.joystickButton)
            << encodeValue("acceleration", inputAxisBinding.acceleration)
            << encodeValue("range", inputAxisBinding.range)
            << encodeValue("affinity", inputAxisBinding.affinity)
            << endObject();

    return encoder;
}

Decoder& operator>>(Decoder& decoder, InputAxisBinding& inputAxisBinding)
{
    decoder >> beginObject()
            >> decodeEnum<InputAxisBindingType>("type", inputAxisBinding.type)
            >> decodeEnum<MouseButton>("mouseButton", inputAxisBinding.mouseButton)
            >> decodeEnum<Key>("key", inputAxisBinding.key)
            >> decodeValue("joystickIndex", inputAxisBinding.joystickIndex)
            >> decodeEnum<JoystickAxis>("joystickAxis", inputAxisBinding.joystickAxis)
            >> decodeValue("joystickAxisDeadZone", inputAxisBinding.joystickAxisDeadZone)
            >> decodeValue("joystickAxisDeadValue", inputAxisBinding.joystickAxisDeadValue)
            >> decodeEnum<JoystickButton>("joystickButton", inputAxisBinding.joystickButton)
            >> decodeValue("acceleration", inputAxisBinding.acceleration)
            >> decodeValue("range", inputAxisBinding.range)
            >> decodeValue("affinity", inputAxisBinding.affinity)
            >> endObject();

    return decoder;
}

}