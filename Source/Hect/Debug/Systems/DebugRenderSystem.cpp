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
#include "DebugRenderSystem.h"

#include "Hect/Logic/World.h"
#include "Hect/Platform/Platform.h"
#include "Hect/Runtime/Engine.h"

using namespace hect;

DebugRenderSystem::DebugRenderSystem(World& world) :
    RenderSystem(world, Engine::renderer()),
    _transformDebugRenderLayer(Engine::assetCache()),
    _boundingBoxDebugRenderLayer(Engine::assetCache())
{
    Platform::keyboard().addListener(*this);

    // Add debug render layers
    addRenderLayer(Key_F5, _transformDebugRenderLayer);
    addRenderLayer(Key_F6, _boundingBoxDebugRenderLayer);
}

DebugRenderSystem::~DebugRenderSystem()
{
    Platform::keyboard().removeListener(*this);
}

void DebugRenderSystem::addRenderLayer(Key toggleKey, DebugRenderLayer& renderLayer)
{
    _toggleKeys[toggleKey] = &renderLayer;
    _renderLayers.push_back(&renderLayer);
}

void DebugRenderSystem::renderAll(RenderTarget& target)
{
    for (DebugRenderLayer* renderLayer : _renderLayers)
    {
        if (renderLayer->isActivated())
        {
            renderLayer->render(world(), *this, target);
        }
    }
}

void DebugRenderSystem::receiveEvent(const KeyboardEvent& event)
{
    if (event.type == KeyboardEventType_KeyDown)
    {
        auto it = _toggleKeys.find(event.key);
        if (it != _toggleKeys.end())
        {
            it->second->toggleActivated();
        }
    }
}
