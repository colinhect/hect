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
#include "Hect/Noise/Nodes/AddNoise.h"
#include "Hect/Noise/Nodes/CoherentNoise.h"
#include "Hect/Noise/Nodes/FractalNoise.h"
#include "Hect/Noise/Nodes/MultiplyNoise.h"
#include "Hect/Noise/Nodes/RidgedNoise.h"
#include "Hect/Noise/Nodes/ScaleBiasNoise.h"
#include "Hect/Noise/Nodes/ScalePointNoise.h"
#include "Hect/Noise/Nodes/SubtractNoise.h"

namespace hect
{

///
/// Abstract interface for visiting an entire noise tree.
class HECT_EXPORT NoiseTreeVisitor
{
public:
    virtual ~NoiseTreeVisitor() { }

    ///
    /// Visits a CoherentNoise node.
    ///
    /// \param node The node to visit.
    virtual void visit(CoherentNoise& node) = 0;

    ///
    /// Visits a FractalNoise node.
    ///
    /// \param node The node to visit.
    virtual void visit(FractalNoise& node) = 0;

    ///
    /// Visits a RidgedNoise node.
    ///
    /// \param node The node to visit.
    virtual void visit(RidgedNoise& node) = 0;

    ///
    /// Visits an AddNoise node.
    ///
    /// \param node The node to visit.
    virtual void visit(AddNoise& node) = 0;

    ///
    /// Visits a SubtractNoise node.
    ///
    /// \param node The node to visit.
    virtual void visit(SubtractNoise& node) = 0;

    ///
    /// Visits a MultiplyNoise node.
    ///
    /// \param node The node to visit.
    virtual void visit(MultiplyNoise& node) = 0;

    ///
    /// Visits a ScaleBiasNoise node.
    ///
    /// \param node The node to visit.
    virtual void visit(ScaleBiasNoise& node) = 0;

    ///
    /// Visits a ScalePointNoise node.
    ///
    /// \param node The node to visit.
    virtual void visit(ScalePointNoise& node) = 0;
};

}