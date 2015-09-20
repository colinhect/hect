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
#include "ScrollPanel.h"

#include "Hect/Logic/Systems/InterfaceSystem.h"

using namespace hect;

ScrollPanel::ScrollPanel(InterfaceSystem& interfaceSystem) :
    Widget(interfaceSystem),
    _grid(interfaceSystem.add<Grid>())
{
    _grid->addColumn();
    _grid->addColumn(30);
    _grid->addRow();
    _grid->addRow(30);
}

void ScrollPanel::addChild(const WidgetBase::Handle& child)
{
    WidgetBase::addChild(child);
}

void ScrollPanel::removeChild(const WidgetBase::Handle& child)
{
    WidgetBase::removeChild(child);
}

void ScrollPanel::updateLayout()
{
    WidgetBase::updateLayout();
}
