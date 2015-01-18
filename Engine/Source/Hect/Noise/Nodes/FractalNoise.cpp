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
#include "FractalNoise.h"

#include "Hect/Noise/NoiseModule.h"
#include "Hect/Noise/NoiseNodeVisitor.h"
#include "Hect/Noise/Random.h"

using namespace hect;

FractalNoise::FractalNoise(RandomSeed seed, Real frequency, Real lacunarity, Real persistence, unsigned octaveCount) :
    _frequency(frequency),
    _lacunarity(lacunarity),
    _persistence(persistence)
{
    // Generate a coherent noise node for each octave
    Random random(seed);
    _octaveNoise.reserve(octaveCount);
    for (size_t octaveIndex = 0; octaveIndex < octaveCount; ++octaveIndex)
    {
        RandomSeed nextSeed = static_cast<RandomSeed>(random.next());
        _octaveNoise.push_back(CoherentNoise(nextSeed));
    }
}

Real FractalNoise::compute(const Vector3& position)
{
    Real value = 0;
    Real currentPersistence = 1;
    Vector3 currentPosition = position * _frequency;

    for (CoherentNoise& noise : _octaveNoise)
    {
        value += noise.compute(currentPosition) * currentPersistence;
        currentPosition *= _lacunarity;
        currentPersistence *= _persistence;
    }

    return value;
}

void FractalNoise::accept(NoiseNodeVisitor& visitor)
{
    visitor.visit(*this);
}
