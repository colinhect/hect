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

#include "Hect/Math/Utilities.h"
#include "Hect/Noise/NoiseModule.h"
#include "Hect/Noise/NoiseNodeVisitor.h"
#include "Hect/Noise/Random.h"

using namespace hect;

RidgedNoise::RidgedNoise(RandomSeed seed, Real frequency, Real lacunarity, unsigned octaveCount) :
    _frequency(frequency),
    _lacunarity(lacunarity)
{
    Real h = 1;
    Real weightFrequency = 1;

    // Generate a coherent noise node and weight for each octave
    Random random(seed);
    _octaveNoise.reserve(octaveCount);
    for (size_t octaveIndex = 0; octaveIndex < octaveCount; ++octaveIndex)
    {
        RandomSeed nextSeed = static_cast<RandomSeed>(random.next());
        _octaveNoise.push_back(CoherentNoise(nextSeed));

        _octaveWeights.push_back(std::pow(weightFrequency, -h));
        weightFrequency *= _lacunarity;
    }
}

Real RidgedNoise::compute(const Vector3& position)
{
    Real value = 0;
    Real weight = 1;
    Real gain = 2;
    Vector3 currentPosition = position * _frequency;

    size_t octaveIndex = 0;
    for (CoherentNoise& noise : _octaveNoise)
    {
        Real octaveValue = noise.compute(currentPosition);
        octaveValue = Real(1) - std::abs(octaveValue);
        octaveValue *= octaveValue;
        octaveValue *= weight;

        weight = octaveValue * gain;
        weight = clamp(weight, Real(1), Real(0));

        value += octaveValue * _octaveWeights[octaveIndex++];
        currentPosition *= _lacunarity;
    }

    return value;
}

void RidgedNoise::accept(NoiseNodeVisitor& visitor)
{
    visitor.visit(*this);
}
