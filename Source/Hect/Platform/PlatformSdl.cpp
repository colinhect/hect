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
#include "Platform.h"

#include <algorithm>
#include <SDL.h>

#include "Hect/Core/Configuration.h"
#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"
#include "Hect/Core/Logging.h"
#include "Hect/Platform/FileSystem.h"

#ifdef HECT_WINDOWS_BUILD
#include <Windows.h>
#endif

using namespace hect;

namespace
{

static Mouse _mouse;
static Keyboard _keyboard;
static std::vector<Joystick> _joysticks;
static std::vector<SDL_Joystick*> _sdlJoysticks;
static std::vector<SDL_Window*> _sdlWindows;
static std::vector<SDL_GLContext> _sdlContexts;
static MouseMode _mouseMode = MouseMode_Cursor;

Key convertKey(SDL_Keycode key)
{
    switch (key)
    {
    case SDLK_a:
        return Key_A;
    case SDLK_b:
        return Key_B;
    case SDLK_c:
        return Key_C;
    case SDLK_d:
        return Key_D;
    case SDLK_e:
        return Key_E;
    case SDLK_f:
        return Key_F;
    case SDLK_g:
        return Key_G;
    case SDLK_h:
        return Key_H;
    case SDLK_i:
        return Key_I;
    case SDLK_j:
        return Key_J;
    case SDLK_k:
        return Key_K;
    case SDLK_l:
        return Key_L;
    case SDLK_m:
        return Key_M;
    case SDLK_n:
        return Key_N;
    case SDLK_o:
        return Key_O;
    case SDLK_p:
        return Key_P;
    case SDLK_q:
        return Key_Q;
    case SDLK_r:
        return Key_R;
    case SDLK_s:
        return Key_S;
    case SDLK_t:
        return Key_T;
    case SDLK_u:
        return Key_U;
    case SDLK_v:
        return Key_V;
    case SDLK_w:
        return Key_W;
    case SDLK_x:
        return Key_X;
    case SDLK_y:
        return Key_Y;
    case SDLK_z:
        return Key_Z;
    case SDLK_ESCAPE:
        return Key_Esc;
    case SDLK_TAB:
        return Key_Tab;

    case SDLK_F1:
        return Key_F1;
    case SDLK_F2:
        return Key_F2;
    case SDLK_F3:
        return Key_F3;
    case SDLK_F4:
        return Key_F4;
    case SDLK_F5:
        return Key_F5;
    case SDLK_F6:
        return Key_F6;
    case SDLK_F7:
        return Key_F7;
    case SDLK_F8:
        return Key_F8;
    case SDLK_F9:
        return Key_F9;
    case SDLK_F10:
        return Key_F10;
    case SDLK_F11:
        return Key_F11;
    case SDLK_F12:
        return Key_F12;

    case SDLK_LCTRL:
        return Key_LeftCtrl;
    case SDLK_LSHIFT:
        return Key_LeftShift;
    case SDLK_LALT:
        return Key_LeftAlt;

    default:
        return Key_Unknown;
    }
}

}

void Platform::initialize()
{
    FileSystem::initialize();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC) != 0)
    {
        throw Error(format("Failed to initialize SDL: %s", SDL_GetError()));
    }

    SDL_version version;
    SDL_GetVersion(&version);
    HECT_INFO(format("SDL version %d.%d.%d", version.major, version.minor, version.patch));

    for (int i = 0; i < SDL_NumJoysticks(); i++)
    {
        SDL_Joystick* joystick = SDL_JoystickOpen(i);
        if (joystick)
        {
            _sdlJoysticks.push_back(joystick);

            std::string name = SDL_JoystickName(joystick);
            size_t buttonCount = SDL_JoystickNumButtons(joystick);
            size_t axisCount = SDL_JoystickNumAxes(joystick);

            HECT_INFO(format("Detected joystick '%s' with %i buttons and %i axes", name.c_str(), buttonCount, axisCount));

            _joysticks.push_back(Joystick(name, buttonCount, axisCount));
        }
    }
}

void Platform::deinitialize()
{
    // Close all joysticks
    for (SDL_Joystick* joystick : _sdlJoysticks)
    {
        SDL_JoystickClose(joystick);
    }
    _sdlJoysticks.clear();

    SDL_Quit();

    FileSystem::deinitialize();
}

void Platform::showFatalError(const std::string& message)
{
#ifdef HECT_WINDOWS_BUILD
    MessageBoxA(NULL, message.c_str(), "Fatal Error", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#endif
}

WindowHandle Platform::createWindow(const std::string& title, const VideoMode& videoMode)
{
    WindowHandle handle = _sdlWindows.size();

    // Create the window flags
    uint32_t flags = SDL_WINDOW_OPENGL;
    if (videoMode.isFullscreen())
    {
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    // Create the window
    SDL_Window* window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, videoMode.width(), videoMode.height(), SDL_WINDOW_OPENGL);
    if (!window)
    {
        throw Error(format("Failed to create SDL window: %s", SDL_GetError()));
    }

    // Create the OpenGL context
    SDL_GLContext context = SDL_GL_CreateContext(window);

    _sdlWindows.push_back(window);
    _sdlContexts.push_back(context);

    return handle;
}

void Platform::destroyWindow(WindowHandle handle)
{
    if (handle < _sdlWindows.size())
    {
        SDL_GLContext context = _sdlContexts[handle];
        if (context)
        {
            SDL_GL_DeleteContext(context);
            _sdlContexts[handle] = nullptr;
        }

        SDL_Window* window = _sdlWindows[handle];
        if (window)
        {
            SDL_DestroyWindow(window);
            _sdlWindows[handle] = nullptr;
        }
    }
}

void Platform::swapBuffers(WindowHandle handle)
{
    if (handle < _sdlWindows.size())
    {
        SDL_Window* window = _sdlWindows[handle];
        if (window)
        {
            SDL_GL_SwapWindow(window);
        }
    }
}

bool Platform::handleEvents()
{
    // Update the mouse mode if needed
    MouseMode currentMouseMode = _mouse.mode();
    if (currentMouseMode != _mouseMode)
    {
        _mouseMode = currentMouseMode;

        switch (currentMouseMode)
        {
        case MouseMode_Cursor:
            SDL_SetRelativeMouseMode(SDL_FALSE);
            break;
        case MouseMode_Relative:
            SDL_SetRelativeMouseMode(SDL_TRUE);
            break;
        }
    }

    bool active = true;

    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            active = false;
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
        {
            KeyboardEvent event;
            event.type = e.type == SDL_KEYDOWN ? KeyboardEventType_KeyDown : KeyboardEventType_KeyUp;
            event.key = convertKey(e.key.keysym.sym);
            _keyboard.enqueueEvent(event);
        }
            break;
        case SDL_MOUSEMOTION:
        {
            // Get relative cursor movement
            int movementX = 0;
            int movementY = 0;
            SDL_GetRelativeMouseState(&movementX, &movementY);

            // Get absolute cursor position
            int positionX = 0;
            int positionY = 0;
            SDL_GetMouseState(&positionX, &positionY);

            // Enqueue the event
            MouseEvent event;
            event.type = MouseEventType_Movement;
            event.cursorMovement = IntVector2(movementX, -movementY);
            event.cursorPosition = IntVector2(positionX, positionY);
            _mouse.enqueueEvent(event);
        }
            break;
        case SDL_JOYAXISMOTION:
        {
            // Enqueue the event
            JoystickEvent event;
            event.type = JoystickEventType_AxisMotion;
            event.joystickIndex = e.jaxis.which;
            event.axisIndex = e.jaxis.axis;
            event.axisValue = std::max<Real>((Real)e.jaxis.value / (Real)32767, -1.0);
            _joysticks[event.joystickIndex].enqueueEvent(event);
        }
            break;
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
        {
            // Enqueue the event
            JoystickEvent event;
            event.type = e.type == SDL_JOYBUTTONDOWN ? JoystickEventType_ButtonDown : JoystickEventType_ButtonUp;
            event.joystickIndex = e.jbutton.which;
            event.buttonIndex = e.jbutton.button;
            _joysticks[event.joystickIndex].enqueueEvent(event);
        }
            break;
        }
    }

    _mouse.dispatchEvents();
    _keyboard.dispatchEvents();

    for (Joystick& joystick : _joysticks)
    {
        joystick.dispatchEvents();
    }

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

Sequence<Joystick> Platform::joysticks()
{
    return Sequence<Joystick>(_joysticks.begin(), _joysticks.end());
}