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
#include "AddNoise.h"

#include "Hect/Noise/NoiseTree.h"
#include "Hect/Noise/NoiseTreeVisitor.h"

using namespace hect;

AddNoise::AddNoise()
{
}

AddNoise::AddNoise(NoiseNode& firstNode, NoiseNode& secondNode) :
    _firstNode(&firstNode),
    _secondNode(&secondNode)
{
}

NoiseNode& AddNoise::firstNode()
{
    if (!_firstNode)
    {
        throw InvalidOperation("First node is not set");
    }
    return *_firstNode;
}

void AddNoise::setFirstNode(NoiseNode& node);

NoiseNode& AddNoise::secondNode()
{
    if (!_secondNode)
    {
        throw InvalidOperation("Second node is not set");
    }
    return *_firstNode;
}

void AddNoise::setSecondNode(NoiseNode& node);

Real AddNoise::compute(const Vector3& point)
{
    Real value = Real(0.0);

    if (_firstNode)
    {
        value = _firstNode->compute(point);
    }
    
    if (_secondNode)
    {
        value += _secondNode->compute(point);
    }
    return value;
}

void AddNoise::accept(NoiseTreeVisitor& visitor)
{
    visitor.visit(*this);
}
