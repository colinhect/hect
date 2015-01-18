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
#include "Hect/IO/Asset.h"
#include "Hect/IO/Decoder.h"
#include "Hect/Noise/NoiseNode.h"

namespace hect
{

///
/// An encapsulated composition of NoiseNode%s.
///
/// A noise function allows for the creation and ownership of multiple noise
/// nodes or sub-functions (since a noise function is, itself, a node).  One of
/// function's nodes must be set as the root node before the function can be
/// sampled.
class HECT_EXPORT NoiseFunction :
    public Uncopyable,
    public Asset<NoiseFunction>,
    public NoiseNode
{
public:

    ///
    /// Creates a new NoiseNode of the specified type.
    ///
    /// \note The lifetime of the created node is managed by the noise
    /// function.
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
    /// Sets the root node in the tree.
    ///
    /// \note It is expected that the node was created by the noise function
    /// itself using NoiseFunction::createNode().
    ///
    /// \param node The new root node.
    void setRoot(NoiseNode& node);

    Real sample(const Vector3& position) override;

    ///
    /// Decodes a noise node from a decoder and adds it as a node to the
    /// function.
    ///
    /// \param decoder The decoder to use.
    ///
    /// \returns The decoded node.
    ///
    /// \throws DecodeError If the decoding failed.
    NoiseNode& decodeNode(Decoder& decoder);

    friend HECT_EXPORT Decoder& operator>>(Decoder& decoder, NoiseFunction& noiseFunction);

private:
    NoiseNode* _rootNode { nullptr };
    std::vector<std::unique_ptr<NoiseNode>> _nodes;
};

}

#include "NoiseFunction.inl"
