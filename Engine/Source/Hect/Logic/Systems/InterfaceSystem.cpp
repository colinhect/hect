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
#include "InterfaceSystem.h"

#include "Hect/Runtime/Engine.h"

using namespace hect;

InterfaceSystem::InterfaceSystem(Engine& engine, Scene& scene) :
    System(engine, scene, SystemTickStage::Subsequent),
    _mouse(engine.mouse()),
    _renderer(engine.renderer()),
    _vectorRenderer(engine.vectorRenderer())
{
    if (engine.hasMouse())
    {
        engine.mouse().registerListener(*this);
    }

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

Vector2 InterfaceSystem::measureTextDimensions(const std::string& text, const Font& font, double size) const
{
    return _vectorRenderer.measureTextDimensions(text, font, size);
}

const Color& InterfaceSystem::styleColor(StyleColor styleColor) const
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

void InterfaceSystem::setStyleColor(StyleColor styleColor, const Color& color)
{
    _styleColors[styleColor] = color;
}

void InterfaceSystem::render(RenderTarget& target)
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

void InterfaceSystem::tick(double timeStep)
{
    for (const WidgetBase::Handle& widget : _widgets)
    {
        if (!widget->hasParent() && widget->isVisible())
        {
            widget->tick(timeStep);
        }
    }
}

void InterfaceSystem::receiveEvent(const MouseEvent& event)
{
    if (_mouse.mode() == MouseMode::Cursor)
    {
        if (event.type == MouseEventType::Movement)
        {
            for (const WidgetBase::Handle& widget : _widgets)
            {
                if (widget->bounds().contains(event.cursorPosition))
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
                if (widget->bounds().contains(event.cursorPosition))
                {
                    widget->receiveEvent(event);
                }
            }
        }
    }
}
