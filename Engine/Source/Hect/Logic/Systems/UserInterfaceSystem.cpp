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
#include "UserInterfaceSystem.h"

#include "Hect/Runtime/Engine.h"

using namespace hect;

UserInterfaceSystem::UserInterfaceSystem(Engine& engine, Scene& scene) :
    System(engine, scene, SystemTickStage::Subsequent),
    _renderer(engine.renderer()),
    _vectorRenderer(engine.vectorRenderer())
{
}

void UserInterfaceSystem::add(WidgetBase::Handle widget)
{
    _widgets.push_back(widget);
}

void UserInterfaceSystem::render(RenderTarget& target)
{
    Renderer::Frame frame = _renderer.beginFrame(target);
    VectorRenderer::Frame vectorFrame = _vectorRenderer.beginFrame(target);

    for (const WidgetBase::Handle& widget : _widgets)
    {
        if (widget->visible())
        {
            vectorFrame.pushState();
            vectorFrame.setClipping(widget->position(), widget->dimensions());
            widget->render(vectorFrame);
            vectorFrame.popState();
        }
    }
}

void UserInterfaceSystem::tick(double timeStep)
{
    for (const WidgetBase::Handle& widget : _widgets)
    {
        widget->tick(timeStep);
    }
}