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

#include "Hect/Core/Event.h"
#include "Hect/Core/Export.h"
#include "Hect/Core/Name.h"
#include "Hect/Input/JoystickAxis.h"
#include "Hect/Input/JoystickButton.h"
#include "Hect/Input/JoystickEvent.h"
#include "Hect/Input/JoystickIndex.h"

namespace hect
{

///
/// Provides access to a joystick.
class HECT_EXPORT Joystick :
    public Dispatcher<JoystickEvent>
{
public:
    Joystick(const Name& name, size_t buttonCount, size_t axisCount);

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

    Name _name;

    std::vector<bool> _buttonStates;
    std::vector<double> _axisStates;
};

}
