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

#include <array>

#include "Hect/Core/Event.h"
#include "Hect/Core/Export.h"
#include "Hect/Core/Uncopyable.h"
#include "Hect/Graphics/Font.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/Graphics/RenderTarget.h"
#include "Hect/Graphics/VectorRenderer.h"
#include "Hect/Input/MouseEvent.h"
#include "Hect/Interface/StyleColor.h"
#include "Hect/Interface/Widget.h"

namespace hect
{

class InterfaceSystem;

///
/// A collection of widgets making up a user interface.
class HECT_EXPORT Form :
    public Listener<MouseEvent>,
    public Uncopyable
{
    friend class InterfaceSystem;
public:

    ///
    /// A handle to a form.
    typedef std::shared_ptr<Form> Handle;

    ///
    /// Creates a new widget.
    ///
    /// \param args The arguments to pass to the widget's constructor.
    ///
    /// \returns A handle to the added widget.
    template <typename T, typename... Args>
    typename T::Handle createWidget(Args&&... args);

    ///
    /// Destroys a widget.
    ///
    /// \param widget A handle to the widget to remove.
    void destroyWidget(const WidgetBase::Handle& widget);

    ///
    /// Returns the effective dimensions of the specified text using the given
    /// font and size.
    ///
    /// \param text The text to measure.
    /// \param font The font.
    /// \param size The font size.
    Vector2 measureTextDimensions(const std::string& text, const Font& font, double size) const;

    ///
    /// Returns a style color.
    ///
    /// \param styleColor The style color to get.
    const Color& styleColor(StyleColor styleColor) const;

    ///
    /// Sets a style color.
    ///
    /// \param styleColor The style color to set.
    /// \param color The new color value of the style color.
    void setStyleColor(StyleColor styleColor, const Color& color);

    ///
    /// Renders the form to its render target.
    ///
    /// \param target The target to render to.
    void render(RenderTarget& target);

    ///
    /// Performs a single update tick for all widgets in the form.
    ///
    /// \param timeStep The duration of time in seconds for the tick to
    /// update.
    void tick(double timeStep);

    ///
    /// Returns the interface system the form belongs to.
    InterfaceSystem& interfaceSystem();

    ///
    /// \copydoc Form::interfaceSystem()
    const InterfaceSystem& interfaceSystem() const;

    void receiveEvent(const MouseEvent& event) override;

private:
    Form(InterfaceSystem& interfaceSystem, Renderer& renderer, VectorRenderer& vectorRenderer);

    InterfaceSystem& _interfaceSystem;
    Renderer& _renderer;
    VectorRenderer& _vectorRenderer;

    std::vector<WidgetBase::Handle> _widgets;
    std::vector<WidgetBase::Handle> _destroyedWidgets;
    std::map<StyleColor, Color> _styleColors;
};

}

#include "Form.inl"
