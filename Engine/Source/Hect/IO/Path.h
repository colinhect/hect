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

#include <string>
#include <iostream>

#include "Hect/Core/Export.h"
#include "Hect/Core/Name.h"

namespace hect
{

///
/// A path to a file or directory.
class HECT_EXPORT Path
{
public:

    ///
    /// Constructs an empty path.
    Path();

    ///
    /// Constructs a path from a string.
    ///
    /// \note %Any trailing slashes are ignored.
    ///
    /// \param path The path with each part delimited by a forward slash.
    Path(const char* path);

    ///
    /// Constructs a path from a string.
    ///
    /// \note %Any trailing slashes are ignored.
    ///
    /// \param path The path with each part delimited by a forward slash.
    Path(const std::string& path);

    ///
    /// Constructs a path from a name.
    ///
    /// \note %Any trailing slashes are ignored.
    ///
    /// \param path The path with each part delimited by a forward slash.
    Path(Name path);

    ///
    /// Returns the file extension of the path.
    std::string extension() const;

    ///
    /// Returns the path to the parent directory of the current path.
    Path parent_directory() const;

    ///
    /// Returns whether the path is empty.
    bool empty() const;

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
    const std::string& as_string() const;

    ///
    /// Returns the raw path.
    Name as_name() const;

    ///
    /// Returns true if the path is less than another.
    ///
    /// \param path The other path.
    bool operator<(const Path& path) const;

    ///
    /// Returns whether the path is equivalent to another.
    ///
    /// \param path The other path.
    bool operator==(const Path& path) const;

    ///
    /// Returns whether the path is different from another.
    ///
    /// \param path The other path.
    bool operator!=(const Path& path) const;

private:
    void set_raw_path(const char* raw_path);

    std::string _raw_path;
};

class Encoder;
class Decoder;

HECT_EXPORT Encoder& operator<<(Encoder& encoder, const Path& path);
HECT_EXPORT Decoder& operator>>(Decoder& decoder, Path& path);

}
