///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2014 Colin Hill
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

#include "Hect/Core/Real.h"

namespace hect
{

///
/// A layer of logic within a flow.
class LogicLayer
{
public:
    LogicLayer();
    virtual ~LogicLayer() { }

    ///
    /// Called once every time step.
    ///
    /// \param timeStep The duration of time between each fixed update (in
    /// seconds).
    virtual void fixedUpdate(Real timeStep);

    ///
    /// Called once every frame.
    ///
    /// \param delta The delta between the last fixed update and the next
    /// fixed update.
    virtual void frameUpdate(Real delta);

    ///
    /// Returns whether the layer is active.
    bool isActive() const;

    ///
    /// Sets whether the layer is active.
    ///
    /// \param active True if the layer is active; false otherwise.
    void setActive(bool active);

private:
    bool _active;
};

}