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
#include "Hect/Graphics/VectorRenderer.h"
#include "Hect/Math/Vector2.h"

#include <memory>
#include <string>

namespace hect
{

///
/// Abstract base for Widget.
class HECT_EXPORT WidgetBase :
    public std::enable_shared_from_this<WidgetBase>
{
public:

    ///
    /// A handle to a widget.
    typedef std::shared_ptr<WidgetBase> Handle;

    ///
    /// Constructs a widget.
    WidgetBase();

    ///
    /// Constructs a widget.
    ///
    /// \param position The position of the widget.
    WidgetBase(const Vector2& position);

    ///
    /// Constructs a widget.
    ///
    /// \param position The position of the widget.
    /// \param dimensions The dimensions of the widget.
    WidgetBase(const Vector2& position, const Vector2& dimensions);

    virtual ~WidgetBase() { }

    ///
    /// Performs a single update tick for the widget.
    ///
    /// \param timeStep The duration of time in seconds for the tick to
    /// update.
    virtual void tick(double timeStep);

    ///
    /// Renders the widget to a frame.
    ///
    /// \param frame The frame to render to.
    virtual void render(VectorRenderer::Frame& frame) = 0;

    ///
    /// Returns the position of the widget.
    const Vector2& position() const;

    ///
    /// Sets the position of the widget.
    ///
    /// \param position The new position of the widget.
    void setPosition(const Vector2& position);

    ///
    /// Returns the dimensions of the widget.
    const Vector2& dimensions() const;

    ///
    /// Sets the dimensions of the widget.
    ///
    /// \param dimensions The new dimensions of the widget.
    void setDimensions(const Vector2& dimensions);

    ///
    /// Returns the tooltip of the widget.
    const std::string& tooltip() const;

    ///
    /// Sets the tooltip of the widget.
    ///
    /// \param tooltip The new tooltip of the widget.
    void setTooltip(const std::string& tooltip);

    ///
    /// Returns whether the widget is visible.
    bool visible() const;

    ///
    /// Sets whether the widget is visible.
    ///
    /// \param visible Whether the widget is visible.
    void setVisible(bool visible);

private:
    Vector2 _position;
    Vector2 _dimensions;
    std::string _tooltip;
    bool _visible { true };
};

}