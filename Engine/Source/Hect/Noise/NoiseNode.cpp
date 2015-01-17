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
#include"NoiseNode.h"

#include "Hect/Noise/NoiseFunction.h"
#include "Hect/Noise/NoiseNodeVisitor.h"

using namespace hect;

NoiseNode::NoiseNode(size_t maxSourceNodes) :
    _sourceNodes(maxSourceNodes, nullptr)
{
}

void NoiseNode::accept(NoiseNodeVisitor& visitor)
{
    (void)visitor;
}

void NoiseNode::clearSources()
{
    for (size_t index = 0; index < _sourceNodeCount; ++index)
    {
        _sourceNodes[index] = nullptr;
    }
    _sourceNodeCount = 0;
}

void NoiseNode::addSource(NoiseNode& node)
{
    if (_sourceNodes.empty())
    {
        throw InvalidOperation("Noise node does not support sources");
    }
    else if (_sourceNodeCount >= _sourceNodes.size())
    {
        throw InvalidOperation("Noise node already has the maximum allowed sources");
    }
    _sourceNodes[_sourceNodeCount++] = &node;
}

void NoiseNode::decode(Decoder& decoder, NoiseFunction& noiseFunction)
{
    (void)decoder;
    (void)noiseFunction;
}

NoiseNode& NoiseNode::source(size_t index) const
{
    if (index >= _sourceNodes.size())
    {
        throw InvalidOperation(format("Noise node does not have a source at index %i", index));
    }
    return *_sourceNodes[index];
}