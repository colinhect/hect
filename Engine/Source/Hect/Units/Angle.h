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

#include "Hect/Core/Export.h"
#include "Hect/Units/Unit.h"

namespace hect
{

class Degrees;
class Radians;

///
/// An angle in degrees.
class HECT_EXPORT Degrees :
    public Unit<Degrees, double>
{
public:

    ///
    /// Constructs a zero-degree angle in degrees.
    Degrees();

    ///
    /// Constructs an angle in degrees.
    ///
    /// \param value The value of the angle in degrees.
    explicit Degrees(double value);

    ///
    /// Converts radians to degrees.
    ///
    /// \param angle The angle to convert.
    Degrees(Radians angle);
};

///
/// An angle in radians.
class HECT_EXPORT Radians :
    public Unit<Radians, double>
{
public:

    ///
    /// Constructs a zero-degree angle in radians.
    Radians();

    ///
    /// Constructs an angle in radians.
    ///
    /// \param value The value of the angle in radians.
    explicit Radians(double value);

    ///
    /// Converts degrees to radians.
    ///
    /// \param angle The angle to convert.
    Radians(Degrees angle);
};

}
