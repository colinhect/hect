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
#include "Hect/Event/Dispatcher.h"
#include "Hect/Timing/TimeSpan.h"

namespace hect
{

///
/// A gamepad button.
enum GamepadButton
{
    GamepadButton_Button0,
    GamepadButton_Button1,
    GamepadButton_Button2,
    GamepadButton_Button3,
    GamepadButton_Button4,
    GamepadButton_Button5,
    GamepadButton_Button6,
    GamepadButton_Button7,
    GamepadButton_Button8,
    GamepadButton_Button9,
    GamepadButton_Button11,
    GamepadButton_Button12,
    GamepadButton_Button13,
    GamepadButton_Button14,
    GamepadButton_Button15,
    GamepadButton_Button16,
    GamepadButton_Button17,
    GamepadButton_Button18,
    GamepadButton_Button19,
    GamepadButton_Button20,
    GamepadButton_Button21,
    GamepadButton_Button22,
    GamepadButton_Button23,
    GamepadButton_Button24,
    GamepadButton_Button25,
    GamepadButton_Button26,
    GamepadButton_Button27,
    GamepadButton_Button28,
    GamepadButton_Button29,
    GamepadButton_Button30,
    GamepadButton_Button31
};

///
/// A gamepad axis.
enum GamepadAxis
{
    GamepadAxis_Axis0,
    GamepadAxis_Axis1,
    GamepadAxis_Axis2,
    GamepadAxis_Axis3,
    GamepadAxis_Axis4,
    GamepadAxis_Axis5,
    GamepadAxis_Axis6,
    GamepadAxis_Axis7,
    GamepadAxis_Axis8,
    GamepadAxis_Axis9,
    GamepadAxis_Axis11,
    GamepadAxis_Axis12,
    GamepadAxis_Axis13,
    GamepadAxis_Axis14,
    GamepadAxis_Axis15,
    GamepadAxis_Axis16,
    GamepadAxis_Axis17,
    GamepadAxis_Axis18,
    GamepadAxis_Axis19,
    GamepadAxis_Axis20,
    GamepadAxis_Axis21,
    GamepadAxis_Axis22,
    GamepadAxis_Axis23,
    GamepadAxis_Axis24,
    GamepadAxis_Axis25,
    GamepadAxis_Axis26,
    GamepadAxis_Axis27,
    GamepadAxis_Axis28,
    GamepadAxis_Axis29,
    GamepadAxis_Axis30,
    GamepadAxis_Axis31
};

///
/// A gamepad event type.
enum GamepadEventType
{
    ///
    /// A gamepad axis was moved.
    GamepadEventType_AxisMotion,

    ///
    /// A gamepad button was pressed down.
    GamepadEventType_ButtonDown,

    ///
    /// A gamepad button was released up.
    GamepadEventType_ButtonUp
};

///
/// An event caused by the alteration of a gamepad.
class GamepadEvent
{
public:

    ///
    /// The type of the event.
    GamepadEventType type { GamepadEventType_AxisMotion };

    ///
    /// The index of the gamepad related to the event.
    size_t gamepadIndex { 0 };

    ///
    /// The gamepad button related to the event.
    ///
    /// \note Only relevant for a GamepadEventType_ButtonDown or
    /// GamepadEventType_ButtonUp event.
    GamepadButton button { GamepadButton_Button0 };

    ///
    /// The gamepad axis related to the event.
    ///
    /// \note Only relevant for a GamepadEventType_AxisMotion event.
    GamepadAxis axis { GamepadAxis_Axis0 };

    ///
    /// The value of the gamepad axis related to the event.
    ///
    /// \note Only relevant for a GamepadEventType_AxisMotion event.
    Real axisValue { 0 };
};

///
/// Provides access to a gamepad.
class Gamepad :
    public Dispatcher<GamepadEvent>
{
    friend class Platform;
public:

    ///
    /// Returns whether the given button is down.
    ///
    /// \param button The button to get the state of.
    ///
    /// \throws Error If the gamepad does not have the given button.
    bool isButtonDown(GamepadButton button) const;

    ///
    /// Returns the value of an axis of the gamepad.
    ///
    /// \param axis The axis to get the value of.
    ///
    /// \throws Error If the gamepad does not have the given axis.
    Real axisValue(GamepadAxis axis) const;

private:
    Gamepad(const std::string& name, size_t buttonCount, size_t axisCount);

    void enqueueEvent(const GamepadEvent& event);
    void dispatchEvents();

    std::vector<GamepadEvent> _events;

    std::string _name;

    std::vector<bool> _buttonStates;
    std::vector<Real> _axisStates;
};

};