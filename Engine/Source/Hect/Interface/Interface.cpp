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
#include "Interface.h"

#include "Hect/Scene/Systems/InterfaceSystem.h"

using namespace hect;

Interface::Interface(InterfaceSystem& interface_system, RenderTarget& render_target) :
    Widget(interface_system),
    _render_target(render_target)
{
    // Set the default style colors
    set_style_color(StyleColor::Background, Color(0.15, 0.15, 0.15, 0.9));
    set_style_color(StyleColor::BackgroundSelected, Color(0.0, 122.0 / 255.0, 204.0 / 255.0, 0.9));
    set_style_color(StyleColor::BackgroundPressed, Color(0.15, 0.15, 0.15, 0.9));
    set_style_color(StyleColor::BackgroundMouseOver, Color(0.0, 122.0 / 255.0, 204.0 / 255.0, 0.9));
    set_style_color(StyleColor::Foreground, Color(1.0, 1.0, 1.0));
    set_style_color(StyleColor::ForegroundSelected, Color(1.0, 1.0, 1.0));
    set_style_color(StyleColor::ForegroundPressed, Color(1.0, 1.0, 1.0));
    set_style_color(StyleColor::ForegroundMouseOver, Color(1.0, 1.0, 1.0));
    set_style_color(StyleColor::Border, Color(0.5, 0.5, 0.5));
    set_style_color(StyleColor::BorderPressed, Color(0.5, 0.5, 0.5));
    set_style_color(StyleColor::BorderMouseOver, Color(0.5, 0.5, 0.5));

    // Set the dimensions of the interface to match the render target
    set_dimensions(render_target.dimensions());
}

RenderTarget& Interface::render_target()
{
    return _render_target;
}
