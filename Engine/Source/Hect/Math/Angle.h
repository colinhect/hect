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
#pragma once

#include "Hect/Core/Export.h"
#include "Hect/Core/Real.h"
#include "Hect/IO/Encoder.h"
#include "Hect/IO/Decoder.h"

namespace hect
{

///
/// An angle.
class HECT_EXPORT Angle
{
public:

    ///
    /// Constructs an angle from a value in degrees.
    ///
    /// \param degrees The angle in degrees.
    ///
    /// \returns The angle.
    static Angle fromDegrees(Real degrees);

    ///
    /// Constructs an angle from a value in radians.
    ///
    /// \param radians The angle in radians.
    ///
    /// \returns The angle.
    static Angle fromRadians(Real radians);

    ///
    /// Constructs a zero-degree angle.
    Angle();

    ///
    /// Constructs an angle.
    ///
    /// \param radians The angle in radians.
    Angle(Real radians);

    ///
    /// Returns the value of the angle in degrees.
    Real degrees() const;

    ///
    /// Returns the value of the angle in radians.
    Real radians() const;

    ///
    /// Returns the sum of the angle and another angle.
    ///
    /// \param a The angle to compute the sum with.
    Angle operator+(const Angle& a) const;

    ///
    /// Returns the difference of the angle and another angle.
    ///
    /// \param a The angle to compute the diference with.
    Angle operator-(const Angle& a) const;

    ///
    /// Returns the product of the angle and a scalar.
    ///
    /// \param value The scalar to compute the product with.
    Angle operator*(Real value) const;

    ///
    /// Returns the quotient of the angle and a scalar.
    ///
    /// \param value The scalar to compute the quotient with.
    Angle operator/(Real value) const;

    ///
    /// Returns a negated copy of the angle.
    Angle operator-() const;

    ///
    /// Adds an angle to the angle.
    ///
    /// \param a The angle to add.
    ///
    /// \returns A reference to the angle.
    Angle& operator+=(const Angle& a);

    ///
    /// Subtracts an angle from the angle.
    ///
    /// \param a The angle to subtract.
    ///
    /// \returns A reference to the angle.
    Angle& operator-=(const Angle& a);

    ///
    /// Multiples the angle by a scalar.
    ///
    /// \param value The scalar to multiply by.
    ///
    /// \returns A reference to the angle.
    Angle& operator*=(Real value);

    ///
    /// Divides the angle by a scalar.
    ///
    /// \param value The scalar to divide by.
    ///
    /// \returns A reference to the angle.
    Angle& operator/=(Real value);

private:
    Real _radians { 0 };
};

Encoder& operator<<(Encoder& encoder, Angle angle);
Decoder& operator>>(Decoder& decoder, Angle& angle);

}