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
#include "Logging.h"

#include "Hect/Timing/Timer.h"

#ifdef HECT_WINDOWS_BUILD
#include <Windows.h>

static HANDLE std_out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>

namespace hect
{

static std::recursive_mutex _log_mutex;
static EventDispatcher<LogMessageEvent> _dispatcher;
static std::vector<bool> _log_levels;
static std::vector<std::string> _log_level_strings;

void log(LogLevel level, const std::string& message)
{
    static Timer timer;
    std::lock_guard<std::recursive_mutex> lock(_log_mutex);

    // Ignore the message if the level is not enabled
    if (!is_log_level_enabled(level))
    {
        return;
    }

    // Dispatch the log message event
    LogMessageEvent event;
    event.level = level;
    event.message = message;
    _dispatcher.dispatch_event(event);

    // Calculate timestamp parts
    int64_t total = Milliseconds(timer.elapsed()).value;
    int64_t milliseconds = total % 1000;
    int64_t seconds = total / 1000 % 60;
    int64_t minutes = total / 1000 / 60 % 60;
    int64_t hours = total / 1000 / 60 / 60;

    // Format message with timestamp prepended
    std::stringstream ss;
    ss << std::setfill('0') << "[" << hours << ":"
       << std::setw(2) << minutes << ":"
       << std::setw(2) << seconds << ":"
       << std::setw(3) << milliseconds << "] "
       << message << std::endl;
    std::string formatted_message = ss.str();

#ifdef HECT_WINDOWS_BUILD

#ifdef HECT_DEBUG_BUILD
    OutputDebugString(formatted_message.data());
#endif

    // Set the color text color
    switch (level)
    {
    case LogLevel::Info:
        SetConsoleTextAttribute(std_out_handle, 15);
        break;
    case LogLevel::Debug:
        SetConsoleTextAttribute(std_out_handle, 7);
        break;
    case LogLevel::Warning:
        SetConsoleTextAttribute(std_out_handle, 14);
        break;
    case LogLevel::Error:
        SetConsoleTextAttribute(std_out_handle, 12);
        break;
    case LogLevel::Trace:
        SetConsoleTextAttribute(std_out_handle, 8);
        break;
    }

    std::cout << formatted_message;
    std::cout.flush();

    // Reset the console text color
    SetConsoleTextAttribute(std_out_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
    (void)level;
    std::cout << formatted_message;
    std::cout.flush();
#endif
}

void register_log_listener(EventListener<LogMessageEvent>& listener)
{
    std::lock_guard<std::recursive_mutex> lock(_log_mutex);

    _dispatcher.register_listener(listener);
}

void set_log_level_enabled(LogLevel level, bool enabled)
{
    std::lock_guard<std::recursive_mutex> lock(_log_mutex);
    if (_log_levels.empty())
    {
        _log_levels = std::vector<bool>(5, false);
    }

    _log_levels[static_cast<int>(level)] = enabled;
}

bool is_log_level_enabled(LogLevel level)
{
    std::lock_guard<std::recursive_mutex> lock(_log_mutex);

    bool enabled = false;

    if (!_log_levels.empty())
    {
        enabled = _log_levels[static_cast<int>(level)];
    }

    return enabled;
}

}
