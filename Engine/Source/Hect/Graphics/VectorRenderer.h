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

#include "Hect/Core/Export.h"
#include "Hect/Core/Uncopyable.h"
#include "Hect/Graphics/Font.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/Graphics/RenderTarget.h"
#include "Hect/Interface/HorizontalAlign.h"
#include "Hect/Interface/VerticalAlign.h"
#include "Hect/Math/Vector2.h"

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
        void push_state();

        ///
        /// Pops the previously pushed state as the new current state of the
        /// frame.
        void pop_state();

        ///
        /// Translates the origin.
        ///
        /// \param translation The translation to apply.
        void translate(Vector2 translation);

        ///
        /// Begins a new path.
        void begin_path();

        ///
        /// Sets the active fill style to a solid color.
        ///
        /// \param color The color to set.
        void set_fill_color(Color color);

        ///
        /// Sets the active fill style to a linear gradient.
        ///
        /// \param start_position The start position.
        /// \param end_position The end position.
        /// \param start_color The start color.
        /// \param end_color The end color.
        void set_fill_gradient(Vector2 start_position, Vector2 end_position, Color start_color, Color end_color);

        ///
        /// Draws the current path with the current fill style.
        void fill();

        ///
        /// Sets the active stroke style to a solid color.
        ///
        /// \param color The color to set.
        void set_stroke_color(Color color);

        ///
        /// Draws the current path with the current stroke style.
        void stroke();

        ///
        /// Creates a new rectangle shape sub-path.
        ///
        /// \param rectangle The rectangle.
        void rectangle(Rectangle rectangle);

        ///
        /// Sets the clipping bounds for subsequent render calls within the
        /// frame.
        ///
        /// \param clipping The clipping bounds.
        void set_clipping(Rectangle clipping);

        ///
        /// Sets the font used for subsequent render calls within the frame.
        ///
        /// \param font The font to set.
        /// \param size The font size.
        void set_font(const Font& font, double size);

        ///
        /// Renders a text string.
        ///
        /// \param text The text string to render.
        void render_text(const std::string& text);

    private:
        Frame(VectorRenderer& renderer, RenderTarget& target);

        VectorRenderer& _renderer;
    };

    ///
    /// Pushes the state of a VectorRenderer::Frame for the duration of the
    /// scope's life time.
    class HECT_EXPORT FrameStateScope :
        public Uncopyable
    {
    public:

        ///
        /// Constructs a state scope given the frame.
        ///
        /// \param frame The frame to push the state for.
        FrameStateScope(Frame& frame);

        ~FrameStateScope();

    private:
        Frame& _frame;
    };

    ~VectorRenderer();

    ///
    /// Begins a new context for rendering vector graphics to a target.
    ///
    /// \param target The target to render the frame to.
    ///
    /// \throws InvalidOperation If another frame is already active.
    Frame begin_frame(RenderTarget& target);

    ///
    /// Returns the effective dimensions of the specified text using the given
    /// font and size.
    ///
    /// \param text The text to measure.
    /// \param font The font.
    /// \param size The font size.
    Vector2 measure_text_dimensions(const std::string& text, const Font& font, double size);

private:
    VectorRenderer(Renderer& renderer);

    void initialize();
    void shutdown();
    void on_begin_frame(RenderTarget& target);
    void on_end_frame();

    void set_font(const Font& font, double size);

    Renderer& _renderer;
    bool _in_frame { false };
};

}
