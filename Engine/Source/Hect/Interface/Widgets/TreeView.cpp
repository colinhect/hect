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
#include "TreeView.h"

#include "Hect/Logic/Systems/InterfaceSystem.h"

using namespace hect;

TreeView::TreeView(InterfaceSystem& interfaceSystem) :
    Widget(interfaceSystem)
{
}

TreeView::NodeId TreeView::addNode(const WidgetBase::Handle& widget)
{
    auto node = interfaceSystem().add<Node>();
    node->setDimensions(widget->dimensions());
    node->addChild(widget);
    addChild(node);

    NodeId nodeId = _nextNodeId++;
    _nodes[nodeId] = node;

    _rootNodes.push_back(nodeId);

    updateBounds();

    return nodeId;
}

void TreeView::render(VectorRenderer::Frame& frame, const Rectangle& bounds)
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

void TreeView::updateBounds()
{
    updateItems();
    WidgetBase::updateBounds();
}

void TreeView::updateItems()
{
    // Compute the full dimensions
    Vector2 dimensions = Vector2();
    for (NodeId nodeId : _rootNodes)
    {
        const Vector2& widgetDimensions = _nodes[nodeId]->dimensions();
        dimensions.x = std::max(dimensions.x, widgetDimensions.x);
        dimensions.y += widgetDimensions.y;
    }
    modifyDimensions(dimensions);

    Vector2 position;
    for (NodeId nodeId : _rootNodes)
    {
        _nodes[nodeId]->setPosition(position);
        position.y += _nodes[nodeId]->dimensions().y;
    }
}

TreeView::Node::Node(InterfaceSystem& interfaceSystem) :
    Widget(interfaceSystem)
{
}

void TreeView::Node::render(VectorRenderer::Frame& frame, const Rectangle& bounds)
{
    StyleColor backgroundStyleColor = StyleColor::Background;

    if (isMouseOver())
    {
        backgroundStyleColor = StyleColor::BackgroundMouseOver;
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

void TreeView::Node::onPressed()
{
}
