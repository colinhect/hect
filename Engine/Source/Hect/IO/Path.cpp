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
#include "Path.h"

#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"

using namespace hect;

namespace
{

const char _path_delimiter = '/';

}

Path::Path()
{
}

Path::Path(const char* path)
{
    set_raw_path(path);
}

Path::Path(const std::string& path)
{
    set_raw_path(path.data());
}

Path::Path(Name path)
{
    set_raw_path(path.data());
}

std::string Path::extension() const
{
    // Find the position of the first '.' starting from the right
    size_t i = _raw_path.size();
    for (; i > 0; --i)
    {
        if (_raw_path[i] == '.')
        {
            break;
        }
    }

    // Never found a '.' so there is no file extension
    if (i == 0)
    {
        return "";
    }

    // Return the extension
    return _raw_path.substr(i + 1, _raw_path.size() - i - 1);
}

Path Path::parent_directory() const
{
    // Find the position of the first '/' starting from the right
    size_t i = _raw_path.size();
    for (; i > 0; --i)
    {
        if (_raw_path[i] == _path_delimiter)
        {
            break;
        }
    }

    // Never found a '/' so there is no parent directory
    if (i == 0)
    {
        return Path();
    }

    // Return the parent directory
    return _raw_path.substr(0, i);
}

bool Path::empty() const
{
    return _raw_path.empty();
}

Path Path::operator+(const Path& path) const
{
    Path result(*this);

    // Only add if the right-hand side is not empty
    if (!path._raw_path.empty())
    {
        // Add a delimiter between the paths if the left-hand side is not
        // empty and the right-hand side does not start with a delimeter
        if (!result._raw_path.empty() && path._raw_path[0] != _path_delimiter)
        {
            result._raw_path += _path_delimiter;
        }
        result._raw_path.append(path._raw_path);
    }
    return result;
}

Path& Path::operator+=(const Path& path)
{
    // Only add if the right-hand side is not empty
    if (!path._raw_path.empty())
    {

        // Don't add the delimiter if this path is empty
        if (!_raw_path.empty())
        {
            _raw_path += _path_delimiter;
        }
        _raw_path.append(path._raw_path);
    }
    return *this;
}

const std::string& Path::as_string() const
{
    return _raw_path;
}

Name Path::as_name() const
{
    return Name(_raw_path);
}

bool Path::operator<(const Path& path) const
{
    return _raw_path < path._raw_path;
}

bool Path::operator==(const Path& path) const
{
    return _raw_path == path._raw_path;
}

bool Path::operator!=(const Path& path) const
{
    return _raw_path != path._raw_path;
}

void Path::set_raw_path(const char* raw_path)
{
    _raw_path = std::string(raw_path);
    std::string delimiter(" /");

    // Trim trailing delimiter
    size_t end = _raw_path.find_last_not_of(delimiter);
    _raw_path = _raw_path.substr(0, end + 1);
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const Path& path)
{
    encoder << encode_value(path.as_string());
    return encoder;
}

Decoder& operator>>(Decoder& decoder, Path& path)
{
    std::string raw_path;
    decoder >> decode_value(raw_path);
    path = Path(raw_path);
    return decoder;
}

}
