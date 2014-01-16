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
namespace hect
{

template <typename T>
Angle<T> Angle<T>::fromDegrees(T degrees)
{
    return Angle(((T)pi / (T)180) * degrees);
}

template <typename T>
Angle<T> Angle<T>::fromRadians(T radians)
{
    return Angle(radians);
}

template <typename T>
Angle<T>::Angle():
    _radians(0)
{
}

template <typename T>
T Angle<T>::degrees() const
{
    T degrees = ((T)180 / (T)pi) * _radians;
    if (degrees == (T)360)
    {
        degrees = (T)0;
    }
    return degrees;
}

template <typename T>
T Angle<T>::radians() const
{
    return _radians;
}

template <typename T>
Angle<T> Angle<T>::operator+(const Angle& a) const
{
    return Angle(_radians + a._radians);
}

template <typename T>
Angle<T> Angle<T>::operator-(const Angle& a) const
{
    return Angle(_radians - a._radians);
}

template <typename T>
Angle<T> Angle<T>::operator*(T value) const
{
    return Angle(_radians * value);
}

template <typename T>
Angle<T> Angle<T>::operator/(T value) const
{
    return Angle(_radians / value);
}

template <typename T>
Angle<T> Angle<T>::operator-() const
{
    return Angle(-_radians);
}

template <typename T>
Angle<T>& Angle<T>::operator+=(const Angle& a)
{
    _radians += a._radians;
    return *this;
}

template <typename T>
Angle<T>& Angle<T>::operator-=(const Angle& a)
{
    _radians -= a._radians;
    return *this;
}

template <typename T>
Angle<T>& Angle<T>::operator*=(T value)
{
    _radians *= value;
    return *this;
}

template <typename T>
Angle<T>& Angle<T>::operator/=(T value)
{
    _radians /= value;
    return *this;
}

template <typename T>
Angle<T>::Angle(T radians):
    _radians(radians)
{
}

}