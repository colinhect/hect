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

#include <map>

#include "Hect/Debug/BoundingBoxDebugRenderLayer.h"
#include "Hect/Debug/TransformDebugRenderLayer.h"
#include "Hect/Event/Listener.h"
#include "Hect/Graphics/Systems/RenderSystem.h"
#include "Hect/Input/Input.h"
#include "Hect/Input/Keyboard.h"

namespace hect
{

///
/// Provides functionality useful for debugging.
class DebugRenderSystem :
    public RenderSystem,
    public Listener<KeyboardEvent>
{
public:
    DebugRenderSystem(Scene& scene, Input& input, AssetCache& assetCache, Renderer& renderer);
    ~DebugRenderSystem();

    ///
    /// Adds a debug render layer to the system.
    ///
    /// \param toggleKey The key which toggles the render layer.
    /// \param renderLayer The debug render layer.
    void addRenderLayer(Key toggleKey, DebugRenderLayer& renderLayer);

    void renderAll(RenderTarget& target);

    void receiveEvent(const KeyboardEvent& event);

private:
    Input* _input;

    std::vector<DebugRenderLayer*> _renderLayers;
    std::map<Key, DebugRenderLayer*> _toggleKeys;

    TransformDebugRenderLayer _transformDebugRenderLayer;
    BoundingBoxDebugRenderLayer _boundingBoxDebugRenderLayer;
};

}