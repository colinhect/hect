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
#include "WidgetBase.h"

#include "Hect/Scene/Systems/InterfaceSystem.h"

using namespace hect;

WidgetBase::WidgetBase(InterfaceSystem& interface_system) :
    _interface_system(interface_system)
{
}

void WidgetBase::tick(Seconds time_step)
{
    for (const WidgetBase::Handle& child : _children_to_destroy)
    {
        _children.erase(std::remove(_children.begin(), _children.end(), child), _children.end());
    }

    if (is_layout_dirty())
    {
        update_layout();
    }

    for (const WidgetBase::Handle& child : _children)
    {
        child->tick(time_step);
    }
}

void WidgetBase::render(VectorRenderer::Frame& frame, Rectangle clipping)
{
    for (const WidgetBase::Handle& child : _children)
    {
        if (child->is_visible())
        {
            const Rectangle child_bounds = child->global_bounds();
            const Rectangle child_clipping = child_bounds.intersect(clipping);
            if (child_clipping.size() != Vector2::Zero)
            {
                child->render(frame, child_clipping);
            }
        }
    }
}

void WidgetBase::on_mouse_enter()
{
}

void WidgetBase::on_mouse_exit()
{
    _pressed = false;
}

void WidgetBase::on_pressed()
{
}

void WidgetBase::receive_event(const MouseEvent& event)
{
    if (event.type == MouseEventType::Movement)
    {
        if (global_bounds().contains(event.cursor_position))
        {
            if (!is_mouse_over())
            {
                on_mouse_enter();
                set_mouse_over(true);
            }
        }
        else if (is_mouse_over())
        {
            on_mouse_exit();
            set_mouse_over(false);
        }
    }

    if (event.button == MouseButton::Button0)
    {
        if (event.type == MouseEventType::ButtonDown)
        {
            _pressed = true;
        }
        else if (_pressed && event.type == MouseEventType::ButtonUp)
        {
            on_pressed();

            if (_press_action)
            {
                _press_action();
                _pressed = false;
            }
        }
    }

    for (const WidgetBase::Handle& child : _children)
    {
        if (child->global_bounds().contains(event.cursor_position))
        {
            child->receive_event(event);
        }
    }
}

void WidgetBase::set_press_action(const WidgetBase::Action& action)
{
    _press_action = action;
}

Vector2 WidgetBase::position() const
{
    return _actual_position;
}

void WidgetBase::set_position(Vector2 position)
{
    _assigned_position = position.floor();
    mark_layout_dirty();
}

Vector2 WidgetBase::global_position() const
{
    Vector2 global_position = _actual_position;
    if (has_parent())
    {
        global_position += parent().global_position();
    }

    return global_position;
}

Vector2 WidgetBase::dimensions() const
{
    return _dimensions;
}

void WidgetBase::set_dimensions(Vector2 dimensions)
{
    _dimensions = dimensions;
    mark_layout_dirty();
}

Rectangle WidgetBase::bounds() const
{
    Vector2 origin = position();
    return Rectangle(origin, origin + dimensions());
}

Rectangle WidgetBase::global_bounds() const
{
    Vector2 origin = global_position();
    return Rectangle(origin, origin + dimensions());
}

HorizontalAlign WidgetBase::horizontal_align() const
{
    return _horizontal_align;
}

void WidgetBase::set_horizontal_align(HorizontalAlign align)
{
    _horizontal_align = align;
    mark_layout_dirty();
}

VerticalAlign WidgetBase::vertical_align() const
{
    return _vertical_align;
}

void WidgetBase::set_vertical_align(VerticalAlign align)
{
    _vertical_align = align;
    mark_layout_dirty();
}

const std::string& WidgetBase::tooltip() const
{
    return _tooltip;
}

void WidgetBase::set_tooltip(const std::string& tooltip)
{
    _tooltip = tooltip;
}

bool WidgetBase::is_visible() const
{
    return _visible;
}

void WidgetBase::set_visible(bool visible)
{
    _visible = visible;
}

Color WidgetBase::style_color(StyleColor style_color) const
{
    auto it = _style_colors.find(style_color);
    if (it != _style_colors.end())
    {
        return it->second;
    }
    else if (_parent)
    {
        return _parent->style_color(style_color);
    }
    else
    {
        return Color::One;
    }
}

void WidgetBase::set_style_color(StyleColor style_color, Color color)
{
    _style_colors[style_color] = color;
}

bool WidgetBase::is_mouse_over() const
{
    return _mouse_over;
}

bool WidgetBase::is_pressed() const
{
    return _pressed;
}

void WidgetBase::destroy_child(const WidgetBase::Handle& child)
{
    auto it = std::find(_children.begin(), _children.end(), child);
    if (it == _children.end())
    {
        throw InvalidOperation("Widget is not a child of the widget");
    }

    _children_to_destroy.push_back(child);
}

bool WidgetBase::has_parent() const
{
    return _parent != nullptr;
}

WidgetBase& WidgetBase::parent()
{
    if (!_parent)
    {
        throw InvalidOperation("Widget does not have a parent");
    }

    return *_parent;
}

const WidgetBase& WidgetBase::parent() const
{
    if (!_parent)
    {
        throw InvalidOperation("Widget does not have a parent");
    }

    return *_parent;
}

InterfaceSystem& WidgetBase::interface_system()
{
    return _interface_system;
}

const InterfaceSystem& WidgetBase::interface_system() const
{
    return _interface_system;
}

void WidgetBase::update_layout()
{
    _layout_dirty = false;

    // Update the global position
    _actual_position = _assigned_position;
    if (_parent)
    {
        // Align horizontally
        switch (_horizontal_align)
        {
        case HorizontalAlign::None:
            break;
        case HorizontalAlign::Left:
            break;
        case HorizontalAlign::Center:
            _actual_position.x = (_parent->_dimensions.x - _dimensions.x) * 0.5;
            break;
        case HorizontalAlign::Right:
            _actual_position.x = (_parent->_dimensions.x - _dimensions.x);
            break;
        }

        // Align vertically
        switch (_vertical_align)
        {
        case VerticalAlign::None:
            break;
        case VerticalAlign::Bottom:
            _actual_position.y = (_parent->_dimensions.y - _dimensions.y);
            break;
        case VerticalAlign::Center:
            _actual_position.y = (_parent->_dimensions.y - _dimensions.y) * 0.5;
            break;
        case VerticalAlign::Top:
            break;
        }
    }

    _actual_position = _actual_position.floor();

    // Recursively update the bounds for each child
    for (const WidgetBase::Handle& child : _children)
    {
        child->update_layout();
    }
}

bool WidgetBase::is_layout_dirty() const
{
    return _layout_dirty;
}

void WidgetBase::mark_layout_dirty()
{
    _layout_dirty = true;

    for (const WidgetBase::Handle& child : _children)
    {
        child->mark_layout_dirty();
    }
}

void WidgetBase::set_mouse_over(bool value)
{
    _mouse_over = value;
}
