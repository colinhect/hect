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

static HANDLE stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>

namespace hect
{

static std::recursive_mutex _logMutex;
static Dispatcher<LogMessageEvent> _dispatcher;
static std::vector<bool> _logLevels;
static std::vector<std::string> _logLevelStrings;

void log(LogLevel level, const std::string& message)
{
    static Timer timer;
    std::lock_guard<std::recursive_mutex> lock(_logMutex);

    // Ignore the message if the level is not enabled
    if (!isLogLevelEnabled(level))
    {
        return;
    }

    // Dispatch the log message event
    LogMessageEvent event;
    event.level = level;
    event.message = message;
    _dispatcher.dispatchEvent(event);

    // Calculate timestamp parts
    int64_t total = timer.elapsed().milliseconds();
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
    std::string formattedMessage = ss.str();

#ifdef HECT_WINDOWS_BUILD

#ifdef HECT_DEBUG_BUILD
    OutputDebugString(formattedMessage.data());
#endif

    // Set the color text color
    switch (level)
    {
    case LogLevel::Info:
        SetConsoleTextAttribute(stdOutHandle, 15);
        break;
    case LogLevel::Debug:
        SetConsoleTextAttribute(stdOutHandle, 7);
        break;
    case LogLevel::Warning:
        SetConsoleTextAttribute(stdOutHandle, 14);
        break;
    case LogLevel::Error:
        SetConsoleTextAttribute(stdOutHandle, 12);
        break;
    case LogLevel::Trace:
        SetConsoleTextAttribute(stdOutHandle, 8);
        break;
    }

    std::cout << formattedMessage;
    std::cout.flush();

    // Reset the console text color
    SetConsoleTextAttribute(stdOutHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    // Show a message box when logging an error
    if (level == LogLevel::Error)
    {
        MessageBoxA(NULL, message.data(), "Error", MB_ICONHAND | MB_OK);
    }
#else
    (void)level;
    std::cout << formattedMessage;
    std::cout.flush();
#endif
}

void registerLogListener(Listener<LogMessageEvent>& listener)
{
    std::lock_guard<std::recursive_mutex> lock(_logMutex);

    _dispatcher.registerListener(listener);
}

void setLogLevelEnabled(LogLevel level, bool enabled)
{
    std::lock_guard<std::recursive_mutex> lock(_logMutex);
    if (_logLevels.empty())
    {
        _logLevels = std::vector<bool>(5, false);
    }

    _logLevels[static_cast<int>(level)] = enabled;
}

bool isLogLevelEnabled(LogLevel level)
{
    std::lock_guard<std::recursive_mutex> lock(_logMutex);

    bool enabled = false;

    if (!_logLevels.empty())
    {
        enabled = _logLevels[static_cast<int>(level)];
    }

    return enabled;
}

}
