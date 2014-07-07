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
#include "DebugSystem.h"

using namespace hect;

DebugSystem::DebugSystem(Scene& scene) :
    System(scene)
{
}

void DebugSystem::addRenderLayer(Key::Enum toggleKey, DebugRenderLayer* renderLayer)
{
    assert(renderLayer);
    renderLayer->setToggleKey(toggleKey);
    _renderLayers.push_back(std::shared_ptr<DebugRenderLayer>(renderLayer));
}

void DebugSystem::renderActivatedRenderLayers(RenderSystem& renderSystem, Camera& camera, RenderTarget& target)
{
    for (auto& renderLayer : _renderLayers)
    {
        if (renderLayer->isActivated())
        {
            renderLayer->render(scene(), renderSystem, camera, target);
        }
    }
}

void DebugSystem::receiveEvent(const KeyboardEvent& event)
{
    if (event.type == KeyboardEventType::KeyDown)
    {
        for (auto& renderLayer : _renderLayers)
        {
            if (renderLayer->toggleKey() == event.key)
            {
                renderLayer->toggleActivated();
            }
        }
    }
}
