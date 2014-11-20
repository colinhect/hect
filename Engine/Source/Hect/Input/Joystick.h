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
/// A joystick button.
enum JoystickButton
{
    JoystickButton_Button0,
    JoystickButton_Button1,
    JoystickButton_Button2,
    JoystickButton_Button3,
    JoystickButton_Button4,
    JoystickButton_Button5,
    JoystickButton_Button6,
    JoystickButton_Button7,
    JoystickButton_Button8,
    JoystickButton_Button9,
    JoystickButton_Button11,
    JoystickButton_Button12,
    JoystickButton_Button13,
    JoystickButton_Button14,
    JoystickButton_Button15,
    JoystickButton_Button16,
    JoystickButton_Button17,
    JoystickButton_Button18,
    JoystickButton_Button19,
    JoystickButton_Button20,
    JoystickButton_Button21,
    JoystickButton_Button22,
    JoystickButton_Button23,
    JoystickButton_Button24,
    JoystickButton_Button25,
    JoystickButton_Button26,
    JoystickButton_Button27,
    JoystickButton_Button28,
    JoystickButton_Button29,
    JoystickButton_Button30,
    JoystickButton_Button31
};

///
/// A joystick axis.
enum JoystickAxis
{
    JoystickAxis_Axis0,
    JoystickAxis_Axis1,
    JoystickAxis_Axis2,
    JoystickAxis_Axis3,
    JoystickAxis_Axis4,
    JoystickAxis_Axis5,
    JoystickAxis_Axis6,
    JoystickAxis_Axis7,
    JoystickAxis_Axis8,
    JoystickAxis_Axis9,
    JoystickAxis_Axis11,
    JoystickAxis_Axis12,
    JoystickAxis_Axis13,
    JoystickAxis_Axis14,
    JoystickAxis_Axis15,
    JoystickAxis_Axis16,
    JoystickAxis_Axis17,
    JoystickAxis_Axis18,
    JoystickAxis_Axis19,
    JoystickAxis_Axis20,
    JoystickAxis_Axis21,
    JoystickAxis_Axis22,
    JoystickAxis_Axis23,
    JoystickAxis_Axis24,
    JoystickAxis_Axis25,
    JoystickAxis_Axis26,
    JoystickAxis_Axis27,
    JoystickAxis_Axis28,
    JoystickAxis_Axis29,
    JoystickAxis_Axis30,
    JoystickAxis_Axis31
};

///
/// A joystick event type.
enum JoystickEventType
{
    ///
    /// A joystick axis was moved.
    JoystickEventType_AxisMotion,

    ///
    /// A joystick button was pressed down.
    JoystickEventType_ButtonDown,

    ///
    /// A joystick button was released up.
    JoystickEventType_ButtonUp
};

///
/// An event caused by the alteration of a joystick.
class JoystickEvent
{
public:

    ///
    /// The type of the event.
    JoystickEventType type { JoystickEventType_AxisMotion };

    ///
    /// The index of the joystick related to the event.
    size_t joystickIndex { 0 };

    ///
    /// The joystick button related to the event.
    ///
    /// \note Only relevant for a ::JoystickEventType_ButtonDown or
    /// ::JoystickEventType_ButtonUp event.
    JoystickButton button { JoystickButton_Button0 };

    ///
    /// The joystick axis related to the event.
    ///
    /// \note Only relevant for a ::JoystickEventType_AxisMotion event.
    JoystickAxis axis { JoystickAxis_Axis0 };

    ///
    /// The value of the joystick axis related to the event.
    ///
    /// \note Only relevant for a ::JoystickEventType_AxisMotion event.
    Real axisValue { 0 };
};

///
/// Provides access to a joystick.
class Joystick :
    public Dispatcher<JoystickEvent>
{
public:
    Joystick(const std::string& name, size_t buttonCount, size_t axisCount);

    ///
    /// Returns whether the given button is down.
    ///
    /// \param button The button to get the state of.
    ///
    /// \throws Error If the joystick does not have the given button.
    bool isButtonDown(JoystickButton button) const;

    ///
    /// Returns the value of an axis of the joystick.
    ///
    /// \param axis The axis to get the value of.
    ///
    /// \throws Error If the joystick does not have the given axis.
    Real axisValue(JoystickAxis axis) const;

    void enqueueEvent(const JoystickEvent& event);
    void dispatchEvents();

private:
    std::vector<JoystickEvent> _events;

    std::string _name;

    std::vector<bool> _buttonStates;
    std::vector<Real> _axisStates;
};

};