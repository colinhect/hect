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

#include "Hect/Core/CollectionAccessor.h"
#include "Hect/Graphics/Pass.h"

namespace hect
{

///
/// An implementation of a material.
///
/// \note A technique is made up of multiple passes.
class Technique
{
public:

    ///
    /// Constructs a technique without any passes.
    Technique();

    ///
    /// Returns the passes.
    CollectionAccessor<Pass> passes();

    ///
    /// Returns the passes.
    const CollectionAccessor<Pass> passes() const;

    ///
    /// Adds a new pass.
    ///
    /// \param pass The new pass to add.
    void addPass(const Pass& pass);

    ///
    /// Removes all passes.
    void clearPasses();

private:
    std::vector<Pass> _passes;
};

}