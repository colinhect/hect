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
#include "Window.h"

#include "Hect/Core/Configuration.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/Input/InputSystem.h"
#include "Hect/Input/Keyboard.h"
#include "Hect/Input/Mouse.h"

using namespace hect;

#include <SDL.h>

static SDL_Window* _sdlWindow = nullptr;
static SDL_GLContext  _glContext = nullptr;

Key::Enum _convertKey(SDL_Keycode key)
{
    switch (key)
    {
    case SDLK_a:
        return Key::A;
    case SDLK_b:
        return Key::B;
    case SDLK_c:
        return Key::C;
    case SDLK_d:
        return Key::D;
    case SDLK_e:
        return Key::E;
    case SDLK_f:
        return Key::F;
    case SDLK_g:
        return Key::G;
    case SDLK_h:
        return Key::H;
    case SDLK_i:
        return Key::I;
    case SDLK_j:
        return Key::J;
    case SDLK_k:
        return Key::K;
    case SDLK_l:
        return Key::L;
    case SDLK_m:
        return Key::M;
    case SDLK_n:
        return Key::N;
    case SDLK_o:
        return Key::O;
    case SDLK_p:
        return Key::P;
    case SDLK_q:
        return Key::Q;
    case SDLK_r:
        return Key::R;
    case SDLK_s:
        return Key::S;
    case SDLK_t:
        return Key::T;
    case SDLK_u:
        return Key::U;
    case SDLK_v:
        return Key::V;
    case SDLK_w:
        return Key::W;
    case SDLK_x:
        return Key::X;
    case SDLK_y:
        return Key::Y;
    case SDLK_z:
        return Key::Z;
    case SDLK_ESCAPE:
        return Key::Esc;
    case SDLK_TAB:
        return Key::Tab;

    case SDLK_F1:
        return Key::F1;
    case SDLK_F2:
        return Key::F2;
    case SDLK_F3:
        return Key::F3;
    case SDLK_F4:
        return Key::F4;
    case SDLK_F5:
        return Key::F5;
    case SDLK_F6:
        return Key::F6;
    case SDLK_F7:
        return Key::F7;
    case SDLK_F8:
        return Key::F8;
    case SDLK_F9:
        return Key::F9;
    case SDLK_F10:
        return Key::F10;
    case SDLK_F11:
        return Key::F11;
    case SDLK_F12:
        return Key::F12;

    case SDLK_LCTRL:
        return Key::LeftCtrl;
    case SDLK_LSHIFT:
        return Key::LeftShift;
    case SDLK_LALT:
        return Key::LeftAlt;

    default:
        return Key::Unknown;
    }
}

#ifdef HECT_WINDOWS
#include <Windows.h>
#endif

void Window::showFatalError(const std::string& message)
{
#ifdef HECT_WINDOWS
    MessageBoxA(NULL, message.c_str(), "Fatal Error", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#endif
}

Window::Window(const std::string& title, const VideoMode& videoMode) :
    RenderTarget(videoMode.width(), videoMode.height()),
    _mouseMode(MouseMode::Cursor)
{
    // Currently only a single window is supported
    if (_sdlWindow)
    {
        throw Error("Mutliple windows are not supported");
    }

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw Error(format("Failed to initialize SDL: %s", SDL_GetError()));
    }

    // Create the window flags
    uint32_t flags = SDL_WINDOW_OPENGL;
    if (videoMode.isFullscreen())
    {
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    // Create the window
    _sdlWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, videoMode.width(), videoMode.height(), SDL_WINDOW_OPENGL);
    if (!_sdlWindow)
    {
        throw Error(format("Failed to create SDL window: %s", SDL_GetError()));
    }

    // Create the OpenGL context
    _glContext = SDL_GL_CreateContext(_sdlWindow);
}

Window::~Window()
{
    // Destroy the OpenGL context
    SDL_GL_DeleteContext(_glContext);

    // Destroy the window
    SDL_DestroyWindow(_sdlWindow);

    // Quit SDL
    SDL_Quit();
}

void Window::bind(Renderer* renderer)
{
    renderer->bindWindow(*this);
}

bool Window::pollEvents(InputSystem& inputSystem)
{
    // Update the mouse mode if needed
    MouseMode::Enum currentMouseMode = inputSystem.mouse().mode();
    if (currentMouseMode != _mouseMode)
    {
        _mouseMode = currentMouseMode;

        switch (currentMouseMode)
        {
        case MouseMode::Cursor:
            SDL_SetRelativeMouseMode(SDL_FALSE);
            break;
        case MouseMode::Relative:
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
            event.type = e.type == SDL_KEYDOWN ? KeyboardEventType::KeyDown : KeyboardEventType::KeyUp;
            event.key = _convertKey(e.key.keysym.sym);
            inputSystem._enqueueEvent(event);
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
            event.cursorMovement = IntVector2(movementX, -movementY);
            event.cursorPosition = IntVector2(positionX, positionY);
            inputSystem._enqueueEvent(event);
        }
        break;
        }
    }

    inputSystem._dispatchEvents();

    return active;
}

void Window::swapBuffers()
{
    SDL_GL_SwapWindow(_sdlWindow);
}