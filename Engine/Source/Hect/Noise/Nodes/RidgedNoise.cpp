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
    Random random(seed);
    Real weightFrequency = Real(1.0);
    _octaveNoise.reserve(octaveCount);
    _octaveWeights.reserve(octaveCount);
    
    // For each octave
    for (size_t octaveIndex = 0; octaveIndex < octaveCount; ++octaveIndex)
    {
        // Create a coherent noise node for this octave
        RandomSeed nextSeed = static_cast<RandomSeed>(random.next());
        _octaveNoise.push_back(CoherentNoise(nextSeed));

        // Compute the weight of this octave
        _octaveWeights.push_back(std::pow(weightFrequency, Real(-1.0)));
        weightFrequency *= _lacunarity;
    }
}

Real RidgedNoise::compute(const Vector3& point)
{
    Real value = Real(0.0);
    Real weight = Real(1.0);
    Vector3 currentPoint = point * _frequency;

    // For each octave
    size_t octaveIndex = 0;
    for (CoherentNoise& noise : _octaveNoise)
    {
        // Compute the coherent noise for this octave
        Real signal = noise.compute(currentPoint);

        // Adjust the value to be ridged
        signal = Real(1.0) - std::abs(signal);
        signal *= signal;
        signal *= weight;

        weight = signal * Real(2.0);
        weight = clamp(weight, Real(0.0), Real(1.0));

        value += signal * _octaveWeights[octaveIndex++];
        currentPoint *= _lacunarity;
    }

    return (value * Real(1.25)) - Real(1.0);
}

void RidgedNoise::accept(NoiseNodeVisitor& visitor)
{
    visitor.visit(*this);
}
