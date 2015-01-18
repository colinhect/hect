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
#include "Hect/Math/Vector3.h"

namespace hect
{

class NoiseNodeVisitor;

///
/// A component of a NoiseModule that can be composed with other components.
class HECT_EXPORT NoiseNode
{
public:
    virtual ~NoiseNode() { }

    ///
    /// Computes the noise node at the given 3-dimensional point.
    ///
    /// \param point The 3-dimensional point to compute.
    ///
    /// \returns The value resulting from the computation.
    virtual Real compute(const Vector3& point) = 0;

    ///
    /// Accepts a visitor, invoking the NoiseNodeVisitor::visit() function
    /// defined for the specific node type.
    ///
    /// \param visitor The visitor to invoke.
    virtual void accept(NoiseNodeVisitor& visitor);
};

}