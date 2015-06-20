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

#include "Hect/Core/Export.h"
#include "Hect/Core/Uncopyable.h"
#include "Hect/Graphics/Font.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/Graphics/RenderTarget.h"
#include "Hect/Math/Vector2.h"
#include "Hect/UI/HorizontalAlign.h"
#include "Hect/UI/VerticalAlign.h"

namespace hect
{

///
/// Provides the functionality for rendering vector graphics.
class HECT_EXPORT VectorRenderer :
    public Uncopyable
{
    friend class Engine;
public:

    ///
    /// Provides a context for rendering to a target.
    class HECT_EXPORT Frame :
        public Uncopyable
    {
        friend class VectorRenderer;
    public:
        Frame(Frame&& frame);
        ~Frame();

        ///
        /// Pushes the current state of the frame onto the stack.
        void pushState();

        ///
        /// Pops the previously pushed state as the new current state of the
        /// frame.
        void popState();

        ///
        /// Begins a new path.
        void beginPath();

        ///
        /// Sets the active fill style to a solid color.
        ///
        /// \param color The color to set.
        void setColor(const Color& color);

        ///
        /// Fills the current path with the current fill style.
        void fillPath();

        ///
        /// Creates a new rectangle shape sub-path.
        ///
        /// \param position The position of the rectangle.
        /// \param dimensions The dimensions of the rectangle.
        void rectangle(const Vector2& position, const Vector2& dimensions);

        ///
        /// Sets the clipping bounds for subsequent render calls within the
        /// frame.
        ///
        /// \param position The position.
        /// \param dimensions The dimensions.
        void setClipping(const Vector2& position, const Vector2& dimensions);

        ///
        /// Sets the font used for subsequent render calls within the frame.
        ///
        /// \param font The font to set.
        /// \param size The font size.
        void setFont(const Font& font, double size);

        ///
        /// Renders a text string.
        ///
        /// \param text The text string to render.
        /// \param position The position to render the text.
        /// \param dimensions The dimensions to render the text.
        /// \param horizontalAlign The horizontal alignment.
        /// \param verticalAlign The vertical alignment.
        void renderText(const std::string& text, const Vector2& position, const Vector2& dimensions, HorizontalAlign horizontalAlign = HorizontalAlign::Left, VerticalAlign verticalAlign = VerticalAlign::Top);

    private:
        Frame(VectorRenderer& renderer, RenderTarget& target);

        VectorRenderer& _renderer;
    };

    ~VectorRenderer();

    ///
    /// Begins a new context for rendering vector graphics to a target.
    ///
    /// \param target The target to render the frame to.
    ///
    /// \throws InvalidOperation If another frame is already active.
    Frame beginFrame(RenderTarget& target);

private:
    VectorRenderer(Renderer& renderer);

    void initialize();
    void shutdown();
    void onBeginFrame(RenderTarget& target);
    void onEndFrame();

    Renderer& _renderer;
    bool _inFrame { false };
};

}
