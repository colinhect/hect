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

#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"

namespace hect
{

///
/// A strongly-typed unit supporting basic arithmetic and comparison.
template <typename UnitType, typename ValueType>
class Unit
{
public:

    ///
    /// Constructs a unit with a value of zero.
    Unit();

    ///
    /// Constructs a unit with the specified value.
    ///
    /// \param value The value.
    Unit(ValueType value);

    ///
    /// Returns the sum of the unit and another unit.
    ///
    /// \param unit The unit to compute the sum with.
    UnitType operator+(UnitType unit) const;

    ///
    /// Returns the difference of the unit and another unit.
    ///
    /// \param unit The unit to compute the difference with.
    UnitType operator-(UnitType unit) const;

    ///
    /// Returns the product of the unit and another unit.
    ///
    /// \param unit The unit to compute the product with.
    UnitType operator*(UnitType unit) const;

    ///
    /// Returns the quotient of the unit and another unit.
    ///
    /// \param unit The unit to compute the quotient with.
    UnitType operator/(UnitType unit) const;

    ///
    /// Returns the product of the unit and a scalar.
    ///
    /// \param value The scalar to compute the product with.
    UnitType operator*(ValueType value) const;

    ///
    /// Returns the quotient of the unit and a scalar.
    ///
    /// \param value The scalar to compute the quotient with.
    UnitType operator/(ValueType value) const;

    ///
    /// Adds a unit to the unit.
    ///
    /// \param unit The unit to add.
    ///
    /// \returns A reference to the unit.
    Unit& operator+=(UnitType unit);

    ///
    /// Subtracts a unit from the unit.
    ///
    /// \param unit The unit to subtract.
    ///
    /// \returns A reference to the unit.
    Unit& operator-=(UnitType unit);

    ///
    /// Multiplies the unit by a unit.
    ///
    /// \param unit The unit to multiply by.
    ///
    /// \returns A reference to the unit.
    Unit& operator*=(UnitType unit);

    ///
    /// Divides the unit by a unit.
    ///
    /// \param unit The unit to divide by.
    ///
    /// \returns A reference to the unit.
    Unit& operator/=(UnitType unit);

    ///
    /// Multiplies the unit by a scalar.
    ///
    /// \param value The scalar to multiply by.
    ///
    /// \returns A reference to the unit.
    Unit& operator*=(ValueType value);

    ///
    /// Devices the unit by a scalar.
    ///
    /// \param value The scalar to divide by.
    ///
    /// \returns A reference to the unit.
    Unit& operator/=(ValueType value);

    ///
    /// Returns a negated copy of the unit.
    UnitType operator-() const;

    ///
    /// Returns whether the unit is less than another.
    ///
    /// \param unit The other unit.
    bool operator<(UnitType unit) const;

    ///
    /// Returns whether the unit is less than or equal to another.
    ///
    /// \param unit The other unit.
    bool operator<=(UnitType unit) const;

    ///
    /// Returns whether the unit is greater than another.
    ///
    /// \param unit The other unit.
    bool operator>(UnitType unit) const;

    ///
    /// Returns whether the unit is greater than or equal to another.
    ///
    /// \param unit The other unit.
    bool operator>=(UnitType unit) const;

    ///
    /// Returns whether the unit is equal to another.
    ///
    /// \param unit The other unit.
    bool operator==(UnitType unit) const;

    ///
    /// Returns whether the unit is not equal to another.
    ///
    /// \param unit The other unit.
    bool operator!=(UnitType unit) const;

    ///
    /// The value.
    ValueType value { ValueType() };
};

template <typename UnitType, typename ValueType>
Encoder& operator<<(Encoder& encoder, Unit<UnitType, ValueType> unit);

template <typename UnitType, typename ValueType>
Decoder& operator>>(Decoder& decoder, Unit<UnitType, ValueType>& unit);

}

#include "Unit.inl"
