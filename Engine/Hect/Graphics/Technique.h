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

#include "Hect/Core/Sequence.h"
#include "Hect/Graphics/Pass.h"

namespace hect
{

///
/// A specific technique to achieve the effect of a material.
class Technique
{
    typedef std::vector<Pass> PassContainer;
public:

    ///
    /// A sequence of passes.
    typedef Sequence<Pass, PassContainer> PassSequence;

    ///
    /// Returns the passes.
    PassSequence passes();

    ///
    /// Returns the passes.
    const PassSequence passes() const;

    ///
    /// Adds a new pass.
    ///
    /// \param pass The new pass to add.
    void addPass(const Pass& pass);

    ///
    /// Removes all passes.
    void clearPasses();

    ///
    /// Returns whether the technique is equivalent to another.
    ///
    /// \param technique The other technique.
    bool operator==(const Technique& technique) const;

    ///
    /// Returns whether the technique is different from another.
    ///
    /// \param technique The other technique.
    bool operator!=(const Technique& technique) const;

    friend Encoder& operator<<(Encoder& encoder, const Technique& technique);
    friend Decoder& operator>>(Decoder& decoder, Technique& technique);

private:
    PassContainer _passes;
};

}