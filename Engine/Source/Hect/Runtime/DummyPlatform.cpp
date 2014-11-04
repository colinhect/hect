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
#include "DummyPlatform.h"

using namespace hect;

class DummyWindow :
    public Window
{
public:

    DummyWindow(const std::string& title, const VideoMode& videoMode) :
        Window(title, videoMode)
    {
    }

    void swapBuffers()
    {
    }
};

DummyPlatform::DummyPlatform()
{
    _mouse.reset(new Mouse());
    _keyboard.reset(new Keyboard());
}

Window::Pointer DummyPlatform::createWindow(const std::string& title, const VideoMode& videoMode)
{
    return Window::Pointer(new DummyWindow(title, videoMode));
}

bool DummyPlatform::handleEvents()
{
    bool active = true;

    _mouse->dispatchEvents();
    _keyboard->dispatchEvents();

    return active;
}

bool DummyPlatform::hasMouse()
{
    return true;
}

Mouse& DummyPlatform::mouse()
{
    return *_mouse;
}

bool DummyPlatform::hasKeyboard()
{
    return true;
}

Keyboard& DummyPlatform::keyboard()
{
    return *_keyboard;
}

Platform::JoystickSequence DummyPlatform::joysticks()
{
    return _joysticks;
}