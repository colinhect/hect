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
#include "Hect/IO/Decoder.h"
#include "Hect/Math/Vector3.h"

namespace hect
{

class NoiseFunction;
class NoiseNodeVisitor;

///
/// A component of a NoiseFunction that can be composed with other components.
class HECT_EXPORT NoiseNode
{
public:

    ///
    /// Constructs a noise node.
    ///
    /// \param maxSourceNodes The maximum number of source nodes the node
    /// supports.
    NoiseNode(size_t maxSourceNodes = 0);

    virtual ~NoiseNode() { }

    ///
    /// Samples the noise function at the given 3-dimensional position.
    ///
    /// \param position The 3-dimensional position to sample.
    ///
    /// \returns The value resulting from the sample.
    virtual Real sample(const Vector3& position) = 0;

    ///
    /// Accepts a visitor, invoking the NoiseNodeVisitor::visit() function
    /// defined for the specific node type.
    ///
    /// \param visitor The visitor to invoke.
    virtual void accept(NoiseNodeVisitor& visitor);

    ///
    /// Clears all source nodes.
    void clearSources();

    ///
    /// Adds a source node.
    ///
    /// \param node The source node.
    ///
    /// \throws InvalidOperation If the node does not support source nodes or
    /// already has the maximum number of source nodes.
    void addSource(NoiseNode& node);

    ///
    /// Decodes the node from an object.
    ///
    /// \param decoder The decoder to use.
    /// \param noiseFunction The noise function that the node belongs to.
    virtual void decode(Decoder& decoder, NoiseFunction& noiseFunction);

protected:

    ///
    /// Returns the source node at the given index.
    ///
    /// \param index The source node index.
    ///
    /// \throws InvalidOperation If the node does not have a source node at the
    /// specified index.
    NoiseNode& source(size_t index) const;

private:
    std::vector<NoiseNode*> _sourceNodes;
    size_t _sourceNodeCount { 0 };
};

}