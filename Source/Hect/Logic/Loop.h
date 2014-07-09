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
#include "Hect/Core/Timer.h"
#include "Hect/Core/TimeSpan.h"

namespace hect
{

///
/// A logic loop.
class Loop
{
public:
    Loop(TimeSpan timeStep);
    virtual ~Loop() { }

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
    /// Returns whether the loop is active.
    bool isActive() const;

    ///
    /// Sets whether the loop is active.
    ///
    /// \param active True if the loop is active; false otherwise.
    void setActive(bool active);

    ///
    /// Ticks the loop.
    ///
    /// \returns True if the loop is still active; false otherwise.
    bool tick();
    
private:
    bool _active;
    Timer _timer;
    TimeSpan _timeStep;
    TimeSpan _accumulator;
    TimeSpan _delta;
};

}