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
#include "NoiseFunction.h"

#include "Hect/Noise/Nodes/AddNoiseNode.h"
#include "Hect/Noise/Nodes/CoherentNoiseNode.h"
#include "Hect/Noise/Nodes/ScalePositionNoiseNode.h"

using namespace hect;

void NoiseFunction::clear()
{
    _rootNode = nullptr;
    _nodes.clear();
}

void NoiseFunction::setRoot(NoiseNode& node)
{
    _rootNode = &node;
}

Real NoiseFunction::sample(const Vector3& position)
{
    Real value = 0;
    if (_rootNode)
    {
        value = _rootNode->sample(position);
    }
    return value;
}

NoiseNode& NoiseFunction::decodeNode(Decoder& decoder)
{
    std::string type;

    // Decode the type
    decoder >> beginObject()
            >> decodeValue("type", type, true);

    // AddNoiseNode
    if (type == "Add")
    {
        AddNoiseNode& node = createNode<AddNoiseNode>();
        node.decode(decoder, *this);
        decoder >> endObject();
        return node;
    }

    // CoherentNoiseNode
    else if (type == "Coherent")
    {
        CoherentNoiseNode& node = createNode<CoherentNoiseNode>();
        node.decode(decoder, *this);
        decoder >> endObject();
        return node;
    }

    // ScalePositionNoiseNode
    else if (type == "ScalePosition")
    {
        ScalePositionNoiseNode& node = createNode<ScalePositionNoiseNode>();
        node.decode(decoder, *this);
        decoder >> endObject();
        return node;
    }

    throw DecodeError(format("Unknown noise node type '%s'", type.c_str()));
}

namespace hect
{

Decoder& operator>>(Decoder& decoder, NoiseFunction& noiseFunction)
{
    // Clear the tree
    noiseFunction.clear();

    // Decode from the root node
    NoiseNode& node = noiseFunction.decodeNode(decoder);
    noiseFunction.setRoot(node);

    return decoder;
}

}
