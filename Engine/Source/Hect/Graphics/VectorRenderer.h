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
        /// Sets the font used for subsequenty render calls within the frame.
        ///
        /// \param font The font to set.
        /// \param size The font size.
        void setFont(const Font& font, double size);

        ///
        /// Renders a text string.
        ///
        /// \param text The text string to render.
        /// \param position The position to render the text.
        void renderText(const std::string& text, const Vector2& position);

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
