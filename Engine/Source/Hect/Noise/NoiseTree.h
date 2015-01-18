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
#include "Hect/Core/Uncopyable.h"
#include "Hect/Noise/NoiseNode.h"

namespace hect
{

///
/// An encapsulated composition of NoiseNode%s.
///
/// A noise tree allows for the creation and ownership of multiple noise
/// nodes, with a specfic node specified as the root.
class HECT_EXPORT NoiseTree :
    public Uncopyable
{
public:

    ///
    /// Constructs an empty noise tree.
    NoiseTree();

    ///
    /// Constructs a noise tree moved from another.
    ///
    /// \param noiseTree The noise tree to move.
    NoiseTree(NoiseTree&& noiseTree);

    ///
    /// Creates a new NoiseNode of the specified type.
    ///
    /// \note The lifetime of the created node is managed by the noise
    /// tree.
    ///
    /// \param args The arguments to pass to the node's constructor.
    ///
    /// \returns A reference to the created node.
    template <typename T, typename... Args>
    T& createNode(Args&&... args);

    ///
    /// Clears all nodes in the tree.
    void clear();

    ///
    /// Returns the root node in the tree.
    ///
    /// \throws InvalidOperation If no root node is set.
    NoiseNode& root();

    ///
    /// Sets the root node in the tree.
    ///
    /// \note It is expected that the node was created by the noise tree
    /// itself using NoiseTree::createNode().
    ///
    /// \param node The new root node.
    void setRoot(NoiseNode& node);

    ///
    /// Sets the noise tree as being moved from another.
    ///
    /// \param noiseTree The noise tree to move.
    NoiseTree& operator=(NoiseTree&& noiseTree);

private:
    NoiseNode* _rootNode { nullptr };
    std::vector<std::unique_ptr<NoiseNode>> _nodes;
};

}

#include "NoiseTree.inl"
