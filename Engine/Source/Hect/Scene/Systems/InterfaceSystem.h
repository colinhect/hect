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
#pragma once

#include <vector>

#include "Hect/Core/Export.h"
#include "Hect/Graphics/Font.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/Graphics/VectorRenderer.h"
#include "Hect/Interface/Interface.h"
#include "Hect/Scene/System.h"
#include "Hect/Runtime/Platform.h"

namespace hect
{

///
/// Manages the user interfaces of a Scene.
///
/// \system
class HECT_EXPORT InterfaceSystem :
    public System<InterfaceSystem>,
    public EventListener<MouseEvent>
{
public:
    InterfaceSystem(Scene& scene, Platform& platform, Renderer& renderer, VectorRenderer& vector_renderer);

    ///
    /// Creates a new interface.
    ///
    /// \param render_target The target to render to.
    ///
    /// \returns A handle to the new interface.
    Interface::Handle create_interface(RenderTarget& render_target);

    ///
    /// Returns the effective dimensions of the specified text using the given
    /// font and size.
    ///
    /// \param text The text to measure.
    /// \param font The font.
    /// \param size The font size.
    Vector2 measure_text_dimensions(const std::string& text, const Font& font, double size) const;

    ///
    /// Renders all interfaces to their render targets.
    void render_all_interfaces();

    ///
    /// Performs a single tick of simulation for all interfaces.
    ///
    /// \param time_step The duration of time to tick.
    void tick_all_interfaces(Seconds time_step);

    ///
    /// The font used when none is specified.
    ///
    /// \property{required}
    Font::Handle default_font;

    ///
    /// The font size used when none is specified.
    ///
    /// \property{required}
    double default_font_size;

private:
    // EventListener overrides
    void receive_event(const MouseEvent& event) override;

    Platform& _platform;
    Renderer& _renderer;
    VectorRenderer& _vector_renderer;

    std::vector<Interface::Handle> _interfaces;
};

}
