///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2016 Colin Hill
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
#include "Timer.h"

#include "Hect/Core/Configuration.h"

#ifdef HECT_PLATFORM_SDL
#include <SDL.h>
#else
#include <chrono>
#endif

using namespace hect;

#ifdef HECT_PLATFORM_SDL
static const uint64_t _frequency = SDL_GetPerformanceFrequency();
#else
static std::chrono::high_resolution_clock::time_point _start = std::chrono::high_resolution_clock::now();
#endif

Microseconds Timer::total_elapsed()
{
#ifdef HECT_PLATFORM_SDL
    uint64_t ticks = SDL_GetPerformanceCounter();
    return Microseconds(1000000 * ticks / _frequency);
#else
    static auto _start = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(now - _start).count();
    return Microseconds(microseconds);
#endif
}

Timer::Timer()
{
    reset();
}

void Timer::reset()
{
    _start = total_elapsed();
}

Microseconds Timer::elapsed() const
{
    return total_elapsed() - _start;
}
