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
#include "Angle.h"

#include "Hect/Math/Constants.h"
#include "Hect/Math/Degrees.h"

namespace hect
{

Angle::Angle()
{
}

double Angle::inDegrees() const
{
    double degrees = (180.0 / Pi) * _radians;
    if (degrees == 360.0)
    {
        degrees = 0;
    }
    return degrees;
}

double Angle::inRadians() const
{
    return _radians;
}

Angle Angle::operator+(Angle a) const
{
    return Angle(_radians + a._radians);
}

Angle Angle::operator-(Angle a) const
{
    return Angle(_radians - a._radians);
}

Angle Angle::operator*(double value) const
{
    return Angle(_radians * value);
}

Angle Angle::operator/(double value) const
{
    return Angle(_radians / value);
}

Angle Angle::operator-() const
{
    return Angle(-_radians);
}

Angle& Angle::operator+=(Angle a)
{
    _radians += a._radians;
    return *this;
}

Angle& Angle::operator-=(Angle a)
{
    _radians -= a._radians;
    return *this;
}

Angle& Angle::operator*=(double value)
{
    _radians *= value;
    return *this;
}

Angle& Angle::operator/=(double value)
{
    _radians /= value;
    return *this;
}

Angle::Angle(double radians) :
    _radians(radians)
{
}

Encoder& operator<<(Encoder& encoder, Angle angle)
{
    encoder << angle.inDegrees();
    return encoder;
}

Decoder& operator>>(Decoder& decoder, Angle& angle)
{
    double degrees;
    decoder >> decodeValue(degrees);
    angle = Degrees(degrees);
    return decoder;
}

}
