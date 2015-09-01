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

WidgetBase::WidgetBase()
{
}

WidgetBase::WidgetBase(const Vector2& position) :
    _localPosition(position)
{
    updateBounds();
}

WidgetBase::WidgetBase(const Vector2& position, const Vector2& dimensions) :
    _localPosition(position),
    _dimensions(dimensions)
{
    updateBounds();
}

void WidgetBase::tick(double timeStep)
{
    (void)timeStep;
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

const std::string& WidgetBase::tooltip() const
{
    return _tooltip;
}

void WidgetBase::setTooltip(const std::string& tooltip)
{
    _tooltip = tooltip;
}

bool WidgetBase::visible() const
{
    return _visible;
}

void WidgetBase::setVisible(bool visible)
{
    _visible = visible;
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

void WidgetBase::renderChildren(VectorRenderer::Frame& frame, const Rectangle& bounds)
{
    for (const WidgetBase::Handle& child : _children)
    {
        child->render(frame, bounds);
    }
}

void WidgetBase::setInterfaceSystem(InterfaceSystem& interfaceSystem)
{
    _interfaceSystem = &interfaceSystem;
}

void WidgetBase::updateBounds()
{
    // Update the global position
    _globalPosition = _localPosition;
    if (_parent)
    {
        _globalPosition += _parent->globalPosition();
    }

    // Compute the bounds
    _bounds = Rectangle(_globalPosition, _globalPosition + _dimensions);

    // Recursively update the bounds for each child
    for (const WidgetBase::Handle& child : _children)
    {
        child->updateBounds();
    }
}
