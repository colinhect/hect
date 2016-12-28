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
    Unit();
    Unit(ValueType value);

    UnitType operator+(UnitType unit) const;
    UnitType operator-(UnitType unit) const;
    UnitType operator*(UnitType unit) const;
    UnitType operator/(UnitType unit) const;
    UnitType operator*(ValueType value) const;
    UnitType operator/(ValueType value) const;

    Unit& operator+=(UnitType unit);
    Unit& operator-=(UnitType unit);
    Unit& operator*=(UnitType unit);
    Unit& operator/=(UnitType unit);
    Unit& operator*=(ValueType value);
    Unit& operator/=(ValueType value);

    UnitType operator-() const;

    bool operator<(UnitType unit) const;
    bool operator<=(UnitType unit) const;
    bool operator>(UnitType unit) const;
    bool operator>=(UnitType unit) const;

    bool operator==(UnitType unit) const;
    bool operator!=(UnitType unit) const;

    ValueType value { ValueType() };
};

template <typename UnitType, typename ValueType>
Encoder& operator<<(Encoder& encoder, Unit<UnitType, ValueType> unit);

template <typename UnitType, typename ValueType>
Decoder& operator>>(Decoder& decoder, Unit<UnitType, ValueType>& unit);

}

#include "Unit.inl"
