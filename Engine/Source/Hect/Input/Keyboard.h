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
enum class Key
{
    Unknown,
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    Num0,
    Num1,
    Num2,
    Num3,
    Num4,
    Num5,
    Num6,
    Num7,
    Num8,
    Num9,
    Esc,
    Space,
    Enter,
    Backspace,
    Tab,
    Tick,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    LeftCtrl,
    LeftShift,
    LeftAlt,
};

///
/// A KeyboardEvent type.
enum class KeyboardEventType
{
    ///
    /// A key was pressed down.
    KeyDown,

    ///
    /// A key was released up.
    KeyUp
};

///
/// An event triggered by pressing or releasing a key on the Keyboard.
class HECT_EXPORT KeyboardEvent
{
public:

    ///
    /// The type of the event.
    KeyboardEventType type { KeyboardEventType::KeyDown };

    ///
    /// The key relating to the event.
    Key key { Key::Esc };
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