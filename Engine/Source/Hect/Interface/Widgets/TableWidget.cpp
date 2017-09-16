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

TableWidget::Cell::Cell(InterfaceSystem& interface_system) :
    Widget(interface_system)
{
}

void TableWidget::Cell::render(VectorRenderer::Frame& frame, Rectangle clipping)
{
    VectorRenderer::FrameStateScope scope(frame);

    //frame.set_clipping(clipping);
    frame.translate(position());

    WidgetBase::render(frame, clipping);
}

TableWidget::TableWidget(InterfaceSystem& interface_system) :
    Widget(interface_system)
{
}

unsigned TableWidget::column_count() const
{
    return static_cast<unsigned>(_column_widths.size());
}

TableWidget::Column TableWidget::add_column(double width)
{
    const Column column = static_cast<Column>(_column_widths.size());
    _column_widths.push_back(width);
    update_dimensions();
    return column;
}

void TableWidget::resize_column(Column column, double width)
{
    check_column(column);
    _column_widths[column] = width;
    update_dimensions();
}

unsigned TableWidget::row_count() const
{
    return static_cast<unsigned>(_row_heights.size());
}

TableWidget::Row TableWidget::add_row(double height)
{
    const Row row = static_cast<Row>(_row_heights.size());
    _row_heights.push_back(height);
    update_dimensions();
    return row;
}

void TableWidget::resize_row(Row row, double height)
{
    check_row(row);
    _row_heights[row] = height;
    update_dimensions();
}

TableWidget::Cell::Handle TableWidget::create_cell(Column column, Row row)
{
    check_column(column);
    check_row(row);

    // Compute the position of the cell
    Vector2 position;
    for (Column index = 0; index < column; ++index)
    {
        position.x += _column_widths[index];
    }
    for (Row index = 0; index < row; ++index)
    {
        position.y += _row_heights[index];
    }

    // Compute the dimensions of the cell
    Vector2 dimensions;
    dimensions.x = _column_widths[column];
    dimensions.y = _row_heights[row];

    // Create the cell
    Cell::Handle cell_widget = create_child<Cell>();
    cell_widget->set_position(position);
    cell_widget->set_dimensions(dimensions);

    _cells[column][row] = cell_widget;

    return cell_widget;
}

void TableWidget::render(VectorRenderer::Frame& frame, Rectangle clipping)
{
    VectorRenderer::FrameStateScope scope(frame);

    //frame.set_clipping(clipping);
    frame.translate(position());

    WidgetBase::render(frame, clipping);
}

void TableWidget::update_dimensions()
{
    Vector2 dimensions;
    for (double width : _column_widths)
    {
        dimensions.x += width;
    }

    for (double height : _row_heights)
    {
        dimensions.y += height;
    }

    set_dimensions(dimensions);
}

void TableWidget::check_column(Column column) const
{
    if (column >= _column_widths.size())
    {
        throw InvalidOperation("Column is out of range");
    }
}

void TableWidget::check_row(Row row) const
{
    if (row >= _row_heights.size())
    {
        throw InvalidOperation("Row is out of range");
    }
}
