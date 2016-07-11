///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2016 Colin Hill
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

#include <cstdint>

#include "Hect/Core/Export.h"
#include "Hect/Math/Vector2.h"
#include "Hect/Math/Vector3.h"
#include "Hect/Math/Vector4.h"

namespace hect
{

///
/// A seed for a random number generator.
typedef uint32_t RandomSeed;

///
/// Provides the functionality for generating psuedo-random numbers.
class HECT_EXPORT Random
{
public:

    ///
    /// Constructs a random number generator using a seed generated from the
    /// wall clock time.
    Random();

    ///
    /// Constructs a random number generator given a seed.
    ///
    /// \param seed The initial seed.
    Random(RandomSeed seed);

    ///
    /// Returns the next random number.
    uint32_t next();

    ///
    /// Returns the next random 8-bit unsigned integer within the specified
    /// range.
    ///
    /// \param min The minimum.
    /// \param max The maximum.
    uint8_t next(uint8_t min, uint8_t max);

    ///
    /// Returns the next random size within the specified range.
    ///
    /// \param min The minimum.
    /// \param max The maximum.
    size_t next(size_t min, size_t max);

    ///
    /// Returns the next random float within the specified range.
    ///
    /// \param min The minimum.
    /// \param max The maximum.
    float next(float min, float max);

    ///
    /// Returns the next random double within the specified range.
    ///
    /// \param min The minimum.
    /// \param max The maximum.
    double next(double min, double max);

    ///
    /// Returns the next random 2-dimensional vector within the specified
    /// range.
    ///
    /// \param min The minimum.
    /// \param max The maximum.
    Vector2 next(Vector2 min, Vector2 max);

    ///
    /// Returns the next random 3-dimensional vector within the specified
    /// range.
    ///
    /// \param min The minimum.
    /// \param max The maximum.
    Vector3 next(Vector3 min, Vector3 max);

    ///
    /// Returns the next random 4-dimensional vector within the specified
    /// range.
    ///
    /// \param min The minimum.
    /// \param max The maximum.
    Vector4 next(Vector4 min, Vector4 max);

private:
    uint32_t _current;
};

}