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
/// Computes the sum of the output of two nodes.
class HECT_EXPORT AddNoise :
    public NoiseNode
{
public:

    ///
    /// Constructs a node that computes the sum of the output of two nodes.
    AddNoise();

    ///
    /// Constructs a node that computes the sum of the output of two nodes.
    ///
    /// \param firstNode The node whose output is being added to.
    /// \param secondNode The node whose output is being added.
    AddNoise(NoiseNode& firstNode, NoiseNode& secondNode);

    ///
    /// Returns the node whose output is being added to.
    ///
    /// \throws InvalidOperation if the first node is not set.
    NoiseNode& firstNode();

    ///
    /// Sets the node whose output is being added to.
    ///
    /// \param node The node.
    void setFirstNode(NoiseNode& node);

    ///
    /// Returns the node whose output is being added.
    ///
    /// \throws InvalidOperation if the second node is not set.
    NoiseNode& secondNode();

    ///
    /// Sets the node whose output is being added.
    ///
    /// \param node The node.
    void setSecondNode(NoiseNode& node);

    double compute(const Vector3& point) override;
    void accept(NoiseTreeVisitor& visitor) override;

private:
    NoiseNode* _firstNode { nullptr };
    NoiseNode* _secondNode { nullptr };
};

}