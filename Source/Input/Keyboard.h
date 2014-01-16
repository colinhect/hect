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

namespace hect
{

///
/// A key on a keyboard.
enum class Key
{
    A = 0,
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
    Num0 = 26,
    Num1,
    Num2,
    Num3,
    Num4,
    Num5,
    Num6,
    Num7,
    Num8,
    Num9,
    Esc = 36,
    Ctrl,
    Shift,
    Alt,
    Space = 57,
    Enter,
    Backspace,
    Tab,
    Tick = 54,
    F1 = 85,
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
    F12
};

///
/// A keyboard event type.
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
/// An event triggered by pressing or releasing a key on the keyboard.
class KeyboardEvent
{
public:

    ///
    /// Constructs a default event.
    KeyboardEvent();

    ///
    /// The type of the event.
    KeyboardEventType type;

    ///
    /// The key relating to the event.
    Key key;
};

///
/// Provides access to the system keyboard.
class Keyboard
{
    friend class InputSystem;
public:

    ///
    /// Returns whether the given key is down.
    ///
    /// \param key The key to check if it is down.
    bool isKeyDown(Key key) const;

    ///
    /// Returns the dispatcher of keyboard events.
    Dispatcher<KeyboardEvent>& dispatcher();

private:
    Keyboard();

    void _enqueueEvent(const KeyboardEvent& event);
    void _dispatchEvents();

    Dispatcher<KeyboardEvent> _dispatcher;
    std::vector<KeyboardEvent> _events;

    std::vector<bool> _keyStates;
};

};