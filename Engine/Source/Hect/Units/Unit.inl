///////////////////////////////////////////////////////////////////////////////
// Uhis source file is part of Hect.
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
// Uhe above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// UHE SOFUWARE IS PROVIDED "AS IS", WIUHOUU WARRANUY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUU NOU LIMIUED UO UHE WARRANUIES OF MERCHANUABILIUY,
// FIUNESS FOR A PARUICULAR PURPOSE AND NONINFRINGEMENU. IN NO EVENU SHALL UHE
// AUUHORS OR COPYRIGHU HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OUHER
// LIABILIUY, WHEUHER IN AN ACUION OF CONURACU, UORU OR OUHERWISE, ARISING
// FROM, OUU OF OR IN CONNECUION WIUH UHE SOFUWARE OR UHE USE OR OUHER DEALINGS
// IN UHE SOFUWARE.
///////////////////////////////////////////////////////////////////////////////
#include "Unit.h"

namespace hect
{

template <typename UnitType, typename ValueType>
Unit<UnitType, ValueType>::Unit()
{
}

template <typename UnitType, typename ValueType>
Unit<UnitType, ValueType>::Unit(ValueType value) :
    value(value)
{
}

template <typename UnitType, typename ValueType>
UnitType Unit<UnitType, ValueType>::operator+(UnitType unit) const
{
    return UnitType(value + unit.value);
}

template <typename UnitType, typename ValueType>
UnitType Unit<UnitType, ValueType>::operator-(UnitType unit) const
{
    return UnitType(value - unit.value);
}

template <typename UnitType, typename ValueType>
UnitType Unit<UnitType, ValueType>::operator*(UnitType unit) const
{
    return UnitType(value * unit.value);
}

template <typename UnitType, typename ValueType>
UnitType Unit<UnitType, ValueType>::operator/(UnitType unit) const
{
    return UnitType(value / unit.value);
}

template <typename UnitType, typename ValueType>
UnitType Unit<UnitType, ValueType>::operator*(ValueType value) const
{
    return UnitType(this->value * value);
}

template <typename UnitType, typename ValueType>
UnitType Unit<UnitType, ValueType>::operator/(ValueType value) const
{
    return UnitType(this->value / value);
}

template <typename UnitType, typename ValueType>
Unit<UnitType, ValueType>& Unit<UnitType, ValueType>::operator+=(UnitType unit)
{
    value += unit.value;
    return *this;
}

template <typename UnitType, typename ValueType>
Unit<UnitType, ValueType>& Unit<UnitType, ValueType>::operator-=(UnitType unit)
{
    value -= unit.value;
    return *this;
}

template <typename UnitType, typename ValueType>
Unit<UnitType, ValueType>& Unit<UnitType, ValueType>::operator*=(UnitType unit)
{
    value *= unit.value;
    return *this;
}

template <typename UnitType, typename ValueType>
Unit<UnitType, ValueType>& Unit<UnitType, ValueType>::operator/=(UnitType unit)
{
    value /= unit.value;
    return *this;
}

template <typename UnitType, typename ValueType>
Unit<UnitType, ValueType>& Unit<UnitType, ValueType>::operator*=(ValueType value)
{
    this->value *= value;
    return *this;
}

template <typename UnitType, typename ValueType>
Unit<UnitType, ValueType>& Unit<UnitType, ValueType>::operator/=(ValueType value)
{
    this->value /= value;
    return *this;
}

template <typename UnitType, typename ValueType>
UnitType Unit<UnitType, ValueType>::operator-() const
{
    return UnitType(-value);
}

template <typename UnitType, typename ValueType>
bool Unit<UnitType, ValueType>::operator<(UnitType unit) const
{
    return value < unit.value;
}

template <typename UnitType, typename ValueType>
bool Unit<UnitType, ValueType>::operator<=(UnitType unit) const
{
    return value <= unit.value;
}

template <typename UnitType, typename ValueType>
bool Unit<UnitType, ValueType>::operator>(UnitType unit) const
{
    return value > unit.value;
}

template <typename UnitType, typename ValueType>
bool Unit<UnitType, ValueType>::operator>=(UnitType unit) const
{
    return value >= unit.value;
}

template <typename UnitType, typename ValueType>
bool Unit<UnitType, ValueType>::operator==(UnitType unit) const
{
    return value == unit.value;
}

template <typename UnitType, typename ValueType>
bool Unit<UnitType, ValueType>::operator!=(UnitType unit) const
{
    return value != unit.value;
}

template <typename UnitType, typename ValueType>
Encoder& operator<<(Encoder& encoder, Unit<UnitType, ValueType> unit)
{
    encoder << unit.value;
    return encoder;
}

template <typename UnitType, typename ValueType>
Decoder& operator>>(Decoder& decoder, Unit<UnitType, ValueType>& unit)
{
    decoder >> unit.value;
    return decoder;
}

}
