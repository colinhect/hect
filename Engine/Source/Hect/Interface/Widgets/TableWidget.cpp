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
#include "TableWidget.h"

#include "Hect/Scene/Systems/InterfaceSystem.h"

using namespace hect;

TableWidget::Cell::Cell(InterfaceSystem& interfaceSystem) :
    Widget(interfaceSystem)
{
}

void TableWidget::Cell::render(VectorRenderer::Frame& frame, Rectangle clipping)
{
    VectorRenderer::FrameStateScope scope(frame);

    //frame.setClipping(clipping);
    frame.translate(position());

    WidgetBase::render(frame, clipping);
}

TableWidget::TableWidget(InterfaceSystem& interfaceSystem) :
    Widget(interfaceSystem)
{
}

unsigned TableWidget::columnCount() const
{
    return static_cast<unsigned>(_columnWidths.size());
}

TableWidget::Column TableWidget::addColumn(double width)
{
    const Column column = static_cast<Column>(_columnWidths.size());
    _columnWidths.push_back(width);
    updateDimensions();
    return column;
}

void TableWidget::resizeColumn(Column column, double width)
{
    checkColumn(column);
    _columnWidths[column] = width;
    updateDimensions();
}

unsigned TableWidget::rowCount() const
{
    return static_cast<unsigned>(_rowHeights.size());
}

TableWidget::Row TableWidget::addRow(double height)
{
    const Row row = static_cast<Row>(_rowHeights.size());
    _rowHeights.push_back(height);
    updateDimensions();
    return row;
}

void TableWidget::resizeRow(Row row, double height)
{
    checkRow(row);
    _rowHeights[row] = height;
    updateDimensions();
}

TableWidget::Cell::Handle TableWidget::createCell(Column column, Row row)
{
    checkColumn(column);
    checkRow(row);

    // Compute the position of the cell
    Vector2 position;
    for (Column index = 0; index < column; ++index)
    {
        position.x += _columnWidths[index];
    }
    for (Row index = 0; index < row; ++index)
    {
        position.y += _rowHeights[index];
    }

    // Compute the dimensions of the cell
    Vector2 dimensions;
    dimensions.x = _columnWidths[column];
    dimensions.y = _rowHeights[row];

    // Create the cell
    Cell::Handle cellWidget = createChild<Cell>();
    cellWidget->setPosition(position);
    cellWidget->setDimensions(dimensions);

    _cells[column][row] = cellWidget;

    return cellWidget;
}

void TableWidget::render(VectorRenderer::Frame& frame, Rectangle clipping)
{
    VectorRenderer::FrameStateScope scope(frame);

    //frame.setClipping(clipping);
    frame.translate(position());

    WidgetBase::render(frame, clipping);
}

void TableWidget::updateDimensions()
{
    Vector2 dimensions;
    for (double width : _columnWidths)
    {
        dimensions.x += width;
    }

    for (double height : _rowHeights)
    {
        dimensions.y += height;
    }

    setDimensions(dimensions);
}

void TableWidget::checkColumn(Column column) const
{
    if (column >= _columnWidths.size())
    {
        throw InvalidOperation("Column is out of range");
    }
}

void TableWidget::checkRow(Row row) const
{
    if (row >= _rowHeights.size())
    {
        throw InvalidOperation("Row is out of range");
    }
}