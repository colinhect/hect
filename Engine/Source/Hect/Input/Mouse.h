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

#include "Hect/Core/EventDispatcher.h"
#include "Hect/Core/Export.h"
#include "Hect/Input/MouseButton.h"
#include "Hect/Input/MouseMode.h"
#include "Hect/Input/MouseEvent.h"
#include "Hect/Math/Vector2.h"

namespace hect
{

///
/// Provides access to the system mouse.
class HECT_EXPORT Mouse :
    public EventDispatcher<MouseEvent>
{
public:
    Mouse();

    ///
    /// Returns whether the given button is down.
    ///
    /// \param button The button to check if it is down.
    bool is_button_down(MouseButton button) const;

    ///
    /// Returns the position of the cursor in window space.
    const IntVector2& cursor_position() const;

    ///
    /// Returns the relative motion of the cursor.
    const IntVector2& cursor_movement() const;

    ///
    /// Clears the current relative motion of the cursor.
    void clear_movement();

    ///
    /// Returns the mode.
    MouseMode mode() const;

    ///
    /// Sets the mode.
    ///
    /// \param mode The new mode.
    void set_mode(MouseMode mode);

    void enqueue_event(const MouseEvent& event);
    void dispatch_events();

private:
    std::vector<MouseEvent> _events;

    MouseMode _mode;
    IntVector2 _cursor_position;
    IntVector2 _cursor_movement;

    std::vector<bool> _button_states;
};

}
