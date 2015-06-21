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
#pragma once

#include <array>

#include "Hect/Core/Export.h"
#include "Hect/Graphics/Font.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/Graphics/VectorRenderer.h"
#include "Hect/Logic/System.h"
#include "Hect/UI/Widget.h"

namespace hect
{

///
/// Manages the user interface control Widget%s of a Scene.
///
/// \system
class HECT_EXPORT WidgetSystem :
    public System<WidgetSystem>
{
public:
    WidgetSystem(Engine& engine, Scene& scene);

    ///
    /// Adds a new widget.
    ///
    /// \param widget The widget to add.
    void add(WidgetBase::Handle widget);

    void render(RenderTarget& target) override;
    void tick(double timeStep) override;

    ///
    /// \property{required}
    Font::Handle defaultFont;

    ///
    /// \property{required}
    double defaultFontSize;

private:
    Renderer& _renderer;
    VectorRenderer& _vectorRenderer;

    std::vector<WidgetBase::Handle> _widgets;
};

}