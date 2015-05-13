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
#include "Hect/Input/JoystickAxis.h"
#include "Hect/Input/JoystickButton.h"
#include "Hect/Input/JoystickEventType.h"
#include "Hect/Input/JoystickIndex.h"

namespace hect
{

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

}