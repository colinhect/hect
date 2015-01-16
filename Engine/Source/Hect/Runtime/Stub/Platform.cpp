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
#include "Hect/Runtime/Platform.h"

#include "Hect/Core/Configuration.h"

#ifndef HECT_PLATFORM_SDL

using namespace hect;

namespace
{

Mouse _mouse;
Keyboard _keyboard;
std::vector<Joystick> _joysticks;

}

Platform::~Platform()
{
}

bool Platform::handleEvents()
{
    bool active = true;

    _mouse.dispatchEvents();
    _keyboard.dispatchEvents();

    return active;
}

bool Platform::hasMouse()
{
    return true;
}

Mouse& Platform::mouse()
{
    return _mouse;
}

bool Platform::hasKeyboard()
{
    return true;
}

Keyboard& Platform::keyboard()
{
    return _keyboard;
}

bool Platform::hasJoystick(JoystickIndex index)
{
    (void)index;
    return false;
}

Joystick& Platform::joystick(JoystickIndex index)
{
    (void)index;
    throw InvalidOperation("No joysticks connected");
}

Platform::JoystickSequence Platform::joysticks()
{
    return _joysticks;
}

Platform::Platform()
{
}

#endif