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
#include "Random.h"

#include <cassert>
#include <ctime>
#include <limits>

using namespace hect;

Random::Random() :
    _current(static_cast<RandomSeed>(std::time(nullptr)))
{
}

Random::Random(RandomSeed seed) :
    _current(seed)
{
}

uint32_t Random::next()
{
    uint32_t output = 0;

    for (int i = 0; i < 16; ++i)
    {
        _current = 3039177861 * _current + 1;
        uint32_t temp = _current >> 30;

        output = output << 2;
        output = output + temp;
    }

    return output;
}

uint8_t Random::next(uint8_t min, uint8_t max)
{
    assert(min < max);
    return next() % (max - min) + min;
}

size_t Random::next(size_t min, size_t max)
{
    assert(min < max);
    return next() % (max - min) + min;
}

float Random::next(float min, float max)
{
    assert(min < max);
    float random = static_cast<float>(next()) / static_cast<float>(std::numeric_limits<uint32_t>::max());
    return random * (max - min) + min;
}

double Random::next(double min, double max)
{
    assert(min < max);
    double random = static_cast<double>(next()) / static_cast<double>(std::numeric_limits<uint32_t>::max());
    return random * (max - min) + min;
}

Vector2 Random::next(const Vector2& min, const Vector2& max)
{
    double x = next(min.x, max.x);
    double y = next(min.y, max.y);
    return Vector2(x, y);
}

Vector3 Random::next(const Vector3& min, const Vector3& max)
{
    double x = next(min.x, max.x);
    double y = next(min.y, max.y);
    double z = next(min.z, max.z);
    return Vector3(x, y, z);
}

Vector4 Random::next(const Vector4& min, const Vector4& max)
{
    double x = next(min.x, max.x);
    double y = next(min.y, max.y);
    double z = next(min.z, max.z);
    double w = next(min.w, max.w);
    return Vector4(x, y, z, w);
}
