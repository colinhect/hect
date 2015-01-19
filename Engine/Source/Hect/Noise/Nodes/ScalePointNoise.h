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

namespace hect
{

///
/// Scales the point before a node is computed.
class HECT_EXPORT ScalePointNoise :
    public NoiseNode
{
public:

    ///
    /// Constructs a node that scales the point before a node is computed.
    ScalePointNoise();

    ///
    /// Constructs a node that scales the point before a node is computed.
    ///
    /// \param sourceNode The node to perform the point scaling on.
    /// \param factor The scale factor.
    ScalePointNoise(NoiseNode& sourceNode, const Vector3& factor = Vector3::one());

    ///
    /// Returns the node to perform the point scaling on.
    ///
    /// \throws InvalidOperation if the source node is not set.
    NoiseNode& sourceNode() const;

    ///
    /// Sets the node to perform the point scaling on.
    ///
    /// \param node The node.
    void setSourceNode(NoiseNode& node);

    ///
    /// Returns the scale factor.
    const Vector3& factor() const;

    ///
    /// Sets the scale factor.
    ///
    /// \param factor The scale factor.
    void setFactor(const Vector3& factor);

    double compute(const Vector3& point) override;
    void accept(NoiseTreeVisitor& visitor) override;

private:
    NoiseNode* _sourceNode { nullptr };
    Vector3 _factor { Vector3::one() };
};

}