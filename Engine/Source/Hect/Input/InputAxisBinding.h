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
#pragma once

#include "Hect/Core/Export.h"
#include "Hect/Input/InputAxisBindingType.h"
#include "Hect/Input/Joystick.h"
#include "Hect/Input/Keyboard.h"
#include "Hect/Input/Mouse.h"
#include "Hect/IO/Encodable.h"
#include "Hect/Units/Time.h"

namespace hect
{

class Platform;

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
    void update(Platform& platform, Seconds timeStep);

    ///
    /// Returns the contributing value.
    double value() const;

    ///
    /// The type.
    InputAxisBindingType type { InputAxisBindingType::Key };

    ///
    /// The mouse button controlling the axis.
    ///
    /// \note Only relevant when the type is
    /// ::InputAxisBindingType::MouseButton.
    MouseButton mouseButton { MouseButton::Button0 };

    ///
    /// How sensitively mouse motion affects the input axis.
    ///
    /// \note Only relevant when the type is
    /// ::InputAxisBindingType::MouseMoveX or ::InputAxisBindingType::MouseMoveY.
    double mouseSensitivity { 0.01 };

    ///
    /// The key controlling the axis.
    ///
    /// \note Only relevant when the type is ::InputAxisBindingType::Key.
    Key key { Key::A };

    ///
    /// The index of the joystick controlling the axis.
    ///
    /// \note Only relevant when the type is
    /// ::InputAxisBindingType::JoystickAxis or
    /// ::InputAxisBindingType::JoystickButton.
    JoystickIndex joystickIndex { 0 };

    ///
    /// The joystick axis controlling the axis.
    ///
    /// \note Only relevant when the type is
    /// ::InputAxisBindingType::JoystickAxis.
    JoystickAxis joystickAxis { JoystickAxis::Axis0 };

    ///
    /// The dead zone of the joystick axis controlling the axis.
    ///
    /// \note Only relevant when the type is
    /// ::InputAxisBindingType::JoystickAxis.
    Vector2 joystickAxisDeadZone;

    ///
    /// The joystick button controlling the axis.
    ///
    /// \note Only relevant when the type is
    /// ::InputAxisBindingType::JoystickButton.
    JoystickButton joystickButton { JoystickButton::Button0 };

    ///
    /// How quickly the axis is affected by discrete input events.
    ///
    /// \note Only relevant when the type is
    /// ::InputAxisBindingType::MouseButton, ::InputAxisBindingType::MouseScroll,
    /// ::InputAxisBindingType::Key, or ::InputAxisBindingType::JoystickButton.
    double acceleration { 0 };

    ///
    /// How quickly the axis returns to zero.
    ///
    /// \note Only relevant when the type is
    /// ::InputAxisBindingType::MouseButton, ::InputAxisBindingType::MouseScroll,
    /// ::InputAxisBindingType::Key, or ::InputAxisBindingType::JoystickButton.
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
    void applyGravity(Seconds timeStep);
    void modifyValue(double delta);

    double _value { 0 };
};

}
