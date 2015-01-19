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
#include "RidgedNoise.h"

#include "Hect/Noise/NoiseTree.h"
#include "Hect/Noise/NoiseTreeVisitor.h"
#include "Hect/Noise/Random.h"

using namespace hect;

RidgedNoise::RidgedNoise(RandomSeed seed, double lacunarity, unsigned octaveCount) :
    _seed(seed),
    _lacunarity(lacunarity),
    _octaveCount(octaveCount)
{
    generateOctaves();
}

RandomSeed RidgedNoise::seed() const
{
    return _seed;
}

void RidgedNoise::setSeed(RandomSeed seed)
{
    _seed = seed;
    generateOctaves();
}

double RidgedNoise::lacunarity() const
{
    return _lacunarity;
}

void RidgedNoise::setLacunarity(double lacunarity)
{
    _lacunarity = lacunarity;
    generateOctaves();
}

unsigned RidgedNoise::octaveCount() const
{
    return _octaveCount;
}

void RidgedNoise::setOctaveCount(unsigned octaveCount)
{
    _octaveCount = octaveCount;
    generateOctaves();
}

double RidgedNoise::compute(const Vector3& point)
{
    double value = 0.0;
    double weight = 1.0;
    Vector3 currentPoint = point;

    // For each octave
    size_t octaveIndex = 0;
    for (CoherentNoise& noise : _octaveNoise)
    {
        // Compute the coherent noise for this octave
        double signal = noise.compute(currentPoint);

        // Adjust the value to be ridged
        signal = 1.0 - std::abs(signal);
        signal *= signal;
        signal *= weight;

        weight = signal * 2.0;
        weight = std::max(0.0, std::min(weight, 1.0));

        value += signal * _octaveWeights[octaveIndex++];
        currentPoint *= _lacunarity;
    }

    return value * 1.25 - 1.0;
}

void RidgedNoise::accept(NoiseTreeVisitor& visitor)
{
    visitor.visit(*this);
}

void RidgedNoise::generateOctaves()
{
    Random random(_seed + 42);
    double frequency = 1.0;

    // Create and octave noise and weight vectors
    _octaveNoise = std::vector<CoherentNoise>(_octaveCount);
    _octaveWeights = std::vector<double>(_octaveCount);

    // For each octave
    for (size_t octaveIndex = 0; octaveIndex < _octaveCount; ++octaveIndex)
    {
        // Randomly seed this octave
        _octaveNoise[octaveIndex].setSeed(random.next());

        // Compute the weight of this octave
        _octaveWeights[octaveIndex] = std::pow(frequency, -1.0);
        frequency *= _lacunarity;
    }
}
