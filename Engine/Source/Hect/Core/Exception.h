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

#include <stdexcept>

#include "Hect/Core/Configuration.h"
#include "Hect/Core/Export.h"

namespace hect
{

#ifdef HECT_WINDOWS_BUILD
class HECT_EXPORT std::runtime_error;
#endif

///
/// A base exception.
class HECT_EXPORT Exception :
    public std::runtime_error
{
public:

    ///
    /// Constructs the exception without a message.
    Exception();

    ///
    /// Constructs the exception with a message.
    ///
    /// \param message The message.
    Exception(const std::string& message);
};

///
/// An invalid operation was attempted.
class HECT_EXPORT InvalidOperation :
    public Exception
{
public:

    ///
    /// Constructs the exception without a message.
    InvalidOperation();

    ///
    /// Constructs the exception with a message.
    ///
    /// \param message The message.
    InvalidOperation(const std::string& message);
};

///
/// A fatal error was encountered.
class HECT_EXPORT FatalError :
    public Exception
{
public:

    ///
    /// Constructs the exception without a message.
    FatalError();

    ///
    /// Constructs the exception with a message.
    ///
    /// \param message The message.
    FatalError(const std::string& message);
};

// In the process of removing this
class HECT_EXPORT Error :
    public Exception
{
public:
    Error();
    Error(const std::string& message);
};

}