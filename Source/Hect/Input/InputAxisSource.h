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
#include "Hect/IO/Encodable.h"

namespace hect
{

///
/// A type of input axis source.
enum InputAxisSourceType
{
    ///
    /// A mouse movement along the x axis.
    InputAxisSourceType_MouseMoveX,

    ///
    /// A mouse movement along the y axis.
    InputAxisSourceType_MouseMoveY,

    ///
    /// A mouse button press.
    InputAxisSourceType_MouseButton,

    ///
    /// A mouse scroll.
    InputAxisSourceType_MouseScroll,

    ///
    /// A key press.
    InputAxisSourceType_Key,

    ///
    /// A gamepad axis.
    InputAxisSourceType_GamepadAxis,

    ///
    /// A gamepad button.
    InputAxisSourceType_GamepadButton
};

///
/// A source of an input axis.
class InputAxisSource :
    public Encodable
{
public:

    ///
    /// Constructs a default input axis source.
    InputAxisSource();

    ///
    /// Updates the value.
    ///
    /// \param timeStepInSeconds The duration of time elapsed in seconds since
    /// the last update.
    void update(Real timeStepInSeconds);

    ///
    /// Returns the value.
    Real value() const;

    void encode(ObjectEncoder& encoder) const;
    void decode(ObjectDecoder& decoder, AssetCache& assetCache);

    ///
    /// The type.
    InputAxisSourceType type;

    ///
    /// The mouse button controlling the axis.
    ///
    /// \note Only relevant for when the source is
    /// InputAxisSourceType_MouseButton.
    MouseButton mouseButton;

    ///
    /// The key controlling the axis.
    ///
    /// \note Only relevant for when the source is InputAxisSourceType_Key.
    Key key;

    ///
    /// The index of the gamepad controlling the axis.
    ///
    /// \note Only relevant for when the source is
    /// InputAxisSourceType_GamepadAxis or InputAxisSourceType_GamepadButton.
    size_t gamepadIndex;

    ///
    /// The gamepad axis controlling the axis.
    ///
    /// \note Only relevant for when the source is
    /// InputAxisSourceType_GamepadAxis.
    GamepadAxis gamepadAxis;

    ///
    /// The dead zone of the gamepad axis controlling the axis.
    ///
    /// \note Only relevant for when the source is
    /// InputAxisSourceType_GamepadAxis.
    Real gamepadAxisDeadZone;

    ///
    /// The gamepad button controlling the axis.
    ///
    /// \note Only relevant for when the source is
    /// InputAxisSourceType_GamepadButton.
    GamepadButton gamepadButton;

    Real acceleration;

    Vector2 range;

private:
    Real _value;
};

}