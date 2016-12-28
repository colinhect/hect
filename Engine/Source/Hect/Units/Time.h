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

#include "Hect/Core/Export.h"
#include "Hect/Units/Unit.h"

namespace hect
{

class Seconds;
class Minutes;
class Hours;

class HECT_EXPORT Seconds :
    public Unit<Seconds, double>
{
public:
    explicit Seconds(double value);
    Seconds(Minutes minutes);
    Seconds(Hours hours);
};

class HECT_EXPORT Minutes :
    public Unit<Minutes, double>
{
public:
    explicit Minutes(double value);
    Minutes(Seconds seconds);
    Minutes(Hours hours);
};

class HECT_EXPORT Hours :
    public Unit<Hours, double>
{
public:
    explicit Hours(double value);
    Hours(Seconds seconds);
    Hours(Minutes minutes);
};

}
