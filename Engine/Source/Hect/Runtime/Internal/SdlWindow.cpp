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
#include "SdlWindow.h"

#ifdef HECT_PLATFORM_SDL

#include <SDL.h>

using namespace hect;

SdlWindow::SdlWindow(const std::string& title, const VideoMode& videoMode) :
    Window(title, videoMode)
{
    // Create the window flags
    uint32_t flags = SDL_WINDOW_OPENGL;
    if (videoMode.isFullscreen())
    {
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    // Create the window
    _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, videoMode.width(), videoMode.height(), flags);
    if (!_window)
    {
        throw FatalError(format("Failed to create SDL window: %s", SDL_GetError()));
    }

    // Create the OpenGL context
    _context = SDL_GL_CreateContext(_window);
}

SdlWindow::~SdlWindow()
{
    if (_context)
    {
        SDL_GL_DeleteContext(_context);
    }

    if (_window)
    {
        SDL_DestroyWindow(_window);
    }
}

void SdlWindow::swapBuffers()
{
    SDL_GL_SwapWindow(_window);
}

#endif