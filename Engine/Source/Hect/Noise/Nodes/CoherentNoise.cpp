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

#include "Hect/Noise/NoiseTree.h"
#include "Hect/Noise/NoiseTreeVisitor.h"
#include "Hect/Noise/Random.h"

using namespace hect;

CoherentNoise::CoherentNoise(RandomSeed seed) :
    _seed(seed)
{
    _permutationTable.reserve(512);
    generatePermuationTable();
}

RandomSeed CoherentNoise::seed() const
{
    return _seed;
}

void CoherentNoise::setSeed(RandomSeed seed)
{
    _seed = seed;
    generatePermuationTable();
}

double CoherentNoise::compute(const Vector3& point)
{
    // Based on Simplex Noise Demystified (c) 2005 Stefan Gustavson
    // http://webstaff.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf

    static Vector3 grad[12] =
    {
        Vector3(1, 1, 0), Vector3(-1, 1, 0), Vector3(1, -1, 0), Vector3(-1, -1, 0),
        Vector3(1, 0, 1), Vector3(-1, 0, 1), Vector3(1, 0, -1), Vector3(-1, 0, -1),
        Vector3(0, 1, 1), Vector3(0, -1, 1), Vector3(0, 1, -1), Vector3(0, -1, -1)
    };

    static double f3 = 1.0 / 3.0;
    static double g3 = 1.0 / 6.0;

    double x = point.x;
    double y = point.y;
    double z = point.z;

    double s = (x + y + z) * f3;
    int i = fastFloor(x + s);
    int j = fastFloor(y + s);
    int k = fastFloor(z + s);
    double t = (i + j + k) * g3;
    double X0 = i - t;
    double Y0 = j - t;
    double Z0 = k - t;
    double x0 = x - X0;
    double y0 = y - Y0;
    double z0 = z - Z0;

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

    double x1 = x0 - i1 + g3;
    double y1 = y0 - j1 + g3;
    double z1 = z0 - k1 + g3;
    double x2 = x0 - i2 + g3 * 2.0;
    double y2 = y0 - j2 + g3 * 2.0;
    double z2 = z0 - k2 + g3 * 2.0;
    double x3 = x0 - 1.0 + g3 * 3.0;
    double y3 = y0 - 1.0 + g3 * 3.0;
    double z3 = z0 - 1.0 + g3 * 3.0;

    int ii = i & 255;
    int jj = j & 255;
    int kk = k & 255;

    std::vector<uint8_t>& perm = _permutationTable;
    int gi0 = perm[ii + perm[jj + perm[kk]]] % 12;
    int gi1 = perm[ii + i1 + perm[jj + j1 + perm[kk + k1]]] % 12;
    int gi2 = perm[ii + i2 + perm[jj + j2 + perm[kk + k2]]] % 12;
    int gi3 = perm[ii + 1 + perm[jj + 1 + perm[kk + 1]]] % 12;

    double n0 = 0;
    double t0 = 0.5 - x0 * x0 - y0 * y0 - z0 * z0;
    if (t0 >= 0)
    {
        t0 *= t0;
        n0 = t0 * t0 * grad[gi0].dot(Vector3(x0, y0, z0));
    }

    double n1 = 0;
    double t1 = 0.5 - x1 * x1 - y1 * y1 - z1 * z1;
    if (t1 >= 0)
    {
        t1 *= t1;
        n1 = t1 * t1 * grad[gi1].dot(Vector3(x1, y1, z1));
    }

    double n2 = 0;
    double t2 = 0.5 - x2 * x2 - y2 * y2 - z2 * z2;
    if (t2 >= 0)
    {
        t2 *= t2;
        n2 = t2 * t2 * grad[gi2].dot(Vector3(x2, y2, z2));
    }

    double n3 = 0;
    double t3 = 0.5 - x3 * x3 - y3 * y3 - z3 * z3;
    if (t3 >= 0)
    {
        t3 *= t3;
        n3 = t3 * t3 * grad[gi3].dot(Vector3(x3, y3, z3));
    }

    return 32.0 * (n0 + n1 + n2 + n3);
}

void CoherentNoise::accept(NoiseTreeVisitor& visitor)
{
    visitor.visit(*this);
}

void CoherentNoise::generatePermuationTable()
{
    _permutationTable.clear();

    // Build permutation table
    for (unsigned i = 0; i < 256; ++i)
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
    for (unsigned i = 0; i < 256; ++i)
    {
        _permutationTable.push_back(_permutationTable[i]);
    }
}

int CoherentNoise::fastFloor(double x) const
{
    return x > 0 ? static_cast<int>(x) : static_cast<int>(x) - 1;
}