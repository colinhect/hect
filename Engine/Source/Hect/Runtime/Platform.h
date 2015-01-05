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

#include <memory>

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
public:
    typedef std::vector<Joystick> JoystickContainer;

    ///
    /// A sequence of joysticks.
    typedef Sequence<Joystick, JoystickContainer> JoystickSequence;

    virtual ~Platform() { }

    ///
    /// Creates a window in the operating system of the platform.
    ///
    /// \param title The title of the window.
    /// \param videoMode The video mode of the window.
    virtual std::unique_ptr<Window> createWindow(const std::string& title, const VideoMode& videoMode) = 0;

    ///
    /// Handles and dispatches all platform events.
    virtual bool handleEvents() = 0;

    ///
    /// Returns whether the platform has a mouse connected.
    virtual bool hasMouse() = 0;

    ///
    /// Returns the mouse.
    ///
    /// \throws InvalidOperation If the platform does not have a mouse
    /// connected.
    virtual Mouse& mouse() = 0;

    ///
    /// Returns whether the platform has a keyboard connected.
    virtual bool hasKeyboard() = 0;

    ///
    /// Returns the keyboard.
    ///
    /// \throws InvalidOperation If the platform does not have a keyboard
    /// connected.
    virtual Keyboard& keyboard() = 0;

    ///
    /// Returns whether the platform has a joystick connected at the given
    /// index.
    ///
    /// \param index The index of the joystick.
    virtual bool hasJoystick(JoystickIndex index) = 0;

    ///
    /// Returns the joystick connected at the specified index.
    ///
    /// \param index The index of the joystick.
    ///
    /// \throws InvalidOperation If no joystick is connected at the given
    // index.
    virtual Joystick& joystick(JoystickIndex index) = 0;

    ///
    /// Returns any joysticks connected to the platform.
    virtual JoystickSequence joysticks() = 0;
};

}