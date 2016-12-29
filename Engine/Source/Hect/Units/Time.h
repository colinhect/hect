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

#include <cstdint>

#include "Hect/Core/Export.h"
#include "Hect/Units/Unit.h"

namespace hect
{

class Microseconds;
class Milliseconds;
class Seconds;
class Minutes;
class Hours;

///
/// A length of time in microseconds.
class HECT_EXPORT Microseconds :
    public Unit<Microseconds, int64_t>
{
public:

    ///
    /// Constructs a zero-length of time in microseconds.
    Microseconds();

    ///
    /// Constructs a length of time in microseconds.
    ///
    /// \param value The value of the length of time in microseconds.
    explicit Microseconds(int64_t value);

    ///
    /// Converts milliseconds to microseconds.
    ///
    /// \param time The length of time to convert.
    Microseconds(Milliseconds time);

    ///
    /// Converts seconds to microseconds.
    ///
    /// \param time The length of time to convert.
    Microseconds(Seconds time);

    ///
    /// Converts minutes to microseconds.
    ///
    /// \param time The length of time to convert.
    Microseconds(Minutes time);

    ///
    /// Converts hours to microseconds.
    ///
    /// \param time The length of time to convert.
    Microseconds(Hours time);
};

///
/// A length of time in milliseconds.
class HECT_EXPORT Milliseconds :
    public Unit<Milliseconds, int64_t>
{
public:

    ///
    /// Constructs a zero-length of time in milliseconds.
    Milliseconds();

    ///
    /// Constructs a length of time in milliseconds.
    ///
    /// \param value The value of the length of time in milliseconds.
    explicit Milliseconds(int64_t value);

    ///
    /// Converts microseconds to milliseconds.
    ///
    /// \param time The length of time to convert.
    Milliseconds(Microseconds time);

    ///
    /// Converts seconds to milliseconds.
    ///
    /// \param time The length of time to convert.
    Milliseconds(Seconds time);

    ///
    /// Converts minutes to milliseconds.
    ///
    /// \param time The length of time to convert.
    Milliseconds(Minutes time);

    ///
    /// Converts hours to milliseconds.
    ///
    /// \param time The length of time to convert.
    Milliseconds(Hours time);
};

///
/// A length of time in seconds.
class HECT_EXPORT Seconds :
    public Unit<Seconds, double>
{
public:

    ///
    /// Constructs a zero-length of time in seconds.
    Seconds();

    ///
    /// Constructs a length of time in seconds.
    ///
    /// \param value The value of the length of time in seconds.
    explicit Seconds(double value);

    ///
    /// Converts microseconds to seconds.
    ///
    /// \param time The length of time to convert.
    Seconds(Microseconds time);

    ///
    /// Converts milliseconds to seconds.
    ///
    /// \param time The length of time to convert.
    Seconds(Milliseconds time);

    ///
    /// Converts minutes to seconds.
    ///
    /// \param time The length of time to convert.
    Seconds(Minutes time);

    ///
    /// Converts hours to seconds.
    ///
    /// \param time The length of time to convert.
    Seconds(Hours time);
};

///
/// A length of time in minutes.
class HECT_EXPORT Minutes :
    public Unit<Minutes, double>
{
public:

    ///
    /// Constructs a zero-length of time in minutes.
    Minutes();

    ///
    /// Constructs a length of time in minutes.
    ///
    /// \param value The value of the length of time in minutes.
    explicit Minutes(double value);

    ///
    /// Converts microseconds to minutes.
    ///
    /// \param time The length of time to convert.
    Minutes(Microseconds time);

    ///
    /// Converts milliseconds to minutes.
    ///
    /// \param time The length of time to convert.
    Minutes(Milliseconds time);

    ///
    /// Converts seconds to minutes.
    ///
    /// \param time The length of time to convert.
    Minutes(Seconds time);

    ///
    /// Converts hours to minutes.
    ///
    /// \param time The length of time to convert.
    Minutes(Hours time);
};

///
/// A length of time in hours.
class HECT_EXPORT Hours :
    public Unit<Hours, double>
{
public:

    ///
    /// Constructs a zero-length of time in hours.
    Hours();

    ///
    /// Constructs a length of time in hours.
    ///
    /// \param value The value of the length of time in hours.
    explicit Hours(double value);

    ///
    /// Converts microseconds to hours.
    ///
    /// \param time The length of time to convert.
    Hours(Microseconds time);

    ///
    /// Converts milliseconds to hours.
    ///
    /// \param time The length of time to convert.
    Hours(Milliseconds time);

    ///
    /// Converts seconds to hours.
    ///
    /// \param time The length of time to convert.
    Hours(Seconds time);

    ///
    /// Converts minutes to hours.
    ///
    /// \param time The length of time to convert.
    Hours(Minutes time);
};

}
