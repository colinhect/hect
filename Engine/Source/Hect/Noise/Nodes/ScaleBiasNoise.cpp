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
#include "ScaleBiasNoise.h"

#include "Hect/Noise/NoiseTree.h"
#include "Hect/Noise/NoiseTreeVisitor.h"

using namespace hect;

ScaleBiasNoise::ScaleBiasNoise()
{
}

ScaleBiasNoise::ScaleBiasNoise(NoiseNode& sourceNode, double factor, double bias) :
    _sourceNode(&sourceNode),
    _factor(factor),
    _bias(bias)
{
}

NoiseNode& ScaleBiasNoise::sourceNode() const
{
    if (!_sourceNode)
    {
        throw InvalidOperation("Source node is not set");
    }
    return *_sourceNode;
}

void ScaleBiasNoise::setSourceNode(NoiseNode& node)
{
    _sourceNode = &node;
}

double ScaleBiasNoise::factor() const
{
    return _factor;
}

void ScaleBiasNoise::setFactor(double factor)
{
    _factor = factor;
}

double ScaleBiasNoise::bias() const
{
    return _bias;
}

void ScaleBiasNoise::setBias(double bias)
{
    _bias = bias;
}

double ScaleBiasNoise::compute(const Vector3& point)
{
    double value = 0.0;
    if (_sourceNode)
    {
        value = _sourceNode->compute(point) * _factor + _bias;
    }
    return value;
}

void ScaleBiasNoise::accept(NoiseTreeVisitor& visitor)
{
    visitor.visit(*this);
}