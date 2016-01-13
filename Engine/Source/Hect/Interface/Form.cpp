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
#include "Form.h"

#include "Hect/Logic/Systems/InterfaceSystem.h"

using namespace hect;

Form::Form(InterfaceSystem& interfaceSystem, Renderer& renderer, VectorRenderer& vectorRenderer) :
    _interfaceSystem(interfaceSystem),
    _renderer(renderer),
    _vectorRenderer(vectorRenderer)
{
    // Set the default style colors
    setStyleColor(StyleColor::Background, Color(0.15, 0.15, 0.15, 0.9));
    setStyleColor(StyleColor::BackgroundSelected, Color(0.0, 122.0 / 255.0, 204.0 / 255.0, 0.9));
    setStyleColor(StyleColor::BackgroundPressed, Color(0.15, 0.15, 0.15, 0.9));
    setStyleColor(StyleColor::BackgroundMouseOver, Color(0.0, 122.0 / 255.0, 204.0 / 255.0, 0.9));
    setStyleColor(StyleColor::Foreground, Color(1.0, 1.0, 1.0));
    setStyleColor(StyleColor::ForegroundSelected, Color(1.0, 1.0, 1.0));
    setStyleColor(StyleColor::ForegroundPressed, Color(1.0, 1.0, 1.0));
    setStyleColor(StyleColor::ForegroundMouseOver, Color(1.0, 1.0, 1.0));
    setStyleColor(StyleColor::Border, Color(0.5, 0.5, 0.5));
    setStyleColor(StyleColor::BorderPressed, Color(0.5, 0.5, 0.5));
    setStyleColor(StyleColor::BorderMouseOver, Color(0.5, 0.5, 0.5));
}

void Form::destroyChild(const WidgetBase::Handle& widget)
{
    // If the widget is not already marked for destruction
    auto it = std::find(_destroyedWidgets.begin(), _destroyedWidgets.end(), widget);
    if (it == _destroyedWidgets.end())
    {
        _destroyedWidgets.push_back(widget);

        // Recursively remove all descendant widgets
        for (const WidgetBase::Handle& childWidget : widget->_children)
        {
            destroyChild(childWidget);
        }
    }
}

const Color& Form::styleColor(StyleColor styleColor) const
{
    auto it = _styleColors.find(styleColor);
    if (it != _styleColors.end())
    {
        return it->second;
    }
    else
    {
        return Color::One;
    }
}

void Form::setStyleColor(StyleColor styleColor, const Color& color)
{
    _styleColors[styleColor] = color;
}

void Form::render(RenderTarget& target)
{
    Renderer::Frame frame = _renderer.beginFrame(target);
    VectorRenderer::Frame vectorFrame = _vectorRenderer.beginFrame(target);

    Rectangle clipping(0.0, 0.0, target.width(), target.height());
    for (const WidgetBase::Handle& widget : _widgets)
    {
        if (!widget->hasParent() && widget->isVisible())
        {
            Rectangle widgetClipping = widget->bounds().intersect(clipping);
            if (widgetClipping.size() != Vector2::Zero)
            {
                widget->render(vectorFrame, widgetClipping);
            }
        }
    }
}

void Form::tick(double timeStep)
{
    // Remove all widgets marked for destruction
    for (const WidgetBase::Handle& widget : _destroyedWidgets)
    {
        _widgets.erase(std::remove(_widgets.begin(), _widgets.end(), widget), _widgets.end());
    }
    _destroyedWidgets.clear();

    // Tick all parent widgets
    for (const WidgetBase::Handle& widget : _widgets)
    {
        if (!widget->hasParent() && widget->isVisible())
        {
            widget->tick(timeStep);
        }
    }
}

InterfaceSystem& Form::interfaceSystem()
{
    return _interfaceSystem;
}

const InterfaceSystem& Form::interfaceSystem() const
{
    return _interfaceSystem;
}

void Form::receiveEvent(const MouseEvent& event)
{
    if (event.type == MouseEventType::Movement)
    {
        for (const WidgetBase::Handle& widget : _widgets)
        {
            if (widget->globalBounds().contains(event.cursorPosition))
            {
                if (!widget->isMouseOver())
                {
                    widget->onMouseEnter();
                    widget->setMouseOver(true);
                }
            }
            else if (widget->isMouseOver())
            {
                widget->onMouseExit();
                widget->setMouseOver(false);
            }
        }
    }

    for (const WidgetBase::Handle& widget : _widgets)
    {
        if (!widget->hasParent())
        {
            if (widget->globalBounds().contains(event.cursorPosition))
            {
                widget->receiveEvent(event);
            }
        }
    }
}
