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

#include <string>

#include "Hect/Core/Configuration.h"

namespace hect
{

///
/// The category of a log message.
enum LogLevel
{
    ///
    /// An informational message for the user.
    LogLevel_Info,

    ///
    /// An informational message for the developer.
    LogLevel_Debug,

    ///
    /// A warning message to the user.
    LogLevel_Warning,

    ///
    /// An error message to the user.
    LogLevel_Error,

    ///
    /// A verbose debug message for the developer.
    LogLevel_Trace,
};

///
/// Logs a message.
///
/// \param level The level of the message.
/// \param message The message.
void log(LogLevel level, const std::string& message);

// Info
#if defined(HECT_ENABLE_LOG_INFO)
#define HECT_INFO(message)      log(hect::LogLevel_Info, message)
#else
#define HECT_INFO(message)
#endif

// Debug
#if defined(HECT_ENABLE_LOG_DEBUG)
#define HECT_DEBUG(message)     log(hect::LogLevel_Debug, message)
#else
#define HECT_DEBUG(message)
#endif

// Warning
#if defined(HECT_ENABLE_LOG_WARNING)
#define HECT_WARNING(message)   log(hect::LogLevel_Warning, message)
#else
#define HECT_WARNING(message)
#endif

// Error
#if defined(HECT_ENABLE_LOG_ERROR)
#define HECT_ERROR(message)     log(hect::LogLevel_Error, message)
#else
#define HECT_ERROR(message)
#endif

// Trace
#if defined(HECT_ENABLE_LOG_TRACE)
#define HECT_TRACE(message)     log(hect::LogLevel_Trace, message)
#else
#define HECT_TRACE(message)
#endif

}