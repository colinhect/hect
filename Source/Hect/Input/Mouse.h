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

#include "Hect/Event/Dispatcher.h"
#include "Hect/Math/Vector2.h"

namespace hect
{

///
/// A mouse button.
enum MouseButton
{
    ///
    /// The left mouse button.
    MouseButton_Left,

    ///
    /// The right mouse button.
    MouseButton_Right,

    ///
    /// The middle mouse button.
    MouseButton_Middle
};

///
/// The mode a mouse is in.
enum MouseMode
{
    ///
    /// The cursor is active.
    MouseMode_Cursor,

    ///
    /// The cursor is inactive and only relative movement is dispatched.
    MouseMode_Relative
};

///
/// A mouse event type.
enum MouseEventType
{
    ///
    /// The mouse was moved.
    MouseEventType_Movement,

    ///
    /// A mouse button was pressed down.
    MouseEventType_ButtonDown,

    ///
    /// A mouse button was released up.
    MouseEventType_ButtonUp,

    ///
    /// The scroll wheel was moved down.
    MouseEventType_ScrollDown,

    ///
    /// The scroll wheel was moved up.
    MouseEventType_ScrollUp
};

///
/// An event caused by the alteration of the mouse.
class MouseEvent
{
public:

    ///
    /// Constructs a default event.
    MouseEvent();

    ///
    /// The type of the event.
    MouseEventType type;

    ///
    /// The mouse button related to the event.
    ///
    /// \note Only relevant for a MouseEventType_ButtonDown or
    /// MouseEventType_ButtonUp event.
    MouseButton button;

    ///
    /// The coordinates of the cursor.
    IntVector2 cursorPosition;

    ///
    /// The movement of the cursor.
    IntVector2 cursorMovement;
};

///
/// Provides access to the system mouse.
class Mouse
{
    friend class InputDevices;
public:

    ///
    /// Returns whether the given button is down.
    ///
    /// \param button The button to check if it is down.
    bool isButtonDown(MouseButton button) const;

    ///
    /// Returns the position of the cursor in window space.
    const IntVector2& cursorPosition() const;

    ///
    /// Sets the mode.
    ///
    /// \param mode The new mode.
    void setMode(MouseMode mode);

    ///
    /// Returns the mode.
    MouseMode mode() const;

    ///
    /// Returns the dispatcher of mouse events.
    Dispatcher<MouseEvent>& dispatcher();

private:
    Mouse();

    void enqueueEvent(const MouseEvent& event);
    void dispatchEvents();

    Dispatcher<MouseEvent> _dispatcher;
    std::vector<MouseEvent> _events;

    MouseMode _mode;
    IntVector2 _cursorPosition;

    std::vector<bool> _buttonStates;
};

};