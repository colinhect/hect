///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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

#include "Hect/Core/Export.h"
#include "Hect/Input/Joystick.h"
#include "Hect/Input/Keyboard.h"
#include "Hect/Input/Mouse.h"
#include "Hect/IO/Encodable.h"

namespace hect
{

class Platform;

///
/// An InputAxisBinding type.
enum InputAxisBindingType
{
    ///
    /// A mouse movement along the x axis.
    InputAxisBindingType_MouseMoveX,

    ///
    /// A mouse movement along the y axis.
    InputAxisBindingType_MouseMoveY,

    ///
    /// A mouse button press.
    InputAxisBindingType_MouseButton,

    ///
    /// A mouse scroll.
    InputAxisBindingType_MouseScroll,

    ///
    /// A key press.
    InputAxisBindingType_Key,

    ///
    /// A joystick axis.
    InputAxisBindingType_JoystickAxis,

    ///
    /// A joystick button.
    InputAxisBindingType_JoystickButton
};

///
/// A binding of an input device to the value of an InputAxis.
class HECT_EXPORT InputAxisBinding :
    public Encodable
{
public:

    ///
    /// Updates the contributing value of the binding.
    ///
    /// \param platform The platform.
    /// \param timeStep The duration of time elapsed in seconds since the last
    /// update.
    void update(Platform& platform, double timeStep);

    ///
    /// Returns the contributing value.
    double value() const;

    ///
    /// The type.
    InputAxisBindingType type { InputAxisBindingType_Key };

    ///
    /// The mouse button controlling the axis.
    ///
    /// \note Only relevant when the type is
    /// ::InputAxisBindingType_MouseButton.
    MouseButton mouseButton { MouseButton_Button0 };

    ///
    /// How sensitively mouse motion affects the input axis.
    ///
    /// \note Only relevant when the type is
    /// ::InputAxisBindingType_MouseMoveX or ::InputAxisBindingType_MouseMoveY.
    double mouseSensitivity { 0.01 };

    ///
    /// The key controlling the axis.
    ///
    /// \note Only relevant when the type is ::InputAxisBindingType_Key.
    Key key { Key_A };

    ///
    /// The index of the joystick controlling the axis.
    ///
    /// \note Only relevant when the type is
    /// ::InputAxisBindingType_JoystickAxis or
    /// ::InputAxisBindingType_JoystickButton.
    JoystickIndex joystickIndex { 0 };

    ///
    /// The joystick axis controlling the axis.
    ///
    /// \note Only relevant when the type is
    /// ::InputAxisBindingType_JoystickAxis.
    JoystickAxis joystickAxis { JoystickAxis_Axis0 };

    ///
    /// The dead zone of the joystick axis controlling the axis.
    ///
    /// \note Only relevant when the type is
    /// ::InputAxisBindingType_JoystickAxis.
    Vector2 joystickAxisDeadZone;

    ///
    /// The joystick button controlling the axis.
    ///
    /// \note Only relevant when the type is
    /// ::InputAxisBindingType_JoystickButton.
    JoystickButton joystickButton { JoystickButton_Button0 };

    ///
    /// How quickly the axis is affected by discrete input events.
    ///
    /// \note Only relevant when the type is
    /// ::InputAxisBindingType_MouseButton, ::InputAxisBindingType_MouseScroll,
    /// ::InputAxisBindingType_Key, or ::InputAxisBindingType_JoystickButton.
    double acceleration { 0 };

    ///
    /// How quickly the axis returns to zero.
    ///
    /// \note Only relevant when the type is
    /// ::InputAxisBindingType_MouseButton, ::InputAxisBindingType_MouseScroll,
    /// ::InputAxisBindingType_Key, or ::InputAxisBindingType_JoystickButton.
    double gravity { 0 };

    ///
    /// The range that the binding affects the input axis.
    Vector2 range { -1, 1 };

    ///
    /// Whether the input binding is inverted.
    bool invert { false };

    ///
    /// The value the binding falls back to due to lack of source input.
    double deadValue { 0 };

    void encode(Encoder& encoder) const override;
    void decode(Decoder& decoder) override;

private:
    void applyGravity(double timeStep);
    void modifyValue(double delta);

    double _value { 0 };
};

}