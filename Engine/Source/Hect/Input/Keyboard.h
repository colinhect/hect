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

namespace hect
{

///
/// A key on a Keyboard.
enum Key
{
    Key_Unknown,
    Key_A,
    Key_B,
    Key_C,
    Key_D,
    Key_E,
    Key_F,
    Key_G,
    Key_H,
    Key_I,
    Key_J,
    Key_K,
    Key_L,
    Key_M,
    Key_N,
    Key_O,
    Key_P,
    Key_Q,
    Key_R,
    Key_S,
    Key_T,
    Key_U,
    Key_V,
    Key_W,
    Key_X,
    Key_Y,
    Key_Z,
    Key_Num0,
    Key_Num1,
    Key_Num2,
    Key_Num3,
    Key_Num4,
    Key_Num5,
    Key_Num6,
    Key_Num7,
    Key_Num8,
    Key_Num9,
    Key_Esc,
    Key_Space,
    Key_Enter,
    Key_Backspace,
    Key_Tab,
    Key_Tick,
    Key_F1,
    Key_F2,
    Key_F3,
    Key_F4,
    Key_F5,
    Key_F6,
    Key_F7,
    Key_F8,
    Key_F9,
    Key_F10,
    Key_F11,
    Key_F12,
    Key_LeftCtrl,
    Key_LeftShift,
    Key_LeftAlt,
};

///
/// A KeyboardEvent type.
enum KeyboardEventType
{
    ///
    /// A key was pressed down.
    KeyboardEventType_KeyDown,

    ///
    /// A key was released up.
    KeyboardEventType_KeyUp
};

///
/// An event triggered by pressing or releasing a key on the Keyboard.
class HECT_EXPORT KeyboardEvent
{
public:

    ///
    /// The type of the event.
    KeyboardEventType type { KeyboardEventType_KeyDown };

    ///
    /// The key relating to the event.
    Key key { Key_Esc };
};

///
/// Provides access to the system keyboard.
class HECT_EXPORT Keyboard :
    public Dispatcher<KeyboardEvent>
{
public:
    Keyboard();

    ///
    /// Returns whether the given key is down.
    ///
    /// \param key The key to check if it is down.
    bool isKeyDown(Key key) const;

    void enqueueEvent(const KeyboardEvent& event);
    void dispatchEvents();

private:
    std::vector<KeyboardEvent> _events;
    std::vector<bool> _keyStates;
};

};