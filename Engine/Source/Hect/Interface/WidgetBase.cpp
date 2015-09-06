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
#include "WidgetBase.h"

using namespace hect;

WidgetBase::WidgetBase(InterfaceSystem& interfaceSystem) :
    _interfaceSystem(&interfaceSystem)
{
    useDefaultStyleColors();
}

WidgetBase::WidgetBase(InterfaceSystem& interfaceSystem, const Vector2& position) :
    _interfaceSystem(&interfaceSystem),
    _localPosition(position)
{
    updateBounds();
    useDefaultStyleColors();
}

WidgetBase::WidgetBase(InterfaceSystem& interfaceSystem, const Vector2& position, const Vector2& dimensions) :
    _interfaceSystem(&interfaceSystem),
    _localPosition(position),
    _dimensions(dimensions)
{
    updateBounds();
    useDefaultStyleColors();
}

void WidgetBase::tick(double timeStep)
{
    for (const WidgetBase::Handle& child : _children)
    {
        child->tick(timeStep);
    }
}

void WidgetBase::render(VectorRenderer::Frame& frame, const Rectangle& bounds)
{
    for (const WidgetBase::Handle& child : _children)
    {
        if (child->isVisible())
        {
            Rectangle childBounds = child->bounds().intersect(bounds);
            if (childBounds.size() != Vector2::Zero)
            {
                child->render(frame, childBounds);
            }
        }
    }
}

void WidgetBase::onMouseEnter()
{
}

void WidgetBase::onMouseExit()
{
    _pressed = false;
}

void WidgetBase::onPressed()
{
}

void WidgetBase::receiveEvent(const MouseEvent& event)
{
    if (event.button == MouseButton::Button0)
    {
        if (event.type == MouseEventType::ButtonDown)
        {
            _pressed = true;
        }
        else if (_pressed && event.type == MouseEventType::ButtonUp)
        {
            onPressed();

            if (_pressAction)
            {
                _pressAction();
                _pressed = false;
            }
        }
    }

    for (const WidgetBase::Handle& child : _children)
    {
        if (child->bounds().contains(event.cursorPosition))
        {
            child->receiveEvent(event);
        }
    }
}

void WidgetBase::setPressAction(const WidgetBase::Action& action)
{
    _pressAction = action;
}

const Vector2& WidgetBase::localPosition() const
{
    return _localPosition;
}

void WidgetBase::setLocalPosition(const Vector2& position)
{
    _localPosition = position;
    updateBounds();
}

const Vector2& WidgetBase::globalPosition() const
{
    return _globalPosition;
}

const Vector2& WidgetBase::dimensions() const
{
    return _dimensions;
}

void WidgetBase::setDimensions(const Vector2& dimensions)
{
    _dimensions = dimensions;
    updateBounds();
}

const Rectangle& WidgetBase::bounds() const
{
    return _bounds;
}

void WidgetBase::setAlignment(HorizontalAlign horizontalAlign, VerticalAlign verticalAlign)
{
    _horizontalAlign = horizontalAlign;
    _verticalAlign = verticalAlign;
    updateBounds();
}

HorizontalAlign WidgetBase::horizontalAlign() const
{
    return _horizontalAlign;
}

VerticalAlign WidgetBase::verticalAlign() const
{
    return _verticalAlign;
}

const std::string& WidgetBase::tooltip() const
{
    return _tooltip;
}

void WidgetBase::setTooltip(const std::string& tooltip)
{
    _tooltip = tooltip;
}

bool WidgetBase::isVisible() const
{
    return _visible;
}

void WidgetBase::setVisible(bool visible)
{
    _visible = visible;
}

const Color& WidgetBase::styleColor(StyleColor styleColor) const
{
    return _styleColors[styleColor];
}

void WidgetBase::setStyleColor(StyleColor styleColor, const Color& color)
{
    _styleColors[styleColor] = color;
}

bool WidgetBase::isMouseOver() const
{
    return _mouseOver;
}

bool WidgetBase::isPressed() const
{
    return _pressed;
}

void WidgetBase::addChild(const WidgetBase::Handle& child)
{
    if (child)
    {
        if (child->_parent)
        {
            throw InvalidOperation("Widget is already a child");
        }
        else
        {
            _children.push_back(child);
            child->_parent = this;
            child->updateBounds();
        }
    }
}

void WidgetBase::removeChild(const WidgetBase::Handle& child)
{
    auto it = std::find(_children.begin(), _children.end(), child);
    if (it == _children.end())
    {
        throw InvalidOperation("Widget is not a child of this widget");
    }
    else
    {
        _children.erase(it);
        child->_parent = nullptr;
        child->updateBounds();
    }
}

bool WidgetBase::hasParent() const
{
    return _parent != nullptr;
}

InterfaceSystem& WidgetBase::interfaceSystem()
{
    if (!_interfaceSystem)
    {
        throw InvalidOperation("Widget does not belong to an interface system");
    }

    return *_interfaceSystem;
}

const InterfaceSystem& WidgetBase::interfaceSystem() const
{
    if (!_interfaceSystem)
    {
        throw InvalidOperation("Widget does not belong to an interface system");
    }

    return *_interfaceSystem;
}

void WidgetBase::updateBounds()
{
    // Update the global position
    _globalPosition = _localPosition;
    if (_parent)
    {
        _globalPosition += _parent->globalPosition();

        // Align horizontally
        switch (_horizontalAlign)
        {
        case HorizontalAlign::None:
            break;
        case HorizontalAlign::Left:
            break;
        case HorizontalAlign::Center:
            _globalPosition.x += (_parent->_dimensions.x - _dimensions.x) * 0.5;
            break;
        case HorizontalAlign::Right:
            _globalPosition.x += (_parent->_dimensions.x - _dimensions.x);
            break;
        }

        // Align vertically
        switch (_verticalAlign)
        {
        case VerticalAlign::None:
            break;
        case VerticalAlign::Bottom:
            _globalPosition.y += (_parent->_dimensions.y - _dimensions.y);
            break;
        case VerticalAlign::Center:
            _globalPosition.y += (_parent->_dimensions.y - _dimensions.y) * 0.5;
            break;
        case VerticalAlign::Top:
            break;
        }
    }

    // Compute the bounds
    _bounds = Rectangle(_globalPosition, _globalPosition + _dimensions);

    // Recursively update the bounds for each child
    for (const WidgetBase::Handle& child : _children)
    {
        child->updateBounds();
    }
}

void WidgetBase::setMouseOver(bool value)
{
    _mouseOver = value;
}

void WidgetBase::useDefaultStyleColors()
{
    setStyleColor(StyleColor::Background, Color(0.15, 0.15, 0.15, 0.9));
    setStyleColor(StyleColor::BackgroundPressed, Color(0.15, 0.15, 0.15, 0.9));
    setStyleColor(StyleColor::BackgroundMouseOver, Color(0.0, 122.0 / 255.0, 204.0 / 255.0, 0.9));
    setStyleColor(StyleColor::Foreground, Color(1.0, 1.0, 1.0));
    setStyleColor(StyleColor::ForegroundPressed, Color(1.0, 1.0, 1.0));
    setStyleColor(StyleColor::ForegroundMouseOver, Color(1.0, 1.0, 1.0));
    setStyleColor(StyleColor::Border, Color(0.5, 0.5, 0.5));
    setStyleColor(StyleColor::BorderPressed, Color(0.5, 0.5, 0.5));
    setStyleColor(StyleColor::BorderMouseOver, Color(0.5, 0.5, 0.5));
}