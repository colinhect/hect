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
#pragma once

#include "Hect/Core/Export.h"
#include "Hect/Interface/Widget.h"

namespace hect
{

///
/// A grid.
class HECT_EXPORT Grid :
    public Widget<Grid>
{
public:

    ///
    /// A numeric identifier to a column in grid.
    typedef size_t ColumnId;

    ///
    /// A numeric identifier to a row in grid.
    typedef size_t RowId;

    ///
    /// Constructs an empty grid widget.
    ///
    /// \param interfaceSystem The interface system.
    Grid(InterfaceSystem& interfaceSystem);

    ///
    /// Adds a column to the grid.
    ///
    /// \param width The width of the column.
    ///
    /// \returns The new column id.
    ColumnId addColumn(double width = 0.0);

    ///
    /// Resizes the width of a column.
    ///
    /// \param columnId The id of the column to resize.
    /// \param width The new width of the column
    ///
    /// \throws InvalidOperation If the column id is out of range.
    void resizeColumn(ColumnId columnId, double width);

    ///
    /// Adds a row to the grid.
    ///
    /// \param height The height of the row.
    ///
    /// \returns The new row id.
    RowId addRow(double height = 0.0);

    ///
    /// Resizes height width of a row.
    ///
    /// \param rowId The id of the row to resize.
    /// \param height The new height of the column
    ///
    /// \throws InvalidOperation If the row id is out of range.
    void resizeRow(RowId rowId, double height);

    ///
    /// Sets the widget that exists in the specified cell.
    ///
    /// \param columnId The id of the column.
    /// \param rowId The id of the row.
    /// \param widget The widget.
    ///
    /// \throws InvalidOperation If the row id or column id is out of range.
    void setCell(ColumnId columnId, RowId rowId, const WidgetBase::Handle& widget);

private:
    void updateDimensions();

    std::map<ColumnId, std::map<RowId, WidgetBase::Handle>> _cells;

    std::vector<double> _columns;
    std::vector<double> _rows;
};

}
