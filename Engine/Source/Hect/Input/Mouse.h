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
#include "Hect/Math/Vector2.h"

namespace hect
{

///
/// A Mouse button.
enum class MouseButton
{
    Button0,
    Button1,
    Button2,
    Button3,
    Button4,
    Button5,
    Button6,
    Button7
};

///
/// The mode a Mouse is in.
enum class MouseMode
{
    ///
    /// The cursor is active.
    Cursor,

    ///
    /// The cursor is inactive and only relative movement is dispatched.
    Relative
};

///
/// A MouseEvent type.
enum class MouseEventType
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

///
/// An event caused by the alteration of the Mouse.
class HECT_EXPORT MouseEvent
{
public:

    ///
    /// The type of the event.
    MouseEventType type { MouseEventType::Movement };

    ///
    /// The mouse button related to the event.
    ///
    /// \note Only relevant for a ::MouseEventType::ButtonDown or
    /// ::MouseEventType::ButtonUp event.
    MouseButton button { MouseButton::Button0 };

    ///
    /// The coordinates of the cursor.
    IntVector2 cursorPosition;

    ///
    /// The movement of the cursor.
    IntVector2 cursorMovement;
};

///
/// Provides access to the system mouse.
class HECT_EXPORT Mouse :
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
    /// Returns the relative motion of the cursor.
    const IntVector2& cursorMovement() const;

    ///
    /// Clears the current relative motion of the cursor.
    void clearMovement();

    ///
    /// Returns the mode.
    MouseMode mode() const;

    ///
    /// Sets the mode.
    ///
    /// \param mode The new mode.
    void setMode(MouseMode mode);

    void enqueueEvent(const MouseEvent& event);
    void dispatchEvents();

private:
    std::vector<MouseEvent> _events;

    MouseMode _mode;
    IntVector2 _cursorPosition;
    IntVector2 _cursorMovement;

    std::vector<bool> _buttonStates;
};

};