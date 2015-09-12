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
#include "ListView.h"

#include "Hect/Logic/Systems/InterfaceSystem.h"

using namespace hect;

ListView::ListView(InterfaceSystem& interfaceSystem) :
    Widget(interfaceSystem)
{
}

ListViewSelectMode ListView::selectMode() const
{
    return _selectMode;
}

void ListView::setSelectMode(ListViewSelectMode selectMode)
{
    _selectMode = selectMode;
}

size_t ListView::addItem(const WidgetBase::Handle& widget)
{
    auto item = interfaceSystem().add<Item>();
    item->setDimensions(widget->dimensions());
    item->addChild(widget);
    addChild(item);

    size_t index = _items.size();
    _items.push_back(item);
    updateBounds();
    return index;
}

void ListView::render(VectorRenderer::Frame& frame, const Rectangle& bounds)
{
    StyleColor backgroundStyleColor = StyleColor::Background;

    frame.pushState();
    frame.setClipping(bounds);
    frame.beginPath();
    frame.rectangle(globalPosition(), dimensions());
    frame.setFillColor(styleColor(backgroundStyleColor));
    frame.fill();
    frame.popState();

    WidgetBase::render(frame, bounds);
}

void ListView::updateBounds()
{
    updateItems();
    WidgetBase::updateBounds();
}

void ListView::updateItems()
{
    // Compute the full dimensions
    _dimensions = Vector2();
    for (const WidgetBase::Handle& widget : _items)
    {
        const Vector2& widgetDimensions = widget->dimensions();
        _dimensions.x = std::max(_dimensions.x, widgetDimensions.x);
        _dimensions.y += widgetDimensions.y;
    }

    Vector2 localPosition = 0;
    for (const WidgetBase::Handle& widget : _items)
    {
        widget->setLocalPosition(localPosition);
        localPosition.y += widget->dimensions().y;
    }
}

ListView::Item::Item(InterfaceSystem& interfaceSystem) :
    Widget(interfaceSystem)
{
}

void ListView::Item::render(VectorRenderer::Frame& frame, const Rectangle& bounds)
{
    StyleColor backgroundStyleColor = StyleColor::Background;

    if (isMouseOver())
    {
        backgroundStyleColor = StyleColor::BackgroundMouseOver;
    }
    else if (_selected)
    {
        backgroundStyleColor = StyleColor::BackgroundSelected;
    }

    frame.pushState();
    frame.setClipping(bounds);
    frame.beginPath();
    frame.rectangle(globalPosition(), dimensions());
    frame.setFillColor(styleColor(backgroundStyleColor));
    frame.fill();
    frame.popState();

    WidgetBase::render(frame, bounds);
}

void ListView::Item::onPressed()
{
    ListView& listView = *reinterpret_cast<ListView*>(_parent);
    ListViewSelectMode selectMode = listView.selectMode();

    if (selectMode != ListViewSelectMode::None)
    {
        _selected = !_selected;

        // Deselect all other items if single select mode is set
        if (_selected && selectMode == ListViewSelectMode::Single)
        {
            for (const Item::Handle& item : listView._items)
            {
                if (item.get() != this)
                {
                    item->_selected = false;
                }
            }
        }
    }
}
