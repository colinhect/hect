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
    MouseButton_Button0,
    MouseButton_Button1,
    MouseButton_Button2,
    MouseButton_Button3,
    MouseButton_Button4,
    MouseButton_Button5,
    MouseButton_Button6,
    MouseButton_Button7
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
    /// The type of the event.
    MouseEventType type { MouseEventType_Movement };

    ///
    /// The mouse button related to the event.
    ///
    /// \note Only relevant for a MouseEventType_ButtonDown or
    /// MouseEventType_ButtonUp event.
    MouseButton button { MouseButton_Button0 };

    ///
    /// The coordinates of the cursor.
    IntVector2 cursorPosition;

    ///
    /// The movement of the cursor.
    IntVector2 cursorMovement;
};

///
/// Provides access to the system mouse.
class Mouse :
    public Dispatcher<MouseEvent>
{
public:
    Mouse();

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

    void enqueueEvent(const MouseEvent& event);
    void dispatchEvents();

private:
    std::vector<MouseEvent> _events;

    MouseMode _mode;
    IntVector2 _cursorPosition;

    std::vector<bool> _buttonStates;
};

};