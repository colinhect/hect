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
    case InputAxisBindingType_Key:
    {
        if (platform.keyboard().isKeyDown(key))
        {
            _value += acceleration * timeStepInSeconds;
        }
        else
        {
            _value -= acceleration * timeStepInSeconds;
        }
    }
        break;
    case InputAxisBindingType_MouseButton:
    {
        if (platform.mouse().isButtonDown(mouseButton))
        {
            _value += acceleration * timeStepInSeconds;
        }
        else
        {
            _value -= acceleration * timeStepInSeconds;
        }
    }
        break;
    case InputAxisBindingType_JoystickButton:
    {
        auto joysticks = platform.joysticks();
        if (joystickIndex < joysticks.size())
        {
            Joystick& joystick = joysticks[joystickIndex];
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
    case InputAxisBindingType_JoystickAxis:
    {
        auto joysticks = platform.joysticks();
        if (joystickIndex < joysticks.size())
        {
            Joystick& joystick = joysticks[joystickIndex];
            _value = joystick.axisValue(joystickAxis);
            if (std::abs(_value) < joystickAxisDeadZone)
            {
                _value = 0;
            }

            // Scale the value to a range from 0 to 1
            _value = _value * 0.5 + 0.5;
        }
    }
        break;
    default:
        break;
    }

    _value = clamp(_value, Real(0), Real(1));

    if (std::abs(_value) < Real(0.001))
    {
        _value = 0;
    }
}

Real InputAxisBinding::value() const
{
    return interpolate(range.x, range.y, _value);
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const InputAxisBinding& inputAxisBinding)
{
    return encoder << beginObject()
           << encodeEnum<InputAxisBindingType>("type", inputAxisBinding.type)
           << encodeEnum<MouseButton>("mouseButton", inputAxisBinding.mouseButton)
           << encodeEnum<Key>("key", inputAxisBinding.key)
           << encodeValue("joystickIndex", inputAxisBinding.joystickIndex)
           << encodeEnum<JoystickAxis>("joystickAxis", inputAxisBinding.joystickAxis)
           << encodeValue("joystickAxisDeadZone", inputAxisBinding.joystickAxisDeadZone)
           << encodeEnum<JoystickButton>("joystickButton", inputAxisBinding.joystickButton)
           << encodeValue("acceleration", inputAxisBinding.acceleration)
           << encodeValue("range", inputAxisBinding.range)
           << endObject();
}

Decoder& operator>>(Decoder& decoder, InputAxisBinding& inputAxisBinding)
{
    return decoder >> beginObject()
           >> decodeEnum<InputAxisBindingType>("type", inputAxisBinding.type)
           >> decodeEnum<MouseButton>("mouseButton", inputAxisBinding.mouseButton)
           >> decodeEnum<Key>("key", inputAxisBinding.key)
           >> decodeValue("joystickIndex", inputAxisBinding.joystickIndex)
           >> decodeEnum<JoystickAxis>("joystickAxis", inputAxisBinding.joystickAxis)
           >> decodeValue("joystickAxisDeadZone", inputAxisBinding.joystickAxisDeadZone)
           >> decodeEnum<JoystickButton>("joystickButton", inputAxisBinding.joystickButton)
           >> decodeValue("acceleration", inputAxisBinding.acceleration)
           >> decodeValue("range", inputAxisBinding.range)
           >> endObject();
}

}