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

#include "Hect/Core/EventListener.h"
#include "Hect/Core/Export.h"
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
    public EventListener<MouseEvent>,
    public Uncopyable
{
    friend class InterfaceSystem;
    friend class Interface;
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
    /// \param interface_system The interface system.
    WidgetBase(InterfaceSystem& interface_system);

    virtual ~WidgetBase() { }

    ///
    /// Performs a single update tick for the widget.
    ///
    /// \param time_step The duration of time for the tick to update.
    virtual void tick(Seconds time_step);

    ///
    /// Renders the widget to a frame.
    ///
    /// \param frame The frame to render to.
    /// \param clipping The clipping bounds of the render area.
    virtual void render(VectorRenderer::Frame& frame, Rectangle clipping);

    ///
    /// Invoked when the mouse cursor enters the bounds of the widget.
    virtual void on_mouse_enter();

    ///
    /// Invoked when the mouse cursor exits the bounds of the widget.
    virtual void on_mouse_exit();

    ///
    /// Invoked when the widget is pressed.
    virtual void on_pressed();

    ///
    /// \copydoc EventListener::receive_event()
    virtual void receive_event(const MouseEvent& event);

    ///
    /// Sets the action that is invoked when the widget is pressed.
    ///
    /// \param action The action to perform when the widget is pressed.
    void set_press_action(const WidgetBase::Action& action);

    ///
    /// Returns the local position of the widget.
    Vector2 position() const;

    ///
    /// Sets the local position of the widget (only affects the widget if it is
    /// not aligned).
    ///
    /// \param position The new position of the widget.
    void set_position(Vector2 position);

    ///
    /// Returns the global position of the widget.
    Vector2 global_position() const;

    ///
    /// Returns the dimensions of the widget.
    Vector2 dimensions() const;

    ///
    /// Sets the dimensions of the widget.
    ///
    /// \param dimensions The new dimensions of the widget.
    void set_dimensions(Vector2 dimensions);

    ///
    /// Returns the local bounds of the widget.
    Rectangle bounds() const;

    ///
    /// Returns the global bounds of the widget.
    Rectangle global_bounds() const;

    ///
    /// Returns the horizontal alignment.
    HorizontalAlign horizontal_align() const;

    ///
    /// Sets the horizontal alignment of the widget.
    ///
    /// \param align The alignment.
    void set_horizontal_align(HorizontalAlign align);

    ///
    /// Returns the vertical alignment.
    VerticalAlign vertical_align() const;

    ///
    /// Sets the vertical alignment of the widget.
    ///
    /// \param align The alignment.
    void set_vertical_align(VerticalAlign align);

    ///
    /// Returns the tooltip of the widget.
    const std::string& tooltip() const;

    ///
    /// Sets the tooltip of the widget.
    ///
    /// \param tooltip The new tooltip of the widget.
    void set_tooltip(const std::string& tooltip);

    ///
    /// Returns whether the widget is visible.
    bool is_visible() const;

    ///
    /// Sets whether the widget is visible.
    ///
    /// \param visible Whether the widget is visible.
    void set_visible(bool visible);

    ///
    /// Returns a style color.
    ///
    /// \param style_color The style color to get.
    Color style_color(StyleColor style_color) const;

    ///
    /// Sets a style color.
    ///
    /// \param style_color The style color to set.
    /// \param color The new color value of the style color.
    void set_style_color(StyleColor style_color, Color color);

    ///
    /// Returns whether the mouse cursor is currently over the bounds of the
    /// widget.
    bool is_mouse_over() const;

    ///
    /// Returns whether the widget is currently pressed.
    bool is_pressed() const;

    ///
    /// Creates a new child widget.
    ///
    /// \param args The arguments to pass to the widget's constructor.
    ///
    /// \returns A handle to the created child widget.
    template <typename T, typename... Args>
    typename T::Handle create_child(Args&&... args);

    ///
    /// Destroys a child widget.
    ///
    /// \param child The child widget to destroy.
    ///
    /// \throws InvalidOperation If the widget is not a child of the widget.
    void destroy_child(const WidgetBase::Handle& child);

    ///
    /// Returns whether the widget is a child of another widget.
    bool has_parent() const;

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
    InterfaceSystem& interface_system();

    ///
    /// \copydoc WidgetBase::interface_system()
    const InterfaceSystem& interface_system() const;

protected:

    ///
    /// Updates the layout of the widget and its children.
    virtual void update_layout();

    ///
    /// Returns whether the layout needs to be updated.
    bool is_layout_dirty() const;

    ///
    /// Mark the widget to update its layout when necessary.
    void mark_layout_dirty();

    ///
    /// Sets whether the mouse cursor is currently hovering over the widget.
    void set_mouse_over(bool value);

private:
    InterfaceSystem& _interface_system;
    WidgetBase* _parent { nullptr };

    Vector2 _assigned_position;
    Vector2 _actual_position;

    Vector2 _dimensions;

    HorizontalAlign _horizontal_align { HorizontalAlign::None };
    VerticalAlign _vertical_align { VerticalAlign::None };

    bool _layout_dirty { true };

    std::string _tooltip;

    Action _press_action;

    bool _pressed { false };
    bool _visible { true };
    bool _mouse_over { false };

    std::vector<WidgetBase::Handle> _children;
    std::vector<WidgetBase::Handle> _children_to_destroy;
    std::map<StyleColor, Color> _style_colors;
};

}

#include "WidgetBase.inl"
