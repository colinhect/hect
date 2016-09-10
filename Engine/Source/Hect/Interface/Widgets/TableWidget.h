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
#pragma once

#include "Hect/Core/Export.h"
#include "Hect/Interface/Widget.h"

namespace hect
{

///
/// A table.
class HECT_EXPORT TableWidget :
    public Widget<TableWidget>
{
public:

    ///
    /// The index of a column in a TableWidget.
    typedef unsigned Column;

    ///
    /// The index of a row in a TableWidget.
    typedef unsigned Row;

    ///
    /// A cell in a TableWidget.
    class Cell :
        public Widget<Cell>
    {
    public:
        Cell(InterfaceSystem& interfaceSystem);

        void render(VectorRenderer::Frame& frame, Rectangle clipping) override;
    };

    ///
    /// Constructs an empty table widget.
    ///
    /// \param interfaceSystem The interface system.
    TableWidget(InterfaceSystem& interfaceSystem);

    ///
    /// Returns the number of columns.
    unsigned columnCount() const;

    ///
    /// Adds a new column.
    ///
    /// \param width The width of the column.
    ///
    /// \returns The column that was added.
    Column addColumn(double width);

    ///
    /// Sets the width of the specified column.
    ///
    /// \param column The column.
    /// \param width The new width of the column.
    ///
    /// \throws InvalidOperation If the column is out of range.
    void resizeColumn(Column column, double width);

    ///
    /// Returns the number of rows.
    unsigned rowCount() const;

    ///
    /// Adds a new row.
    ///
    /// \param height The height of the row.
    ///
    /// \returns The row that was added.
    Column addRow(double height);

    ///
    /// Sets the row of the specified row.
    ///
    /// \param row The row.
    /// \param height The new height of the row.
    ///
    /// \throws InvalidOperation If the row is out of range.
    void resizeRow(Row row, double height);

    ///
    /// Creates a cell widget at the specified coordinates.
    ///
    /// \param column The column.
    /// \param row The row.
    ///
    /// \throws InvalidOperation If the row or column is out of range or a
    /// cell widget already exists in the Row coordinates.
    Cell::Handle createCell(Column column, Row row);

    // Widget overrides
    void render(VectorRenderer::Frame& frame, Rectangle clipping) override;

private:
    void updateDimensions();
    void checkColumn(Column column) const;
    void checkRow(Row row) const;

    std::map<Column, std::map<Row, WidgetBase::Handle>> _cells;
    std::vector<double> _columnWidths;
    std::vector<double> _rowHeights;
};

}
