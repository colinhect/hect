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

#include "Hect/Logic/Systems/InterfaceSystem.h"

using namespace hect;

WidgetBase::WidgetBase(InterfaceSystem& interfaceSystem) :
    _interfaceSystem(&interfaceSystem)
{
}

void WidgetBase::tick(double timeStep)
{
    if (isLayoutDirty())
    {
        updateLayout();
    }

    for (const WidgetBase::Handle& child : _children)
    {
        child->tick(timeStep);
    }
}

void WidgetBase::render(VectorRenderer::Frame& frame, const Rectangle& clipping)
{
    for (const WidgetBase::Handle& child : _children)
    {
        if (child->isVisible())
        {
            Rectangle childClipping = child->globalBounds().intersect(clipping);
            if (childClipping.size() != Vector2::Zero)
            {
                child->render(frame, childClipping);
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
        if (child->globalBounds().contains(event.cursorPosition))
        {
            child->receiveEvent(event);
        }
    }
}

void WidgetBase::setPressAction(const WidgetBase::Action& action)
{
    _pressAction = action;
}

const Vector2& WidgetBase::position() const
{
    return _actualPosition;
}

void WidgetBase::setPosition(const Vector2& position)
{
    _assignedPosition = position.floor();
    markLayoutDirty();
}

Vector2 WidgetBase::globalPosition() const
{
    Vector2 globalPosition = _actualPosition;
    if (hasParent())
    {
        globalPosition += parent().globalPosition();
    }

    return globalPosition;
}

const Vector2& WidgetBase::dimensions() const
{
    return _dimensions;
}

void WidgetBase::setDimensions(const Vector2& dimensions)
{
    _dimensions = dimensions;
    markLayoutDirty();
}

Rectangle WidgetBase::bounds() const
{
    Vector2 origin = position();
    return Rectangle(origin, origin + dimensions());
}

Rectangle WidgetBase::globalBounds() const
{
    Vector2 origin = globalPosition();
    return Rectangle(origin, origin + dimensions());
}

HorizontalAlign WidgetBase::horizontalAlign() const
{
    return _horizontalAlign;
}

void WidgetBase::setHorizontalAlign(HorizontalAlign align)
{
    _horizontalAlign = align;
    markLayoutDirty();
}

VerticalAlign WidgetBase::verticalAlign() const
{
    return _verticalAlign;
}

void WidgetBase::setVerticalAlign(VerticalAlign align)
{
    _verticalAlign = align;
    markLayoutDirty();
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
    auto it = _styleColors.find(styleColor);
    if (it != _styleColors.end())
    {
        return it->second;
    }
    else if (_interfaceSystem)
    {
        return _interfaceSystem->styleColor(styleColor);
    }
    else
    {
        return Color::One;
    }
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
            markLayoutDirty();
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
        markLayoutDirty();
    }
}

bool WidgetBase::hasParent() const
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

void WidgetBase::updateLayout()
{
    _layoutDirty = false;

    // Update the global position
    _actualPosition = _assignedPosition;
    if (_parent)
    {
        // Align horizontally
        switch (_horizontalAlign)
        {
        case HorizontalAlign::None:
            break;
        case HorizontalAlign::Left:
            break;
        case HorizontalAlign::Center:
            _actualPosition.x = (_parent->_dimensions.x - _dimensions.x) * 0.5;
            break;
        case HorizontalAlign::Right:
            _actualPosition.x = (_parent->_dimensions.x - _dimensions.x);
            break;
        }

        // Align vertically
        switch (_verticalAlign)
        {
        case VerticalAlign::None:
            break;
        case VerticalAlign::Bottom:
            _actualPosition.y = (_parent->_dimensions.y - _dimensions.y);
            break;
        case VerticalAlign::Center:
            _actualPosition.y = (_parent->_dimensions.y - _dimensions.y) * 0.5;
            break;
        case VerticalAlign::Top:
            break;
        }
    }

    _actualPosition = _actualPosition.floor();

    // Recursively update the bounds for each child
    for (const WidgetBase::Handle& child : _children)
    {
        child->updateLayout();
    }
}

bool WidgetBase::isLayoutDirty() const
{
    return _layoutDirty;
}

void WidgetBase::markLayoutDirty()
{
    _layoutDirty = true;

    for (const WidgetBase::Handle& child : _children)
    {
        child->markLayoutDirty();
    }
}

void WidgetBase::setMouseOver(bool value)
{
    _mouseOver = value;
}
