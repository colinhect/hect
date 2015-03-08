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

#include <cstdint>

#include "Hect/Core/Export.h"
#include "Hect/Core/Uncopyable.h"
#include "Hect/IO/Encodable.h"
#include "Hect/Logic/SystemTickStage.h"

namespace hect
{

class RenderTarget;
class Scene;

///
/// A numeric identifier for a system type.
typedef uint32_t SystemTypeId;

///
/// Abstract base for System.
class HECT_EXPORT SystemBase :
    public Uncopyable,
    public Encodable
{
    friend class Scene;
public:

    ///
    /// Constructs a system.
    ///
    /// \param scene The scene that the system exists in.
    /// \param tickStage The stage to tick the system in.
    SystemBase(Scene& scene, SystemTickStage tickStage = SystemTickStage_Normal);

    virtual ~SystemBase() { }

    ///
    /// Called before the first call to Scene::refresh().
    virtual void initialize();

    ///
    /// Performs a single tick of simulation for the system.
    ///
    /// \param timeStep The duration of time in seconds for the tick to
    /// simulate.
    virtual void tick(double timeStep);

    ///
    /// Renders the current state of the system to a target.
    ///
    /// \param target The target to render to.
    virtual void render(RenderTarget& target);

    ///
    /// Gets the scene that the system affects.
    Scene& scene();

    ///
    /// Gets the scene that the system affects.
    const Scene& scene() const;

    ///
    /// Returns the tick stage.
    SystemTickStage tickStage() const;

private:
    Scene& _scene;
    SystemTickStage _tickStage { SystemTickStage_Normal };
};

}
