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
#include "TimeSpan.h"

using namespace hect;

TimeSpan TimeSpan::fromMicroseconds(int64_t microseconds)
{
    return TimeSpan(microseconds);
}

TimeSpan TimeSpan::fromMilliseconds(int64_t milliseconds)
{
    return TimeSpan(milliseconds * 1000);
}

TimeSpan TimeSpan::fromSeconds(Real seconds)
{
    return TimeSpan((int64_t)(seconds * 1000 * 1000));
}

TimeSpan::TimeSpan()
{
}

int64_t TimeSpan::microseconds()
{
    return _microseconds;
}

int64_t TimeSpan::milliseconds()
{
    return _microseconds / 1000;
}

Real TimeSpan::seconds()
{
    return static_cast<Real>(milliseconds()) / Real(1000);
}

TimeSpan TimeSpan::operator+(const TimeSpan& t) const
{
    return TimeSpan(_microseconds + t._microseconds);
}

TimeSpan TimeSpan::operator-(const TimeSpan& t) const
{
    return TimeSpan(_microseconds - t._microseconds);
}

TimeSpan TimeSpan::operator*(Real value) const
{
    return TimeSpan((int64_t)(value * _microseconds));
}

TimeSpan TimeSpan::operator/(Real value) const
{
    return TimeSpan(static_cast<int64_t>((static_cast<Real>(_microseconds) / value)));
}

TimeSpan TimeSpan::operator-() const
{
    return TimeSpan(-_microseconds);
}

TimeSpan& TimeSpan::operator+=(const TimeSpan& t)
{
    _microseconds += t._microseconds;
    return *this;
}

TimeSpan& TimeSpan::operator-=(const TimeSpan& t)
{
    _microseconds -= t._microseconds;
    return *this;
}

TimeSpan& TimeSpan::operator*=(Real value)
{
    _microseconds = static_cast<int64_t>(value * _microseconds);
    return *this;
}

TimeSpan& TimeSpan::operator/=(Real value)
{
    _microseconds = static_cast<int64_t>(static_cast<Real>(_microseconds) / value);
    return *this;
}

bool TimeSpan::operator<(const TimeSpan& t) const
{
    return _microseconds < t._microseconds;
}

bool TimeSpan::operator<=(const TimeSpan& t) const
{
    return _microseconds <= t._microseconds;
}

bool TimeSpan::operator>(const TimeSpan& t) const
{
    return _microseconds > t._microseconds;
}

bool TimeSpan::operator>=(const TimeSpan& t) const
{
    return _microseconds >= t._microseconds;
}

TimeSpan::TimeSpan(int64_t microseconds) :
    _microseconds(microseconds)
{
}