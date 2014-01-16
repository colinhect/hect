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
#include "Hect.h"

using namespace hect;

const char pathDelimiter = '/';

Path::Path() { }

Path::Path(const char* path)
{
    _setRawPath(path);
}

Path::Path(const std::string& path)
{
    _setRawPath(path.c_str());
}

std::string Path::extension() const
{

    // Find the position of the first '.' starting from the right
    size_t i = _rawPath.size();
    for (; i > 0; --i)
    {
        if (_rawPath[i] == '.')
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
    return _rawPath.substr(i + 1, _rawPath.size() - i - 1);
}

Path Path::operator+(const Path& path) const
{
    Path result(*this);

    // Only add if the right-hand side is not empty
    if (!path._rawPath.empty())
    {

        // Don't add the delimiter if the left-hand side is empty
        if (!result._rawPath.empty())
        {
            result._rawPath += pathDelimiter;
        }
        result._rawPath.append(path._rawPath);
    }
    return result;
}

Path& Path::operator+=(const Path& path)
{
    // Only add if the right-hand side is not empty
    if (!path._rawPath.empty())
    {

        // Don't add the delimiter if this path is empty
        if (!_rawPath.empty())
        {
            _rawPath += pathDelimiter;
        }
        _rawPath.append(path._rawPath);
    }
    return *this;
}

const std::string& Path::toString() const
{
    return _rawPath;
}

bool Path::operator<(const Path& path) const
{
    return _rawPath < path._rawPath;
}

void Path::_setRawPath(const char* rawPath)
{
    _rawPath = std::string(rawPath);
    std::string delimiter(" /");

    // Trim leading/trailing delimiters
    size_t begin = _rawPath.find_first_not_of(delimiter);
    if (begin != std::string::npos)
    {
        size_t end = _rawPath.find_last_not_of(delimiter);
        size_t range = end - begin + 1;

        _rawPath = _rawPath.substr(begin, range);
    }
}

namespace hect
{

std::ostream& operator<<(std::ostream& os, const Path& path)
{
    return os << path.toString();
}

}