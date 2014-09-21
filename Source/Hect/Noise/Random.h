///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2014 Colin Hill
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

namespace hect
{

///
/// Provides the functionality for generating psuedo-random numbers.
class Random
{
public:

    ///
    /// Constructs a random number generator given a seed.
    ///
    /// \param seed The initial seed.
    Random(unsigned seed);

    ///
    /// Returns the next random number.
    unsigned next();

    ///
    /// Returns the next random number as an integer within a range.
    ///
    /// \param min The minimum value (must be less than max).
    /// \param max The maximum value (must be more than min).
    int nextInt32(int min, int max);

    ///
    /// Returns the next random number as a float within a range.
    ///
    /// \param min The minimum value (must be less than max).
    /// \param max The maximum value (must be more than min).
    float nextFloat32(float min, float max);

    ///
    /// Returns the next random number as a double within a range.
    ///
    /// \param min The minimum value (must be less than max).
    /// \param max The maximum value (must be more than min).
    double nextFloat64(double min, double max);

private:
    unsigned _current;
};

}