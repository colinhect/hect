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

#include "Hect/Graphics/Components/Camera.h"
#include "Hect/Graphics/Systems/RenderSystem.h"

namespace hect
{

///
/// Renders an aspect of debug information.
class DebugRenderLayer
{
public:
    DebugRenderLayer();
    virtual ~DebugRenderLayer();

    ///
    /// Returns whether the layer is activated.
    bool isActivated() const;

    ///
    /// Toggles whether the layer is activated.
    void toggleActivated();

    ///
    /// Renders the debug render layer.
    ///
    /// \param world The world.
    /// \param renderSystem The render system to use.
    /// \param target The render target to render to.
    virtual void render(World& world, RenderSystem& renderSystem, RenderTarget& target) = 0;

private:
    bool _activated;
};

}