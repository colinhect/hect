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
    _position(position)
{
}

WidgetBase::WidgetBase(const Vector2& position, const Vector2& dimensions) :
    _position(position),
    _dimensions(dimensions)
{
}

void WidgetBase::tick(double timeStep)
{
    (void)timeStep;
}

const Vector2& WidgetBase::position() const
{
    return _position;
}

void WidgetBase::setPosition(const Vector2& position)
{
    _position = position;
}

const Vector2& WidgetBase::dimensions() const
{
    return _dimensions;
}

void WidgetBase::setDimensions(const Vector2& dimensions)
{
    _dimensions = dimensions;
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
