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
#include "Hect/Core/Sequence.h"
#include "Hect/Core/Uncopyable.h"
#include "Hect/Input/Joystick.h"
#include "Hect/Input/Keyboard.h"
#include "Hect/Input/Mouse.h"
#include "Hect/Runtime/Window.h"

namespace hect
{

///
/// Provides access to platform functionality and peripherals.
class HECT_EXPORT Platform :
    public Uncopyable
{
    friend class Engine;
public:

    ~Platform();

    ///
    /// Handles and dispatches all platform events.
    bool handle_events();

    ///
    /// Returns whether the platform has a mouse connected.
    bool has_mouse();

    ///
    /// Returns the mouse.
    ///
    /// \throws InvalidOperation If the platform does not have a mouse
    /// connected.
    Mouse& mouse();

    ///
    /// Returns whether the platform has a keyboard connected.
    bool has_keyboard();

    ///
    /// Returns the keyboard.
    ///
    /// \throws InvalidOperation If the platform does not have a keyboard
    /// connected.
    Keyboard& keyboard();

    ///
    /// Returns whether the platform has a joystick connected at the given
    /// index.
    ///
    /// \param index The index of the joystick.
    bool has_joystick(JoystickIndex index);

    ///
    /// Returns the joystick connected at the specified index.
    ///
    /// \param index The index of the joystick.
    ///
    /// \throws InvalidOperation If no joystick is connected at the given
    // index.
    Joystick& joystick(JoystickIndex index);

private:
    Platform();
};

}