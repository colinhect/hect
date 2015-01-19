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
#pragma once

#include "Hect/Core/Export.h"
#include "Hect/Noise/NoiseNode.h"
#include "Hect/Noise/Nodes/CoherentNoise.h"

namespace hect
{

///
/// Generates fractal Brownian motion noise.
class HECT_EXPORT FractalNoise :
    public NoiseNode
{
public:

    ///
    /// Constructs a node that generates fractal Brownian motion noise.
    ///
    /// \param seed The seed.
    /// \param lacunarity The lacunarity.
    /// \param persistence The persistence.
    /// \param octaveCount The number of octaves.
    FractalNoise(RandomSeed seed = 0, double lacunarity = 2.0, double persistence = 0.5, unsigned octaveCount = 6);

    ///
    /// Returns the seed.
    RandomSeed seed() const;

    ///
    /// Sets the seed.
    ///
    /// \param seed The new seed.
    void setSeed(RandomSeed seed);

    ///
    /// Returns the lacunarity.
    double lacunarity() const;

    ///
    /// Sets the lacunarity.
    ///
    /// \param lacunarity The new lacunarity.
    void setLacunarity(double lacunarity);

    ///
    /// Returns the persistence.
    double persistence() const;

    ///
    /// Sets the persistence.
    ///
    /// \param persistence The new persistence.
    void setPersistence(double persistence);

    ///
    /// Returns the octave count.
    unsigned octaveCount() const;

    ///
    /// Sets the octave count.
    ///
    /// \param octaveCount The new octave count.
    void setOctaveCount(unsigned octaveCount);

    double compute(const Vector3& point) override;
    void accept(NoiseTreeVisitor& visitor) override;

private:
    void generateOctaves();

    RandomSeed _seed;
    double _lacunarity;
    double _persistence;
    unsigned _octaveCount;
    std::vector<CoherentNoise> _octaveNoise;
};

}