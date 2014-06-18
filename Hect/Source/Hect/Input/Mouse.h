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

#include "Hect/Core/Export.h"
#include "Hect/Core/Dispatcher.h"
#include "Hect/Math/Vector2.h"

namespace hect
{

///
/// The mode a mouse is in.
namespace MouseMode
{
enum Enum
{
    ///
    /// The cursor is active.
    Cursor,

    ///
    /// The cursor is inactive and only relative movement is dispatched.
    Relative
};
}

///
/// A mouse button.
namespace MouseButton
{
enum Enum
{
    ///
    /// The left mouse button.
    Left,

    ///
    /// The right mouse button.
    Right,

    ///
    /// The middle mouse button.
    Middle
};
}

///
/// A mouse event type.
namespace MouseEventType
{
enum Enum
{
    ///
    /// The mouse was moved.
    Movement,

    ///
    /// A mouse button was pressed down.
    ButtonDown,

    ///
    /// A mouse button was released up.
    ButtonUp,

    ///
    /// The scroll wheel was moved down.
    ScrollDown,

    ///
    /// The scroll wheel was moved up.
    ScrollUp
};
}

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
    MouseEventType::Enum type;

    ///
    /// The mouse button related to the event.
    ///
    /// \note Only relevant for a MouseEvent::ButtonDown or
    /// MouseEvent::ButtonUp event.
    MouseButton::Enum button;

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
    friend class InputSystem;
public:

    ///
    /// Returns whether the given button is down.
    ///
    /// \param button The button to check if it is down.
    bool isButtonDown(MouseButton::Enum button) const;

    ///
    /// Returns the position of the cursor in window space.
    const IntVector2& cursorPosition() const;

    ///
    /// Sets the mode.
    ///
    /// \param mode The new mode.
    void setMode(MouseMode::Enum mode);

    ///
    /// Returns the mode.
    MouseMode::Enum mode() const;

    ///
    /// Returns the dispatcher of mouse events.
    Dispatcher<MouseEvent>& dispatcher();

private:
    Mouse();

    void _enqueueEvent(const MouseEvent& event);
    void _dispatchEvents();

    Dispatcher<MouseEvent> _dispatcher;
    std::vector<MouseEvent> _events;

    MouseMode::Enum _mode;
    IntVector2 _cursorPosition;

    std::vector<bool> _buttonStates;
};

};