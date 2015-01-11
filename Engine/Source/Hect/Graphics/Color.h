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
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"

namespace hect
{

///
/// An RGBA color.
class HECT_EXPORT Color
{
public:

    ///
    /// Constructs a white color.
    Color();

    ///
    /// Constructs a color from RGBA components.
    ///
    /// \param r The red component.
    /// \param g The green component.
    /// \param b The blue component.
    /// \param a The alpha component.
    Color(Real r, Real g, Real b, Real a = Real(1));

    ///
    /// Returns the sum of the color and another color.
    ///
    /// \param color The color to compute the sum with.
    Color operator+(const Color& color) const;

    ///
    /// Returns the difference of the color and another color.
    ///
    /// \param color The color to compute the difference with.
    Color operator-(const Color& color) const;

    ///
    /// Returns the product of the color and a scalar.
    ///
    /// \param value The scalar to compute the product with.
    Color operator*(Real value) const;

    ///
    /// Returns the product of the color and another color.
    ///
    /// \param color The color to compute the product with.
    Color operator*(const Color& color) const;

    ///
    /// Returns the quotient of the color and a scalar.
    ///
    /// \param value The scalar to compute the quotient with.
    Color operator/(Real value) const;

    ///
    /// Returns the quotient of the color and another color.
    ///
    /// \param color The color to compute the quotient with.
    Color operator/(const Color& color) const;

    ///
    /// Returns a negated copy of the color.
    Color operator-() const;

    ///
    /// Adds a color to the color.
    ///
    /// \param color The color to add.
    ///
    /// \returns A reference to the color.
    Color& operator+=(const Color& color);

    ///
    /// Subtracts a color from the color.
    ///
    /// \param color The color to subtract.
    ///
    /// \returns A reference to the color.
    Color& operator-=(const Color& color);

    ///
    /// Multiplies the color by a scalar.
    ///
    /// \param value The scalar to multiply by.
    ///
    /// \returns A reference to the color.
    Color& operator*=(Real value);

    ///
    /// Multiplies the color by another color.
    ///
    /// \param color The color to multiply by.
    ///
    /// \returns A reference to the color.
    Color& operator*=(const Color& color);

    ///
    /// Divides the color by a scalar.
    ///
    /// \param value The scalar to divide by.
    ///
    /// \returns A reference to the color.
    Color& operator/=(Real value);

    ///
    /// Divides the color by another color.
    ///
    /// \param color The color to divide by.
    ///
    /// \returns A reference to the color.
    Color& operator/=(const Color& color);

    ///
    /// Returns the ith component of the vertex.
    ///
    /// \param i The index of which component to access.
    Real& operator[](size_t i);

    ///
    /// Returns a constant reference to the ith component of the vertex.
    ///
    /// \param i The index of which component to access.
    const Real& operator[](size_t i) const;

    ///
    /// Returns whether the color is less than another.
    ///
    /// \param color The other color.
    bool operator<(const Color& color) const;

    ///
    /// Returns whether the color is equivalent to another.
    ///
    /// \param color The other color.
    bool operator==(const Color& color) const;

    ///
    /// Returns whether the color is different from another.
    ///
    /// \param color The other color.
    bool operator!=(const Color& color) const;

    ///
    /// The red component of the color.
    Real r { 1 };

    ///
    /// The green component of the color.
    Real g { 1 };

    ///
    /// The blue component of the color.
    Real b { 1 };

    ///
    /// The alpha component of the color.
    Real a { 1 };
};

HECT_EXPORT Encoder& operator<<(Encoder& encoder, const Color& color);
HECT_EXPORT Decoder& operator>>(Decoder& decoder, Color& color);

}