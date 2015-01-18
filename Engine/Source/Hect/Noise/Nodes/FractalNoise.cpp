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

#include "Hect/Noise/NoiseTree.h"
#include "Hect/Noise/NoiseTreeVisitor.h"
#include "Hect/Noise/Random.h"

using namespace hect;

FractalNoise::FractalNoise(RandomSeed seed, Real lacunarity, Real persistence, unsigned octaveCount) :
    _seed(seed),
    _lacunarity(lacunarity),
    _persistence(persistence),
    _octaveCount(octaveCount)
{
    generateOctaves();
}

RandomSeed FractalNoise::seed() const
{
    return _seed;
}

void FractalNoise::setSeed(RandomSeed seed)
{
    _seed = seed;
    generateOctaves();
}

Real FractalNoise::lacunarity() const
{
    return _lacunarity;
}

void FractalNoise::setLacunarity(Real lacunarity)
{
    _lacunarity = lacunarity;
    generateOctaves();
}

Real FractalNoise::persistence() const
{
    return _persistence;
}

void FractalNoise::setPersistence(Real persistence)
{
    _persistence = persistence;
    generateOctaves();
}

unsigned FractalNoise::octaveCount() const
{
    return _octaveCount;
}

void FractalNoise::setOctaveCount(unsigned octaveCount)
{
    _octaveCount = octaveCount;
    generateOctaves();
}

Real FractalNoise::compute(const Vector3& point)
{
    Real value = Real(0.0);
    Real currentPersistence = Real(1.0);
    Vector3 currentPoint = point;

    // For each octave
    for (CoherentNoise& noise : _octaveNoise)
    {
        // Add the signal from this octave
        value += noise.compute(currentPoint) * currentPersistence;

        currentPoint *= _lacunarity;
        currentPersistence *= _persistence;
    }

    return value;
}

void FractalNoise::accept(NoiseTreeVisitor& visitor)
{
    visitor.visit(*this);
}

void FractalNoise::generateOctaves()
{
    Random random(_seed + 23);

    // Create and octave noise and weight vectors
    _octaveNoise = std::vector<CoherentNoise>(_octaveCount);

    // For each octave
    for (size_t octaveIndex = 0; octaveIndex < _octaveCount; ++octaveIndex)
    {
        // Randomly seed this octave
        _octaveNoise[octaveIndex].setSeed(random.next());
    }
}
