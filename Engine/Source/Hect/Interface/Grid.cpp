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
#include "Grid.h"

#include "Hect/Logic/Systems/InterfaceSystem.h"

using namespace hect;

Grid::Grid()
{
}

Grid::ColumnId Grid::addColumn(double width)
{
    ColumnId id = static_cast<ColumnId>(_columns.size());
    _columns.push_back(width);
    updateDimensions();
    return id;
}

Grid::RowId Grid::addRow(double height)
{
    RowId id = static_cast<RowId>(_rows.size());
    _rows.push_back(height);
    updateDimensions();
    return id;
}

void Grid::setCell(ColumnId columnId, RowId rowId, const WidgetBase::Handle& widget)
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
        position.x += _columns[id];
    }
    for (RowId id = 0; id < rowId; ++id)
    {
        position.y += _rows[id];
    }

    // Compute the dimenions of the cell
    Vector2 dimensions;
    dimensions.x = _columns[columnId];
    dimensions.y = _rows[columnId];

    WidgetBase::Handle cellWidget = interfaceSystem().add<WidgetBase>(position, dimensions);
    cellWidget->addChild(widget);

    addChild(cellWidget);

    _cells[columnId][rowId] = cellWidget;
}

void Grid::updateDimensions()
{
    Vector2 dimensions;
    for (double width : _columns)
    {
        dimensions.x += width;
    }

    for (double height : _rows)
    {
        dimensions.y += height;
    }

    setDimensions(dimensions);
}
