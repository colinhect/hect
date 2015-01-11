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
#include "NullPlatform.h"

#include "Hect/Runtime/Internal/NullWindow.h"

using namespace hect;

NullPlatform::NullPlatform()
{
    _mouse.reset(new Mouse());
    _keyboard.reset(new Keyboard());
}

bool NullPlatform::handleEvents()
{
    bool active = true;

    _mouse->dispatchEvents();
    _keyboard->dispatchEvents();

    return active;
}

bool NullPlatform::hasMouse()
{
    return true;
}

Mouse& NullPlatform::mouse()
{
    return *_mouse;
}

bool NullPlatform::hasKeyboard()
{
    return true;
}

Keyboard& NullPlatform::keyboard()
{
    return *_keyboard;
}

bool NullPlatform::hasJoystick(JoystickIndex index)
{
    (void)index;
    return false;
}

Joystick& NullPlatform::joystick(JoystickIndex index)
{
    (void)index;
    throw InvalidOperation("No joysticks connected");
}

Platform::JoystickSequence NullPlatform::joysticks()
{
    return _joysticks;
}