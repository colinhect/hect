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
#include "Grid.h"

#include "Hect/Scene/Systems/InterfaceSystem.h"

using namespace hect;

Grid::Cell::Cell(InterfaceSystem& interfaceSystem) :
    Widget(interfaceSystem)
{
}

void Grid::Cell::render(VectorRenderer::Frame& frame, const Rectangle& clipping)
{
    VectorRenderer::FrameStateScope scope(frame);

    //frame.setClipping(clipping);
    frame.translate(position());

    WidgetBase::render(frame, clipping);
}

Grid::Grid(InterfaceSystem& interfaceSystem) :
    Widget(interfaceSystem)
{
}

Grid::ColumnId Grid::addColumn(double width)
{
    ColumnId id = static_cast<ColumnId>(_columns.size());
    _columns.push_back(Column { width });
    updateDimensions();
    return id;
}

void Grid::resizeColumn(ColumnId columnId, double width)
{
    if (columnId >= _columns.size())
    {
        throw InvalidOperation("Column id out of range");
    }

    _columns[columnId] = Column { width };
    updateDimensions();
}

Grid::RowId Grid::addRow(double height)
{
    RowId id = static_cast<RowId>(_rows.size());
    _rows.push_back(Row { height });
    updateDimensions();
    return id;
}

void Grid::resizeRow(RowId rowId, double height)
{
    if (rowId >= _rows.size())
    {
        throw InvalidOperation("Row id out of range");
    }

    _rows[rowId] = Row { height };
    updateDimensions();
}

Grid::Cell::Handle Grid::createCell(ColumnId columnId, RowId rowId)
{
    if (columnId >= _columns.size())
    {
        throw InvalidOperation("Column id out of range");
    }
    else if (rowId >= _rows.size())
    {
        throw InvalidOperation("Row id out of range");
    }

    // Compute the position of the cell
    Vector2 position;
    for (ColumnId id = 0; id < columnId; ++id)
    {
        position.x += _columns[id].width;
    }
    for (RowId id = 0; id < rowId; ++id)
    {
        position.y += _rows[id].height;
    }

    // Compute the dimensions of the cell
    Vector2 dimensions;
    dimensions.x = _columns[columnId].width;
    dimensions.y = _rows[rowId].height;

    Cell::Handle cellWidget = createChild<Cell>();
    cellWidget->setPosition(position);
    cellWidget->setDimensions(dimensions);

    _cells[columnId][rowId] = cellWidget;

    return cellWidget;
}

void Grid::render(VectorRenderer::Frame& frame, const Rectangle& clipping)
{
    VectorRenderer::FrameStateScope scope(frame);

    //frame.setClipping(clipping);
    frame.translate(position());

    WidgetBase::render(frame, clipping);
}

void Grid::updateDimensions()
{
    Vector2 dimensions;
    for (const Column& column : _columns)
    {
        dimensions.x += column.width;
    }

    for (const Row& row : _rows)
    {
        dimensions.y += row.height;
    }

    setDimensions(dimensions);
}
