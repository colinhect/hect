///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2016 Colin Hill
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
#include "InterfaceSystem.h"

#include "Hect/Runtime/Engine.h"

using namespace hect;

InterfaceSystem::InterfaceSystem(Engine& engine, Scene& scene) :
    System(engine, scene, SystemTickStage::Subsequent),
    _mouse(engine.mouse()),
    _renderer(engine.renderer()),
    _vectorRenderer(engine.vectorRenderer())
{
    if (engine.hasMouse())
    {
        engine.mouse().registerListener(*this);
    }
}

Form::Handle InterfaceSystem::createForm()
{
    Form::Handle form(new Form(*this, _renderer, _vectorRenderer));
    _forms.push_back(form);
    return form;
}

Vector2 InterfaceSystem::measureTextDimensions(const std::string& text, const Font& font, double size) const
{
    return _vectorRenderer.measureTextDimensions(text, font, size);
}

void InterfaceSystem::render(RenderTarget& target)
{
    for (const Form::Handle& form : _forms)
    {
        form->render(target);
    }
}

void InterfaceSystem::tick(double timeStep)
{
    for (const Form::Handle& form : _forms)
    {
        form->tick(timeStep);
    }
}

void InterfaceSystem::receiveEvent(const MouseEvent& event)
{
    if (_mouse.mode() == MouseMode::Cursor)
    {
        for (const Form::Handle& form : _forms)
        {
            form->receiveEvent(event);
        }
    }
}
