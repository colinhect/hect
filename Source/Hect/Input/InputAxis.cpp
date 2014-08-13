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
#include "InputAxis.h"

#include <algorithm>

#include "Hect/IO/Encoders/InputAxisEncoder.h"

using namespace hect;

InputAxis::InputAxis() :
    _source(InputAxisSource_Key),
    _positiveMouseButton(MouseButton_Left),
    _negativeMouseButton(MouseButton_Right),
    _positiveKey(Key_A),
    _negativeKey(Key_B),
    _gamepadIndex(0),
    _gamepadAxisIndex(0),
    _gamepadAxisDeadZone(0),
    _gamepadAxisInverted(false),
    _positiveGamepadButtonIndex(0),
    _negativeGamepadButtonIndex(0),
    _value(0),
    _acceleration(1),
    _gravity(0)
{
}

InputAxis::InputAxis(const std::string& name) :
    _name(name),
    _source(InputAxisSource_Key),
    _positiveMouseButton(MouseButton_Left),
    _negativeMouseButton(MouseButton_Right),
    _positiveKey(Key_A),
    _negativeKey(Key_B),
    _gamepadIndex(0),
    _gamepadAxisIndex(0),
    _gamepadAxisDeadZone(0),
    _gamepadAxisInverted(false),
    _positiveGamepadButtonIndex(0),
    _negativeGamepadButtonIndex(0),
    _value(0),
    _acceleration(1),
    _gravity(0)
{
}

const std::string& InputAxis::name() const
{
    return _name;
}

void InputAxis::setName(const std::string& name)
{
    _name = name;
}

InputAxisSource InputAxis::source() const
{
    return _source;
}

void InputAxis::setSource(InputAxisSource source)
{
    _source = source;
}

MouseButton InputAxis::positiveMouseButton() const
{
    return _positiveMouseButton;
}

void InputAxis::setPositiveMouseButton(MouseButton button)
{
    _positiveMouseButton = button;
}

MouseButton InputAxis::negativeMouseButton() const
{
    return _negativeMouseButton;
}

void InputAxis::setNegativeMouseButton(MouseButton button)
{
    _negativeMouseButton = button;
}

Key InputAxis::positiveKey() const
{
    return _positiveKey;
}

void InputAxis::setPositiveKey(Key key)
{
    _positiveKey = key;
}

Key InputAxis::negativeKey() const
{
    return _negativeKey;
}

void InputAxis::setNegativeKey(Key key)
{
    _negativeKey = key;
}

size_t InputAxis::gamepadIndex() const
{
    return _gamepadIndex;
}

void InputAxis::setGamepadIndex(size_t index)
{
    _gamepadIndex = index;
}

size_t InputAxis::gamepadAxisIndex() const
{
    return _gamepadAxisIndex;
}

void InputAxis::setGamepadAxisIndex(size_t index)
{
    _gamepadAxisIndex = index;
}

Real InputAxis::gamepadAxisDeadZone() const
{
    return _gamepadAxisDeadZone;
}

void InputAxis::setGamepadAxisDeadZone(Real deadZone)
{
    _gamepadAxisDeadZone = deadZone;
}

bool InputAxis::gamepadAxisInverted() const
{
    return _gamepadAxisInverted;
}

void InputAxis::setGamepadAxisInverted(bool invert)
{
    _gamepadAxisInverted = invert;
}

size_t InputAxis::positiveGamepadButtonIndex() const
{
    return _positiveGamepadButtonIndex;
}

void InputAxis::setPositiveGamepadButtonIndex(size_t index)
{
    _positiveGamepadButtonIndex = index;
}

size_t InputAxis::negativeGamepadButtonIndex() const
{
    return _negativeGamepadButtonIndex;
}

void InputAxis::setNegativeGamepadButtonIndex(size_t index)
{
    _negativeGamepadButtonIndex = index;
}

Real InputAxis::value() const
{
    return _value;
}

void InputAxis::setValue(Real value)
{
    _value = value;
    _value = std::max<Real>(-1, _value);
    _value = std::min<Real>(1, _value);
}

Real InputAxis::acceleration() const
{
    return _acceleration;
}

void InputAxis::setAcceleration(Real acceleration)
{
    _acceleration = acceleration;
}

Real InputAxis::gravity() const
{
    return _gravity;
}

void InputAxis::setGravity(Real gravity)
{
    _gravity = gravity;
}

void InputAxis::encode(ObjectEncoder& encoder) const
{
    encoder;
    InputAxisEncoder::encode(*this, encoder);
}

void InputAxis::decode(ObjectDecoder& decoder, AssetCache& assetCache)
{
    assetCache;
    InputAxisEncoder::decode(*this, decoder);
}