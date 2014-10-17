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

#include <memory>
#include <typeindex>
#include <vector>

#include "Hect/Core/Real.h"
#include "Hect/Core/Sequence.h"
#include "Hect/Core/Uncopyable.h"

namespace hect
{

///
/// A numeric identifier for a system type.
typedef uint32_t SystemTypeId;

///
/// A system affecting entities within a scene.
class System :
    public Uncopyable
{
    friend class Scene;
public:

    typedef std::shared_ptr<System> Pointer;

    ///
    /// Constructs the system given the scene.
    System(Scene& scene);

    ///
    /// Ensures that the system of a specific type is updated before this
    /// system.
    template <typename T>
    void tickAfter();

    ///
    /// Performs a single tick of simulation for the system.
    ///
    /// \param timeStep The duration of time in seconds for the tick to
    /// simulate.
    virtual void tick(Real timeStep) = 0;

    ///
    /// Gets the scene that the system affects.
    Scene& scene();

    ///
    /// Gets the scene that the system affects.
    const Scene& scene() const;

private:
    Scene* _scene;

    std::vector<SystemTypeId> _tickDependencies;
};

}

#include "System.inl"