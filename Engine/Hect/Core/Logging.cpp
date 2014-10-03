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
#include "Logging.h"

#ifdef HECT_WINDOWS_BUILD
#include <Windows.h>

static HANDLE stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

#include <iostream>
#include <mutex>

namespace hect
{

static std::mutex _logMutex;

void log(LogLevel level, const std::string& message)
{
    std::lock_guard<std::mutex> lock{ _logMutex };

#ifdef HECT_WINDOWS_BUILD

#ifdef HECT_DEBUG_BUILD
    OutputDebugString((message + "\n").c_str());
#endif

    // Set the color text color
    switch (level)
    {
    case LogLevel_Info:
        SetConsoleTextAttribute(stdOutHandle, 15);
        std::cout << "I> ";
        break;
    case LogLevel_Debug:
        SetConsoleTextAttribute(stdOutHandle, 7);
        std::cout << "D> ";
        break;
    case LogLevel_Warning:
        SetConsoleTextAttribute(stdOutHandle, 14);
        std::cout << "W> ";
        break;
    case LogLevel_Error:
        SetConsoleTextAttribute(stdOutHandle, 12);
        std::cout << "E> ";
        break;
    case LogLevel_Trace:
        SetConsoleTextAttribute(stdOutHandle, 8);
        std::cout << "T> ";
        break;
    }

    std::cout << message << std::endl;

    // Reset the console text color
    SetConsoleTextAttribute(stdOutHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif
}

}