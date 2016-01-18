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
#include "Hect/Core/Event.h"
#include "Hect/Core/Uncopyable.h"
#include "Hect/Graphics/VectorRenderer.h"
#include "Hect/Input/MouseEvent.h"
#include "Hect/Interface/HorizontalAlign.h"
#include "Hect/Interface/StyleColor.h"
#include "Hect/Interface/VerticalAlign.h"
#include "Hect/Math/Rectangle.h"
#include "Hect/Math/Vector2.h"

#include <map>
#include <memory>
#include <string>

namespace hect
{

class InterfaceSystem;

///
/// Abstract base for Widget.
class HECT_EXPORT WidgetBase :
    public Listener<MouseEvent>,
    public Uncopyable
{
    friend class InterfaceSystem;
    friend class Form;
public:

    ///
    /// An action bound to a widget event.
    typedef std::function<void()> Action;

    ///
    /// A handle to a widget.
    typedef std::shared_ptr<WidgetBase> Handle;

    ///
    /// Constructs a widget.
    ///
    /// \param interfaceSystem The interface system.
    WidgetBase(InterfaceSystem& interfaceSystem);

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
    /// \param clipping The clipping bounds of the render area.
    virtual void render(VectorRenderer::Frame& frame, const Rectangle& clipping);

    ///
    /// Invoked when the mouse cursor enters the bounds of the widget.
    virtual void onMouseEnter();

    ///
    /// Invoked when the mouse cursor exits the bounds of the widget.
    virtual void onMouseExit();

    ///
    /// Invoked when the widget is pressed.
    virtual void onPressed();

    ///
    /// \copydoc Listener::receiveEvent()
    virtual void receiveEvent(const MouseEvent& event);

    ///
    /// Sets the action that is invoked when the widget is pressed.
    ///
    /// \param action The action to perform when the widget is pressed.
    void setPressAction(const WidgetBase::Action& action);

    ///
    /// Returns the local position of the widget (only relevant if the widget
    /// is not aligned).
    const Vector2& position() const;

    ///
    /// Sets the local position of the widget (only affects the widget if it is
    /// not aligned).
    ///
    /// \param position The new position of the widget.
    void setPosition(const Vector2& position);

    ///
    /// Returns the global position of the widget.
    Vector2 globalPosition() const;

    ///
    /// Returns the dimensions of the widget.
    const Vector2& dimensions() const;

    ///
    /// Sets the dimensions of the widget.
    ///
    /// \param dimensions The new dimensions of the widget.
    void setDimensions(const Vector2& dimensions);

    ///
    /// Returns the local bounds of the widget.
    Rectangle bounds() const;

    ///
    /// Returns the global bounds of the widget.
    Rectangle globalBounds() const;

    ///
    /// Returns the horizontal alignment.
    HorizontalAlign horizontalAlign() const;

    ///
    /// Sets the horizontal alignment of the widget.
    ///
    /// \param align The alignment.
    void setHorizontalAlign(HorizontalAlign align);

    ///
    /// Returns the vertical alignment.
    VerticalAlign verticalAlign() const;

    ///
    /// Sets the vertical alignment of the widget.
    ///
    /// \param align The alignment.
    void setVerticalAlign(VerticalAlign align);

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
    bool isVisible() const;

    ///
    /// Sets whether the widget is visible.
    ///
    /// \param visible Whether the widget is visible.
    void setVisible(bool visible);

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
    /// Returns whether the mouse cursor is currently over the bounds of the
    /// widget.
    bool isMouseOver() const;

    ///
    /// Returns whether the widget is currently pressed.
    bool isPressed() const;

    ///
    /// Creates a new child widget.
    ///
    /// \param args The arguments to pass to the widget's constructor.
    ///
    /// \returns A handle to the created child widget.
    template <typename T, typename... Args>
    typename T::Handle createChild(Args&&... args);

    ///
    /// Destroys a child widget.
    ///
    /// \param child The child widget to destroy.
    ///
    /// \throws InvalidOperation If the widget is not a child of the widget.
    void destroyChild(const WidgetBase::Handle& child);

    ///
    /// Returns whether the widget is a child of another widget.
    bool hasParent() const;

    ///
    /// Returns the parent of the widget.
    ///
    /// \throws InvalidOperation If the widget does not have a parent.
    WidgetBase& parent();

    ///
    /// \copydoc WidgetBase::parent()
    const WidgetBase& parent() const;

    ///
    /// Returns the interface system that the widget belongs to.
    InterfaceSystem& interfaceSystem();

    ///
    /// \copydoc WidgetBase::interfaceSystem()
    const InterfaceSystem& interfaceSystem() const;

protected:

    ///
    /// Updates the layout of the widget and its children.
    virtual void updateLayout();

    ///
    /// Returns whether the layout needs to be updated.
    bool isLayoutDirty() const;

    ///
    /// Mark the widget to update its layout when necessary.
    void markLayoutDirty();

    ///
    /// Sets whether the mouse cursor is currently hovering over the widget.
    void setMouseOver(bool value);

private:
    InterfaceSystem& _interfaceSystem;
    WidgetBase* _parent { nullptr };

    Vector2 _assignedPosition;
    Vector2 _actualPosition;

    Vector2 _dimensions;

    HorizontalAlign _horizontalAlign { HorizontalAlign::None };
    VerticalAlign _verticalAlign { VerticalAlign::None };

    bool _layoutDirty { true };

    std::string _tooltip;

    Action _pressAction;

    bool _pressed { false };
    bool _visible { true };
    bool _mouseOver { false };

    std::vector<WidgetBase::Handle> _children;
    std::vector<WidgetBase::Handle> _childrenToDestroy;
    std::map<StyleColor, Color> _styleColors;
};

}

#include "WidgetBase.inl"
