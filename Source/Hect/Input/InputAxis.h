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
#pragma once

#include <vector>

#include "Hect/Core/Real.h"
#include "Hect/Input/Keyboard.h"
#include "Hect/Input/Mouse.h"
#include "Hect/IO/Encodable.h"

namespace hect
{

///
/// The source of an input  axis.
enum InputAxisSource
{
    ///
    /// A mouse movement along the x axis.
    InputAxisSource_MouseMoveX,

    ///
    /// A mouse movement along the y axis.
    InputAxisSource_MouseMoveY,

    ///
    /// A mouse button press.
    InputAxisSource_MouseButton,

    ///
    /// A mouse scroll.
    InputAxisSource_MouseScroll,

    ///
    /// A key press.
    InputAxisSource_Key,

    ///
    /// A joystick axis.
    InputAxisSource_JoystickAxis,

    ///
    /// A joystick button.
    InputAxisSource_JoystickButton
};

///
/// An axis manipulated by an input device.
class InputAxis :
    public Encodable
{
public:

    ///
    /// Constructs an input axis.
    InputAxis();

    ///
    /// Constructs an input axis.
    ///
    /// \param name The name of the axis (must be unique).
    InputAxis(const std::string& name);

    ///
    /// Returns the name.
    const std::string& name() const;

    ///
    /// Sets the name.
    ///
    /// \param name The new name.
    void setName(const std::string& name);

    ///
    /// Returns the source.
    InputAxisSource source() const;

    ///
    /// Sets the source.
    ///
    /// \param source The new source.
    void setSource(InputAxisSource source);

    ///
    /// Returns the mouse button which causes the axis to move in the positive
    /// direction.
    ///
    /// \note Only relevant for when the source is InputAxisSource_MouseButton.
    MouseButton positiveMouseButton() const;

    ///
    /// Sets the mouse button which causes the axis to move in the positive
    /// direction.
    ///
    /// \note Only relevant for when the source is InputAxisSource_MouseButton.
    ///
    /// \param button The new button.
    void setPositiveMouseButton(MouseButton button);

    ///
    /// Returns the mouse button which causes the axis to move in the negative
    /// direction.
    ///
    /// \note Only relevant for when the source is InputAxisSource_MouseButton.
    MouseButton negativeMouseButton() const;

    ///
    /// Sets the mouse button which causes the axis to move in the negative
    /// direction.
    ///
    /// \note Only relevant for when the source is InputAxisSource_MouseButton.
    ///
    /// \param button The new button.
    void setNegativeMouseButton(MouseButton button);

    ///
    /// Returns the key which causes the axis to move in the positive
    /// direction.
    ///
    /// \note Only relevant for when the source is InputAxisSource_Key.
    Key positiveKey() const;

    ///
    /// Sets the key which causes the axis to move in the positive direction.
    ///
    /// \note Only relevant for when the source is InputAxisSource_Key.
    ///
    /// \param key The new key.
    void setPositiveKey(Key key);

    ///
    /// Returns the key which causes the axis to move in the negative
    /// direction.
    ///
    /// \note Only relevant for when the source is InputAxisSource_Key.
    Key negativeKey() const;

    ///
    /// Sets the key which causes the axis to move in the negative direction.
    ///
    /// \note Only relevant for when the source is InputAxisSource_Key.
    ///
    /// \param key The new key.
    void setNegativeKey(Key key);

    ///
    /// Returns the index of the joystick controlling the axis.
    ///
    /// \note Only relevant for when the source is InputAxisSource_JoystickAxis
    /// or InputAxisSource_JoystickButton.
    size_t joystickIndex() const;

    ///
    /// Sets the index of the joystick controlling the axis.
    ///
    /// \note Only relevant for when the source is InputAxisSource_JoystickAxis
    /// or InputAxisSource_JoystickButton.
    ///
    /// \param index The index of the joystick.
    void setJoystickIndex(size_t index);

    ///
    /// Returns the index of the joystick axis controlling the axis.
    ///
    /// \note Only relevant for when the source is
    /// InputAxisSource_JoystickAxis.
    size_t joystickAxisIndex() const;

    ///
    /// Sets the index of the joystick axis controlling the axis.
    ///
    /// \note Only relevant for when the source is
    /// InputAxisSource_JoystickAxis.
    ///
    /// \param index The index of the joystick axis.
    void setJoystickAxisIndex(size_t index);

    ///
    /// Returns the dead zone of the joystick axis controlling the axis.
    ///
    /// \note Only relevant for when the source is
    /// InputAxisSource_JoystickAxis.
    Real joystickAxisDeadZone() const;

    ///
    /// Sets the dead zone of the joystick axis controlling the axis.
    ///
    /// \note Only relevant for when the source is
    /// InputAxisSource_JoystickAxis.
    ///
    /// \param deadZone The dead zone.
    void setJoystickAxisDeadZone(Real deadZone);

    ///
    /// Returns whether the joystick axis controlling the axis should be
    /// inverted.
    ///
    /// \note Only relevant for when the source is
    /// InputAxisSource_JoystickAxis.
    bool joystickAxisInverted() const;

    ///
    /// Sets whether the joystick axis controlling the axis should be
    /// inverted.
    ///
    /// \note Only relevant for when the source is
    /// InputAxisSource_JoystickAxis.
    ///
    /// \param invert True if the axis is inverted; false otherwise.
    void setJoystickAxisInverted(bool invert);

    ///
    /// Returns the index of the joystick button which causes the axis to move
    /// in the positive direction.
    ///
    /// \note Only relevant for when the source is
    /// InputAxisSource_JoystickButton.
    size_t positiveJoystickButtonIndex() const;

    ///
    ///
    /// Sets the index of the joystick button which causes the axis to move in
    /// the positive direction.
    ///
    /// \note Only relevant for when the source is
    /// InputAxisSource_JoystickButton.
    ///
    /// \param index The index of the joystick button.
    void setPositiveJoystickButtonIndex(size_t index);

    ///
    /// Returns the index of the joystick button which causes the axis to move
    /// in the negative direction.
    ///
    /// \note Only relevant for when the source is
    /// InputAxisSource_JoystickButton.
    size_t negativeJoystickButtonIndex() const;

    ///
    ///
    /// Sets the index of the joystick button which causes the axis to move in
    /// the negative direction.
    ///
    /// \note Only relevant for when the source is
    /// InputAxisSource_JoystickButton.
    ///
    /// \param index The index of the joystick button.
    void setNegativeJoystickButtonIndex(size_t index);

    ///
    /// Returns the current value.
    Real value() const;

    ///
    /// Sets the current value.
    ///
    /// \param value The new value.
    void setValue(Real value);

    ///
    /// Returns the acceleration.
    Real acceleration() const;

    ///
    /// Sets the acceleration.
    ///
    /// \param acceleration The new acceleration.
    void setAcceleration(Real acceleration);

    ///
    /// Returns the gravity.
    Real gravity() const;

    ///
    /// Sets the gravity.
    ///
    /// \param gravity The new gravity.
    void setGravity(Real gravity);

    void encode(ObjectEncoder& encoder) const;
    void decode(ObjectDecoder& decoder, AssetCache& assetCache);

private:
    std::string _name;
    InputAxisSource _source;

    MouseButton _positiveMouseButton;
    MouseButton _negativeMouseButton;

    Key _positiveKey;
    Key _negativeKey;

    size_t _joystickIndex;
    size_t _joystickAxisIndex;
    Real _joystickAxisDeadZone;
    bool _joystickAxisInverted;
    size_t _positiveJoystickButtonIndex;
    size_t _negativeJoystickButtonIndex;

    Real _value;
    Real _acceleration;
    Real _gravity;
};

}