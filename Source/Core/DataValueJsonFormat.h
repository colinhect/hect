///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2014 Colin Hill
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

namespace hect
{

///
/// Provides the functionality for parsing JSON into a data value.
class DataValueJsonFormat
{
public:

    ///
    /// Saves a data value to a JSON string.
    ///
    /// \param dataValue The data value.
    /// \param json The resulting JSON string.
    static void save(const DataValue& dataValue, std::string& json);

    ///
    /// Saves a data value to a stream as a JSON string.
    ///
    /// \param dataValue The data value.
    /// \param stream The stream to write to.
    static void save(const DataValue& dataValue, WriteStream& stream);

    ///
    /// Parses a JSON string into a data value.
    ///
    /// \param dataValue The data value to load to.
    /// \param json The JSON string.
    static void load(DataValue& dataValue, const std::string& json);

    ///
    /// Parses JSON from a stream into a data value.
    ///
    /// \param dataValue The data value to load to.
    /// \param stream The stream to read from.
    static void load(DataValue& dataValue, ReadStream& stream);
};

}