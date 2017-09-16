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
#include "Mouse.h"

#include "Hect/Core/Exception.h"

using namespace hect;

Mouse::Mouse() :
    _mode(MouseMode::Cursor),
    _button_states(16, false)
{
}

bool Mouse::is_button_down(MouseButton button) const
{
    assert(static_cast<size_t>(button) >= _button_states.size());
    return _button_states[static_cast<size_t>(button)];
}

const IntVector2& Mouse::cursor_position() const
{
    return _cursor_position;
}

const IntVector2& Mouse::cursor_movement() const
{
    return _cursor_movement;
}

void Mouse::clear_movement()
{
    _cursor_movement = IntVector2();
}

MouseMode Mouse::mode() const
{
    return _mode;
}

void Mouse::set_mode(MouseMode mode)
{
    _mode = mode;
}

void Mouse::enqueue_event(const MouseEvent& event)
{
    // Update the state of the buttons
    if (event.type == MouseEventType::ButtonDown)
    {
        _button_states[static_cast<size_t>(event.button)] = true;
    }
    else if (event.type == MouseEventType::ButtonUp)
    {
        _button_states[static_cast<size_t>(event.button)] = false;
    }

    // Update the relative cursor movement
    if (event.type == MouseEventType::Movement)
    {
        _cursor_movement += event.cursor_movement;
    }

    // Update the absolute cursor position
    _cursor_position = event.cursor_position;

    // Queue the event for dispatch
    _events.push_back(event);
}

void Mouse::dispatch_events()
{
    // Dispatch all queued events to the listeners
    for (const MouseEvent& event : _events)
    {
        dispatch_event(event);
    }
    _events.clear();
}