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
    /// Constructs a default event.
    GamepadEvent();

    ///
    /// The type of the event.
    GamepadEventType type;

    ///
    /// The index of the gamepad related to the event.
    size_t gamepadIndex;

    ///
    /// The index of the gamepad button related to the event.
    ///
    /// \note Only relevant for a GamepadEventType_ButtonDown or
    /// GamepadEventType_ButtonUp event.
    size_t buttonIndex;

    ///
    /// The index of the gamepad axis related to the event.
    ///
    /// \note Only relevant for a GamepadEventType_AxisMotion event.
    size_t axisIndex;

    ///
    /// The value of the gamepad axis related to the event.
    ///
    /// \note Only relevant for a GamepadEventType_AxisMotion event.
    Real axisValue;
};

///
/// Provides access to a gamepad.
class Gamepad :
    public Dispatcher<GamepadEvent>
{
    friend class Platform;
public:

    ///
    /// Returns the number of buttons on the gamepad.
    size_t buttonCount() const;

    ///
    /// Returns whether the given button is down.
    ///
    /// \param buttonIndex The index of the button to get the state of.
    ///
    /// \throws Error If the gamepad does not have the given button.
    bool isButtonDown(size_t buttonIndex) const;

    ///
    /// Returns the number of axes on the gamepad.
    size_t axisCount() const;

    ///
    /// Returns the value of an axis of the gamepad.
    ///
    /// \param axisIndex The index of the axis to get the value of.
    ///
    /// \throws Error If the gamepad does not have the given axis.
    Real axisValue(size_t axisIndex) const;

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