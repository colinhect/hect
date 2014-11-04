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

#include "Hect/Core/Configuration.h"
#include "Hect/Runtime/Platform.h"

#ifdef HECT_PLATFORM_SDL

#include <SDL.h>

namespace hect
{

class SdlPlatform :
    public Platform
{
public:
    SdlPlatform(int argc, char* const argv[]);
    ~SdlPlatform();

    Window::Pointer createWindow(const std::string& title, const VideoMode& videoMode) override;

    bool handleEvents() override;

    bool hasMouse() override;
    Mouse& mouse() override;

    bool hasKeyboard() override;
    Keyboard& keyboard() override;

    JoystickSequence joysticks() override;

private:
    std::unique_ptr<Mouse> _mouse;
    std::unique_ptr<Keyboard> _keyboard;
    std::vector<Joystick> _joysticks;
    std::vector<SDL_Joystick*> _openJoysticks;
    MouseMode _mouseMode { MouseMode_Cursor };
};

}

#undef main

#endif