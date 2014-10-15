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
#include "Hect/Platform/Platform.h"

using namespace hect;

void InputAxisBinding::update(Real timeStepInSeconds)
{
    switch (type)
    {
    case InputAxisBindingType_Key:
    {
        if (Platform::keyboard().isKeyDown(key))
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
        if (Platform::mouse().isButtonDown(mouseButton))
        {
            _value += acceleration * timeStepInSeconds;
        }
        else
        {
            _value -= acceleration * timeStepInSeconds;
        }
    }
    break;
    case InputAxisBindingType_GamepadButton:
    {
        auto gamepads = Platform::gamepads();
        if (gamepadIndex < gamepads.size())
        {
            Gamepad& gamepad = gamepads[gamepadIndex];
            if (gamepad.isButtonDown(gamepadButton))
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
    case InputAxisBindingType_GamepadAxis:
    {
        auto gamepads = Platform::gamepads();
        if (gamepadIndex < gamepads.size())
        {
            Gamepad& gamepad = gamepads[gamepadIndex];
            _value = gamepad.axisValue(gamepadAxis);
            if (std::abs(_value) < gamepadAxisDeadZone)
            {
                _value = 0;
            }

            // Scale the value to a range from 0 to 1
            _value = _value * 0.5 + 0.5;
        }
    }
    break;
    }

    _value = clamp<Real>(_value, 0, 1);

    if (std::abs(_value) < (Real)0.001)
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
           << encodeValue("gamepadIndex", inputAxisBinding.gamepadIndex)
           << encodeEnum<GamepadAxis>("gamepadAxis", inputAxisBinding.gamepadAxis)
           << encodeValue("gamepadAxisDeadZone", inputAxisBinding.gamepadAxisDeadZone)
           << encodeEnum<GamepadButton>("gamepadButton", inputAxisBinding.gamepadButton)
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
           >> decodeValue("gamepadIndex", inputAxisBinding.gamepadIndex)
           >> decodeEnum<GamepadAxis>("gamepadAxis", inputAxisBinding.gamepadAxis)
           >> decodeValue("gamepadAxisDeadZone", inputAxisBinding.gamepadAxisDeadZone)
           >> decodeEnum<GamepadButton>("gamepadButton", inputAxisBinding.gamepadButton)
           >> decodeValue("acceleration", inputAxisBinding.acceleration)
           >> decodeValue("range", inputAxisBinding.range)
           >> endObject();
}

}