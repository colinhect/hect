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
#include <cassert>

#include "Random.h"

using namespace hect;

Random::Random(unsigned seed) :
    _current(seed)
{
}

unsigned Random::next()
{
    unsigned output = 0;

    for (int i = 0; i < 16; ++i)
    {
        _current = 3039177861 * _current + 1;
        unsigned temp = _current >> 30;

        output = output << 2;
        output = output + temp;
    }

    return output;
}

int Random::nextInt(int min, int max)
{
    assert(min < max);

    int range = max - min;
    unsigned n = next() % range;

    return min + n;
}

float Random::nextFloat(float min, float max)
{
    assert(min < max);

    float range = max - min;
    float n = ((float)(next() % 100000000) * 0.00000001f) * range;

    return min + n;
}

double Random::nextDouble(double min, double max)
{
    assert(min < max);

    double range = max - min;
    double n = ((double)(next() % 100000000) * 0.00000001) * range;

    return min + n;
}