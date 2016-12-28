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

template <typename T, typename U>
Unit<T, U>::Unit()
{
}

template <typename T, typename U>
Unit<T, U>::Unit(U value) :
    value(value)
{
}

template <typename T, typename U>
T Unit<T, U>::operator+(T unit) const
{
    return T(value + unit.value);
}

template <typename T, typename U>
T Unit<T, U>::operator-(T unit) const
{
    return T(value - unit.value);
}

template <typename T, typename U>
T Unit<T, U>::operator*(U value) const
{
    return T(this->value * value);
}

template <typename T, typename U>
T Unit<T, U>::operator/(U value) const
{
    return T(this->value / value);
}

template <typename T, typename U>
Unit<T, U>& Unit<T, U>::operator+=(T unit)
{
    value += unit.value;
    return *this;
}

template <typename T, typename U>
Unit<T, U>& Unit<T, U>::operator-=(T unit)
{
    value -= unit.value;
    return *this;
}

template <typename T, typename U>
Unit<T, U>& Unit<T, U>::operator*=(U value)
{
    this->value *= value;
    return *this;
}

template <typename T, typename U>
Unit<T, U>& Unit<T, U>::operator/=(U value)
{
    this->value /= value;
    return *this;
}

template <typename T, typename U>
T Unit<T, U>::operator-() const
{
    return T(-value);
}

template <typename T, typename U>
bool Unit<T, U>::operator==(T unit) const
{
    return value == unit.value;
}

template <typename T, typename U>
bool Unit<T, U>::operator!=(T unit) const
{
    return value != unit.value;
}

template <typename T, typename U>
Encoder& operator<<(Encoder& encoder, Unit<T, U> unit)
{
    encoder << unit.value;
    return encoder;
}

template <typename T, typename U>
Decoder& operator>>(Decoder& decoder, Unit<T, U>& unit)
{
    decoder >> unit.value;
    return decoder;
}

}
