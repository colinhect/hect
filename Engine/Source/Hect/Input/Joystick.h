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

#include "Hect/Core/Event.h"
#include "Hect/Core/Export.h"
#include "Hect/Timing/TimeSpan.h"

namespace hect
{

///
/// An index to a Joystick.
typedef size_t JoystickIndex;

///
/// A Joystick button.
enum JoystickButton
{
    Button0,
    Button1,
    Button2,
    Button3,
    Button4,
    Button5,
    Button6,
    Button7,
    Button8,
    Button9,
    Button11,
    Button12,
    Button13,
    Button14,
    Button15,
    Button16,
    Button17,
    Button18,
    Button19,
    Button20,
    Button21,
    Button22,
    Button23,
    Button24,
    Button25,
    Button26,
    Button27,
    Button28,
    Button29,
    Button30,
    Button31
};

///
/// A Joystick axis.
enum class JoystickAxis
{
    Axis0,
    Axis1,
    Axis2,
    Axis3,
    Axis4,
    Axis5,
    Axis6,
    Axis7,
    Axis8,
    Axis9,
    Axis11,
    Axis12,
    Axis13,
    Axis14,
    Axis15,
    Axis16,
    Axis17,
    Axis18,
    Axis19,
    Axis20,
    Axis21,
    Axis22,
    Axis23,
    Axis24,
    Axis25,
    Axis26,
    Axis27,
    Axis28,
    Axis29,
    Axis30,
    Axis31
};

///
/// A JoystickEvent type.
enum class JoystickEventType
{
    ///
    /// A joystick axis was moved.
    AxisMotion,

    ///
    /// A joystick button was pressed down.
    ButtonDown,

    ///
    /// A joystick button was released up.
    ButtonUp
};

///
/// An event caused by the alteration of a Joystick.
class HECT_EXPORT JoystickEvent
{
public:

    ///
    /// The type of the event.
    JoystickEventType type { JoystickEventType::AxisMotion };

    ///
    /// The index of the joystick related to the event.
    JoystickIndex index { 0 };

    ///
    /// The joystick button related to the event.
    ///
    /// \note Only relevant for a ::JoystickEventType::ButtonDown or
    /// ::JoystickEventType::ButtonUp event.
    JoystickButton button { JoystickButton::Button0 };

    ///
    /// The joystick axis related to the event.
    ///
    /// \note Only relevant for a ::JoystickEventType::AxisMotion event.
    JoystickAxis axis { JoystickAxis::Axis0 };

    ///
    /// The value of the joystick axis related to the event.
    ///
    /// \note Only relevant for a ::JoystickEventType::AxisMotion event.
    double axisValue { 0 };
};

///
/// Provides access to a joystick.
class HECT_EXPORT Joystick :
    public Dispatcher<JoystickEvent>
{
public:
    Joystick(const std::string& name, size_t buttonCount, size_t axisCount);

    ///
    /// Returns whether the given button is down.
    ///
    /// \param button The button to get the state of.
    ///
    /// \throws InvalidOperation If the joystick does not have the given
    /// button.
    bool isButtonDown(JoystickButton button) const;

    ///
    /// Returns the value of an axis of the joystick.
    ///
    /// \param axis The axis to get the value of.
    ///
    /// \throws InvalidOperation If the joystick does not have the given axis.
    double axisValue(JoystickAxis axis) const;

    void enqueueEvent(const JoystickEvent& event);
    void dispatchEvents();

private:
    std::vector<JoystickEvent> _events;

    std::string _name;

    std::vector<bool> _buttonStates;
    std::vector<double> _axisStates;
};

};