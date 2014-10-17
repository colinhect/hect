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

#include "Hect/Core/Real.h"
#include "Hect/Input/Gamepad.h"
#include "Hect/Input/Keyboard.h"
#include "Hect/Input/Mouse.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"

namespace hect
{

///
/// A type of input axis source.
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
    /// A gamepad axis.
    InputAxisBindingType_GamepadAxis,

    ///
    /// A gamepad button.
    InputAxisBindingType_GamepadButton
};

///
/// A binding to an input axis.
class InputAxisBinding
{
public:

    ///
    /// Updates the value.
    ///
    /// \param timeStepInSeconds The duration of time elapsed in seconds since
    /// the last update.
    void update(Real timeStepInSeconds);

    ///
    /// Returns the value.
    Real value() const;

    ///
    /// The type.
    InputAxisBindingType type { InputAxisBindingType_Key };

    ///
    /// The mouse button controlling the axis.
    ///
    /// \note Only relevant for when the source is
    /// InputAxisBindingType_MouseButton.
    MouseButton mouseButton { MouseButton_Button0 };

    ///
    /// The key controlling the axis.
    ///
    /// \note Only relevant for when the source is InputAxisBindingType_Key.
    Key key { Key_A };

    ///
    /// The index of the gamepad controlling the axis.
    ///
    /// \note Only relevant for when the source is
    /// InputAxisBindingType_GamepadAxis or InputAxisBindingType_GamepadButton.
    size_t gamepadIndex { 0 };

    ///
    /// The gamepad axis controlling the axis.
    ///
    /// \note Only relevant for when the source is
    /// InputAxisBindingType_GamepadAxis.
    GamepadAxis gamepadAxis { GamepadAxis_Axis0 };

    ///
    /// The dead zone of the gamepad axis controlling the axis.
    ///
    /// \note Only relevant for when the source is
    /// InputAxisBindingType_GamepadAxis.
    Real gamepadAxisDeadZone { 0 };

    ///
    /// The gamepad button controlling the axis.
    ///
    /// \note Only relevant for when the source is
    /// InputAxisBindingType_GamepadButton.
    GamepadButton gamepadButton { GamepadButton_Button0 };

    ///
    /// How fast a discrete source (such as key or mouse button) drives the axis.
    Real acceleration { 1 };

    ///
    /// The range that the binding affects the input axis.
    Vector2 range { 0, 0 };

    friend Encoder& operator<<(Encoder& encoder, const InputAxisBinding& inputAxisBinding);
    friend Decoder& operator>>(Decoder& decoder, InputAxisBinding& inputAxisBinding);

private:
    Real _value { 0 };
};

}