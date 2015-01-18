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
#include "CoherentNoise.h"

#include <algorithm>

#include "Hect/Noise/NoiseModule.h"
#include "Hect/Noise/NoiseNodeVisitor.h"
#include "Hect/Noise/Random.h"

using namespace hect;

CoherentNoise::CoherentNoise(RandomSeed seed, Real frequency) :
    _seed(seed),
    _frequency(frequency)
{
    _permutationTable.reserve(512);
    generatePermuationTable();
}

Real CoherentNoise::compute(const Vector3& point)
{
    // Based on Simplex Noise Demystified (c) 2005 Stefan Gustavson
    // http://webstaff.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf

    static Vector3 grad[12] =
    {
        Vector3(1, 1, 0), Vector3(-1, 1, 0), Vector3(1, -1, 0), Vector3(-1, -1, 0),
        Vector3(1, 0, 1), Vector3(-1, 0, 1), Vector3(1, 0, -1), Vector3(-1, 0, -1),
        Vector3(0, 1, 1), Vector3(0, -1, 1), Vector3(0, 1, -1), Vector3(0, -1, -1)
    };

    static Real f3 = Real(1.0) / 3;
    static Real g3 = Real(1.0) / 6;

    Real x = point.x * _frequency;
    Real y = point.y * _frequency;
    Real z = point.z * _frequency;

    Real s = (x + y + z) * f3;
    int i = fastFloor(x + s);
    int j = fastFloor(y + s);
    int k = fastFloor(z + s);
    Real t = (i + j + k) * g3;
    Real X0 = i - t;
    Real Y0 = j - t;
    Real Z0 = k - t;
    Real x0 = x - X0;
    Real y0 = y - Y0;
    Real z0 = z - Z0;

    int i1 = 0;
    int j1 = 0;
    int k1 = 0;
    int i2 = 0;
    int j2 = 0;
    int k2 = 0;
    if (x0 >= y0)
    {
        if (y0 >= z0)
        {
            i1 = 1;
            i2 = 1;
            j2 = 1;
        }
        else if (x0 >= z0)
        {
            i1 = 1;
            i2 = 1;
            k2 = 1;
        }
        else
        {
            k1 = 1;
            i2 = 1;
            k2 = 1;
        }
    }
    else
    {
        if (y0 < z0)
        {
            k1 = 1;
            j2 = 1;
            k2 = 1;
        }
        else if (x0 < z0)
        {
            j1 = 1;
            j2 = 1;
            k2 = 1;
        }
        else
        {
            j1 = 1;
            i2 = 1;
            j2 = 1;
        }
    }

    Real x1 = x0 - i1 + g3;
    Real y1 = y0 - j1 + g3;
    Real z1 = z0 - k1 + g3;
    Real x2 = x0 - i2 + g3 * 2;
    Real y2 = y0 - j2 + g3 * 2;
    Real z2 = z0 - k2 + g3 * 2;
    Real x3 = x0 - Real(1.0) + g3 * 3;
    Real y3 = y0 - Real(1.0) + g3 * 3;
    Real z3 = z0 - Real(1.0) + g3 * 3;

    int ii = i & 255;
    int jj = j & 255;
    int kk = k & 255;

    std::vector<uint8_t>& perm = _permutationTable;
    int gi0 = perm[ii + perm[jj + perm[kk]]] % 12;
    int gi1 = perm[ii + i1 + perm[jj + j1 + perm[kk + k1]]] % 12;
    int gi2 = perm[ii + i2 + perm[jj + j2 + perm[kk + k2]]] % 12;
    int gi3 = perm[ii + 1 + perm[jj + 1 + perm[kk + 1]]] % 12;

    Real n0 = 0;
    Real t0 = Real(0.5) - x0 * x0 - y0 * y0 - z0 * z0;
    if (t0 >= 0)
    {
        t0 *= t0;
        n0 = t0 * t0 * grad[gi0].dot(Vector3(x0, y0, z0));
    }

    Real n1 = 0;
    Real t1 = Real(0.5) - x1 * x1 - y1 * y1 - z1 * z1;
    if (t1 >= 0)
    {
        t1 *= t1;
        n1 = t1 * t1 * grad[gi1].dot(Vector3(x1, y1, z1));
    }

    Real n2 = 0;
    Real t2 = Real(0.5) - x2 * x2 - y2 * y2 - z2 * z2;
    if (t2 >= 0)
    {
        t2 *= t2;
        n2 = t2 * t2 * grad[gi2].dot(Vector3(x2, y2, z2));
    }

    Real n3 = 0;
    Real t3 = Real(0.5) - x3 * x3 - y3 * y3 - z3 * z3;
    if (t3 >= 0)
    {
        t3 *= t3;
        n3 = t3 * t3 * grad[gi3].dot(Vector3(x3, y3, z3));
    }

    return Real(32.0) * (n0 + n1 + n2 + n3);
}

void CoherentNoise::accept(NoiseNodeVisitor& visitor)
{
    visitor.visit(*this);
}

void CoherentNoise::generatePermuationTable()
{
    _permutationTable.clear();

    // Build permutation table
    for (size_t i = 0; i < 256; ++i)
    {
        _permutationTable.push_back(static_cast<uint8_t>(i));
    }

    // Shuffle permutation table
    Random random(_seed);
    std::random_shuffle(_permutationTable.begin(), _permutationTable.end(), [&random] (int i)
    {
        return random.next() % i;
    });

    // Extend permutation table to avoid index wrapping
    for (size_t i = 0; i < 256; ++i)
    {
        _permutationTable.push_back(_permutationTable[i]);
    }
}

int CoherentNoise::fastFloor(Real x) const
{
    return x > 0 ? static_cast<int>(x) : static_cast<int>(x) - 1;
}