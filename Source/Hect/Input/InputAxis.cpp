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

#include "Hect/IO/Encoders/InputAxisEncoder.h"

using namespace hect;

InputAxis::InputAxis() :
    _source(InputAxisSource::Key),
    _positiveMouseButton(MouseButton::Left),
    _negativeMouseButton(MouseButton::Right),
    _positiveKey(Key::A),
    _negativeKey(Key::B),
    _value(0),
    _acceleration(1),
    _gravity(0)
{
}

InputAxis::InputAxis(const std::string& name) :
    _name(name),
    _source(InputAxisSource::Key),
    _positiveMouseButton(MouseButton::Left),
    _negativeMouseButton(MouseButton::Right),
    _positiveKey(Key::A),
    _negativeKey(Key::B),
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

InputAxisSource::Enum InputAxis::source() const
{
    return _source;
}

void InputAxis::setSource(InputAxisSource::Enum source)
{
    _source = source;
}

MouseButton::Enum InputAxis::positiveMouseButton() const
{
    return _positiveMouseButton;
}

void InputAxis::setPositiveMouseButton(MouseButton::Enum button)
{
    _positiveMouseButton = button;
}

MouseButton::Enum InputAxis::negativeMouseButton() const
{
    return _negativeMouseButton;
}

void InputAxis::setNegativeMouseButton(MouseButton::Enum button)
{
    _negativeMouseButton = button;
}

Key::Enum InputAxis::positiveKey() const
{
    return _positiveKey;
}

void InputAxis::setPositiveKey(Key::Enum key)
{
    _positiveKey = key;
}

Key::Enum InputAxis::negativeKey() const
{
    return _negativeKey;
}

void InputAxis::setNegativeKey(Key::Enum key)
{
    _negativeKey = key;
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