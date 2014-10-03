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

#include "Hect/Core/Sequence.h"
#include "Hect/Graphics/Window.h"
#include "Hect/Input/Gamepad.h"
#include "Hect/Input/Keyboard.h"
#include "Hect/Input/Mouse.h"

namespace hect
{

class Platform
{
    typedef std::vector<Gamepad> GamepadContainer;
public:

    ///
    /// A sequence of gamepads.
    typedef Sequence<Gamepad, GamepadContainer> GamepadSequence;

    static void initialize();
    static void deinitialize();

    static void showFatalError(const std::string& message);

    static Window::Pointer createWindow(const std::string& title, const VideoMode& videoMode);

    static bool handleEvents();

    static bool hasMouse();
    static Mouse& mouse();

    static bool hasKeyboard();
    static Keyboard& keyboard();

    static GamepadSequence gamepads();

private:
    Platform() { }
};

}