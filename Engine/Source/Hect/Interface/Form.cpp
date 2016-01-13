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

Form::Form(InterfaceSystem& interfaceSystem, RenderTarget& renderTarget) :
    Widget(interfaceSystem),
    _renderTarget(renderTarget)
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

RenderTarget& Form::renderTarget()
{
    return _renderTarget;
}
