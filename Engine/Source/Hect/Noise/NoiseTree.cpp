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
#include "NoiseTree.h"

#include "Hect/Noise/Nodes/SimplexNoiseNode.h"

using namespace hect;

void NoiseTree::clear()
{
    _rootNode = nullptr;
    _nodes.clear();
}

NoiseNode& NoiseTree::root()
{
    if (!_rootNode)
    {
        throw InvalidOperation("No root noise node set");
    }
    return *_rootNode;
}

const NoiseNode& NoiseTree::root() const
{
    if (!_rootNode)
    {
        throw InvalidOperation("No root noise node set");
    }
    return *_rootNode;
}

void NoiseTree::setRoot(NoiseNode& node)
{
    _rootNode = &node;
}

NoiseNode& NoiseTree::decodeNode(Decoder& decoder, std::string& type)
{
    // Decode the type
    decoder >> beginObject()
            >> decodeValue("type", type, true);

    // SimplexNoiseNode
    if (type == "Simplex")
    {
        uint32_t seed = 0;
        decoder >> decodeValue("seed", seed);

        SimplexNoiseNode& node = createNode<SimplexNoiseNode>(seed);
        return node;
    }
    
    throw DecodeError(format("Unknown noise node type '%s'", type.c_str()));
}

namespace hect
{

Decoder& operator>>(Decoder& decoder, NoiseTree& noiseTree)
{
    // Clear the tree
    noiseTree.clear();

    // Decode from the root node
    std::string type;
    NoiseNode& node = noiseTree.decodeNode(decoder, type);
    noiseTree.setRoot(node);

    return decoder;
}

}
