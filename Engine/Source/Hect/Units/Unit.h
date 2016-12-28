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

template <typename T, typename U>
class Unit
{
public:
    Unit();
    Unit(U value);

    T operator+(T unit) const;
    T operator-(T unit) const;
    T operator*(U value) const;
    T operator/(U value) const;

    Unit& operator+=(T unit);
    Unit& operator-=(T unit);
    Unit& operator*=(U value);
    Unit& operator/=(U value);

    T operator-() const;

    bool operator==(T unit) const;
    bool operator!=(T unit) const;

    U value { U() };
};

template <typename T, typename U>
Encoder& operator<<(Encoder& encoder, Unit<T, U> unit);

template <typename T, typename U>
Decoder& operator>>(Decoder& decoder, Unit<T, U>& unit);

}

#include "Unit.inl"
