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
#pragma once

#include <string>

#include "Hect/Core/Configuration.h"
#include "Hect/Core/Event.h"
#include "Hect/Core/Format.h"
#include "Hect/Core/LogLevel.h"
#include "Hect/Core/LogMessageEvent.h"

namespace hect
{

///
/// Logs a message.
///
/// \param level The level of the message.
/// \param message The message.
HECT_EXPORT void log(LogLevel level, const std::string& message);

///
/// Registers a listener of log message events.
///
/// \param listener The listener.
HECT_EXPORT void registerLogListener(Listener<LogMessageEvent>& listener);

///
/// Sets whether messages at the specified level are logged.
///
/// \param level The level.
/// \param enabled Whether messages at the specified level are logged.
HECT_EXPORT void setLogLevelEnabled(LogLevel level, bool enabled);

///
/// Returns whether messages at the specified level are logged.
///
/// \param level The level in question.
HECT_EXPORT bool isLogLevelEnabled(LogLevel level);

// Info
#if defined(HECT_ENABLE_LOG_INFO)
#define HECT_INFO(fmt, ...) \
    if (hect::isLogLevelEnabled(hect::LogLevel::Info)) \
    { \
        log(hect::LogLevel::Info, hect::format(fmt, __VA_ARGS__)); \
    }
#else
#define HECT_INFO(fmt, ...)
#endif

// Debug
#if defined(HECT_ENABLE_LOG_DEBUG)
#define HECT_DEBUG(fmt, ...) \
    if (hect::isLogLevelEnabled(hect::LogLevel::Debug)) \
    { \
        log(hect::LogLevel::Debug, hect::format(fmt, __VA_ARGS__)); \
    }
#else
#define HECT_DEBUG(fmt, ...)
#endif

// Warning
#if defined(HECT_ENABLE_LOG_WARNING)
#define HECT_WARNING(fmt, ...) \
    if (hect::isLogLevelEnabled(hect::LogLevel::Warning)) \
    { \
        log(hect::LogLevel::Warning, hect::format(fmt, __VA_ARGS__)); \
    }
#else
#define HECT_WARNING(fmt, ...)
#endif

// Error
#if defined(HECT_ENABLE_LOG_ERROR)
#define HECT_ERROR(fmt, ...) \
    if (hect::isLogLevelEnabled(hect::LogLevel::Error)) \
    { \
        log(hect::LogLevel::Error, hect::format(fmt, __VA_ARGS__)); \
    }
#else
#define HECT_ERROR(fmt, ...)
#endif

// Trace
#if defined(HECT_ENABLE_LOG_TRACE)
#define HECT_TRACE(fmt, ...) \
    if (hect::isLogLevelEnabled(hect::LogLevel::Trace)) \
    { \
        log(hect::LogLevel::Trace, hect::format(fmt, __VA_ARGS__)); \
    }
#else
#define HECT_TRACE(fmt, ...)
#endif

}