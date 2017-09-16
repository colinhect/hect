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

InterfaceSystem::InterfaceSystem(Scene& scene, Platform& platform, Renderer& renderer, VectorRenderer& vector_renderer) :
    System(scene),
    _platform(platform),
    _renderer(renderer),
    _vector_renderer(vector_renderer)
{
    platform.mouse().register_listener(*this);
}

Interface::Handle InterfaceSystem::create_interface(RenderTarget& render_target)
{
    Interface::Handle interface(new Interface(*this, render_target));
    _interfaces.push_back(interface);
    return interface;
}

Vector2 InterfaceSystem::measure_text_dimensions(const std::string& text, const Font& font, double size) const
{
    return _vector_renderer.measure_text_dimensions(text, font, size);
}

void InterfaceSystem::render_all_interfaces()
{
    for (const Interface::Handle& interface : _interfaces)
    {
        RenderTarget& interface_target = interface->render_target();

        Renderer::Frame frame = _renderer.begin_frame(interface_target);
        VectorRenderer::Frame vector_frame = _vector_renderer.begin_frame(interface_target);

        Rectangle clipping(0.0, 0.0, interface_target.width(), interface_target.height());
        interface->render(vector_frame, clipping);
    }
}

void InterfaceSystem::tick_all_interfaces(Seconds time_step)
{
    for (const Interface::Handle& interface : _interfaces)
    {
        interface->tick(time_step);
    }
}

void InterfaceSystem::receive_event(const MouseEvent& event)
{
    if (_platform.mouse().mode() == MouseMode::Cursor)
    {
        for (const Interface::Handle& interface : _interfaces)
        {
            if (!interface->has_parent())
            {
                if (interface->global_bounds().contains(event.cursor_position))
                {
                    interface->receive_event(event);
                }
            }
        }
    }
}
