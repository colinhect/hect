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
#include "Hect/Logic/System.h"

namespace hect
{

///
/// Manages the user interfaces of a Scene.
///
/// \system
class HECT_EXPORT InterfaceSystem :
    public System<InterfaceSystem>,
    public Listener<MouseEvent>
{
public:
    InterfaceSystem(Engine& engine, Scene& scene);

    ///
    /// Creates a new interface.
    ///
    /// \param renderTarget The target to render to.
    ///
    /// \returns A handle to the new interface.
    Interface::Handle createInterface(RenderTarget& renderTarget);

    ///
    /// Returns the effective dimensions of the specified text using the given
    /// font and size.
    ///
    /// \param text The text to measure.
    /// \param font The font.
    /// \param size The font size.
    Vector2 measureTextDimensions(const std::string& text, const Font& font, double size) const;

    void render(RenderTarget& target) override;
    void tick(double timeStep) override;

    void receiveEvent(const MouseEvent& event) override;

    ///
    /// The font used when none is specified.
    ///
    /// \property{required}
    Font::Handle defaultFont;

    ///
    /// The font size used when none is specified.
    ///
    /// \property{required}
    double defaultFontSize;

private:
    Mouse& _mouse;
    Renderer& _renderer;
    VectorRenderer& _vectorRenderer;

    std::vector<Interface::Handle> _interfaces;
};

}
