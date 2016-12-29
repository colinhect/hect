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
#include "Time.h"

namespace hect
{

Milliseconds::Milliseconds()
{
}

Milliseconds::Milliseconds(int64_t value) :
    Unit(value)
{
}

Milliseconds::Milliseconds(Seconds time) :
    Milliseconds(static_cast<int64_t>(time.value * 1000.0))
{
}

Milliseconds::Milliseconds(Minutes time) :
    Milliseconds(Seconds(time))
{
}

Milliseconds::Milliseconds(Hours time) :
    Milliseconds(Minutes(time))
{
}

Seconds::Seconds()
{
}

Seconds::Seconds(double value) :
    Unit(value)
{
}

Seconds::Seconds(Milliseconds time) :
    Seconds(static_cast<double>(time.value) / 1000.0)
{
}

Seconds::Seconds(Minutes time) :
    Seconds(time.value * 60.0)
{
}

Seconds::Seconds(Hours time) :
    Seconds(Minutes(time))
{
}

Minutes::Minutes()
{
}

Minutes::Minutes(double value) :
    Unit(value)
{
}

Minutes::Minutes(Milliseconds time) :
    Minutes(Seconds(time))
{
}

Minutes::Minutes(Seconds time) :
    Minutes(time.value / 60.0)
{
}

Minutes::Minutes(Hours time) :
    Minutes(time.value * 60.0)
{
}

Hours::Hours()
{
}

Hours::Hours(double value) :
    Unit(value)
{
}

Hours::Hours(Milliseconds time) :
    Hours(Seconds(time))
{
}

Hours::Hours(Seconds time) :
    Hours(Minutes(time))
{
}

Hours::Hours(Minutes time) :
    Hours(time.value / 60.0)
{
}

}
