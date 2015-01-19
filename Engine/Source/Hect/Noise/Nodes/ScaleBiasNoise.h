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
/// Scales and adds a bias to the output of a node.
class HECT_EXPORT ScaleBiasNoise :
    public NoiseNode
{
public:

    ///
    /// Constructs a node that scales and adds a bias to the output of a node.
    ScaleBiasNoise();

    ///
    /// Constructs a node that scales and adds a bias to the output of a node.
    ///
    /// \param sourceNode The node whose output to scale.
    /// \param factor The scale factor.
    /// \param bias The bias to add to the output after it is scaled.
    ScaleBiasNoise(NoiseNode& sourceNode, double factor = 1.0, double bias = 0.0);

    ///
    /// Returns the node whose output to scale.
    ///
    /// \throws InvalidOperation if the source node is not set.
    NoiseNode& sourceNode() const;

    ///
    /// Sets the node whose output to scale.
    ///
    /// \param node The node.
    void setSourceNode(NoiseNode& node);

    ///
    /// Returns the scale factor.
    double factor() const;

    ///
    /// Sets the scale factor.
    ///
    /// \param factor The scale factor.
    void setFactor(double factor);

    ///
    /// Returns the bias to add to the output after it is scaled.
    double bias() const;

    ///
    /// Sets the bias to add to the output after it is scaled.
    ///
    /// \param bias The bias.
    void setBias(double bias);

    double compute(const Vector3& point) override;
    void accept(NoiseTreeVisitor& visitor) override;

private:
    NoiseNode* _sourceNode { nullptr };
    double _factor { 1.0 };
    double _bias { 0.0 };
};

}