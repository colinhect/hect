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
/// A path to a file or directory.
class Path
{
public:

    ///
    /// Constructs an empty path.
    Path();

    ///
    /// Constructs a path from a string.  Any leading or trailing slashes
    /// are ignored.
    ///
    /// \param path The path with each part delimited by a forward slash.
    Path(const char* path);

    ///
    /// Constructs a path from a string.  Any leading or trailing slashes
    /// are ignored.
    ///
    /// \param path The path with each part delimited by a forward slash.
    Path(const std::string& path);

    ///
    /// Returns the file extension of the path.
    std::string extension() const;

    ///
    /// Returns the concatenation of the path and another path.
    ///
    /// \param path The path to concatenate.
    Path operator+(const Path& path) const;

    ///
    /// Appends another path to the end of the path.
    ///
    /// \param path The path to append.
    Path& operator+=(const Path& path);

    ///
    /// Returns the raw path.
    const std::string& toString() const;

    ///
    /// Returns true if the path is less than the given path.
    bool operator<(const Path& path) const;

private:
    void _setRawPath(const char* rawPath);

    std::string _rawPath;
};

///
/// Outputs a path to a stream.
///
/// \param os the output stream.
/// \param path the path to output.
std::ostream& operator<<(std::ostream& os, const Path& path);

}