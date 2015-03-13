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
#include "Color.h"

#include <cassert>

using namespace hect;

Color::Color()
{
}

Color::Color(double r, double g, double b, double a) :
    r(r),
    g(g),
    b(b),
    a(a)
{
}

Color::Color(const Vector3& v) :
    r(v.x),
    g(v.y),
    b(v.z)
{
}

Color::Color(const Vector4& v) :
    r(v.x),
    g(v.y),
    b(v.z),
    a(v.w)
{
}

Color Color::operator+(const Color& color) const
{
    return Color(r + color.r, g + color.g, b + color.b, a + color.a);
}

Color Color::operator-(const Color& color) const
{
    return Color(r - color.r, g - color.g, b - color.b, a - color.a);
}

Color Color::operator*(double value) const
{
    return Color(r * value, g * value, b * value, a * value);
}

Color Color::operator*(const Color& color) const
{
    return Color(r * color.r, g * color.g, b * color.b, a * color.a);
}

Color Color::operator/(double value) const
{
    return Color(r / value, g / value, b / value, a / value);
}

Color Color::operator/(const Color& color) const
{
    return Color(r / color.r, g / color.g, b / color.b, a / color.a);
}

Color Color::operator-() const
{
    return Color(-r, -g, -b, -a);
}

Color& Color::operator+=(const Color& color)
{
    r += color.r;
    g += color.g;
    b += color.b;
    a += color.a;
    return *this;
}

Color& Color::operator-=(const Color& color)
{
    r -= color.r;
    g -= color.g;
    b -= color.b;
    a -= color.a;
    return *this;
}

Color& Color::operator*=(double value)
{
    r *= value;
    g *= value;
    b *= value;
    a *= value;
    return *this;
}

Color& Color::operator*=(const Color& color)
{
    r *= color.r;
    g *= color.g;
    b *= color.b;
    a *= color.a;
    return *this;
}

Color& Color::operator/=(double value)
{
    r /= value;
    g /= value;
    b /= value;
    a /= value;
    return *this;
}

Color& Color::operator/=(const Color& color)
{
    r /= color.r;
    g /= color.g;
    b /= color.b;
    a /= color.a;
    return *this;
}

double& Color::operator[](size_t i)
{
    assert(i < 4);
    return reinterpret_cast<double*>(this)[i];
}

const double& Color::operator[](size_t i) const
{
    assert(i < 4);
    return reinterpret_cast<const double*>(this)[i];
}

bool Color::operator<(const Color& color) const
{
    return r < color.r && g < color.g && b < color.b && a < color.a;
}

bool Color::operator==(const Color& color) const
{
    return r == color.r && g == color.g && b == color.b && a == color.a;
}

bool Color::operator!=(const Color& color) const
{
    return !(*this == color);
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const Color& color)
{
    encoder << beginArray() << color.r << color.g << color.b << color.a << endArray();
    return encoder;
}

Decoder& operator>>(Decoder& decoder, Color& color)
{
    decoder >> beginArray();
    for (size_t i = 0; decoder.hasMoreElements() && i <= 4; ++i)
    {
        decoder >> decodeValue(color[i]);
    }
    decoder >> endArray();
    return decoder;
}

}