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

#include <cstdint>

#include "Hect/Core/Real.h"
#include "Hect/Core/Uncopyable.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"

namespace hect
{

class Scene;

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

    ///
    /// Constructs a system.
    ///
    /// \param scene The scene that the system exists in.
    System(Scene& scene);

    virtual ~System() { }

    ///
    /// Performs a single tick of simulation for the system.
    ///
    /// \param timeStep The duration of time in seconds for the tick to
    /// simulate.
    virtual void tick(Real timeStep);

    ///
    /// Gets the scene that the system affects.
    Scene& scene();

    ///
    /// Gets the scene that the system affects.
    const Scene& scene() const;

    ///
    /// Encodes the system to an object.
    ///
    /// \param encoder The encoder to use.
    virtual void encode(Encoder& encoder) const;

    ///
    /// Decodes the system from an object
    ///
    /// \param decoder The decoder to use.
    virtual void decode(Decoder& decoder);

private:
    Scene& _scene;
};

}
