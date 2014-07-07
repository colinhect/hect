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

#include "Hect/Core/Listener.h"
#include "Hect/Input/Keyboard.h"
#include "Hect/Debug/DebugRenderLayer.h"

namespace hect
{

///
/// Provides functionality useful for debugging.
class DebugSystem :
    public System,
    public Listener<KeyboardEvent>
{
public:
    DebugSystem(Scene& scene);

    ///
    /// Adds a debug render layer to the system.
    ///
    /// \param toggleKey The key which toggles the render layer.
    /// \param renderLayer The debug render layer (the debug system takes
    /// ownership of this pointer).
    void addRenderLayer(Key::Enum toggleKey, DebugRenderLayer* renderLayer);

    ///
    /// Renders all activated debug render layers.
    ///
    /// \param renderSystem The render system to use.
    /// \param target The render target to render to.
    void renderActivatedRenderLayers(RenderSystem& renderSystem, RenderTarget& target);

    void receiveEvent(const KeyboardEvent& event);

private:
    std::vector<std::shared_ptr<DebugRenderLayer>> _renderLayers;
};

}