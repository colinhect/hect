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

const char _pathDelimiter = '/';

}

Path::Path()
{
}

Path::Path(const char* path)
{
    setRawPath(path);
}

Path::Path(const std::string& path)
{
    setRawPath(path.data());
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

Path Path::parentDirectory() const
{
    // Find the position of the first '/' starting from the right
    size_t i = _rawPath.size();
    for (; i > 0; --i)
    {
        if (_rawPath[i] == _pathDelimiter)
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
    return _rawPath.substr(0, i);
}

bool Path::empty() const
{
    return _rawPath.empty();
}

Path Path::operator+(const Path& path) const
{
    Path result(*this);

    // Only add if the right-hand side is not empty
    if (!path._rawPath.empty())
    {
        // Add a delimiter between the paths if the left-hand side is not
        // empty and the right-hand side does not start with a delimeter
        if (!result._rawPath.empty() && path._rawPath[0] != _pathDelimiter)
        {
            result._rawPath += _pathDelimiter;
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
            _rawPath += _pathDelimiter;
        }
        _rawPath.append(path._rawPath);
    }
    return *this;
}

const std::string& Path::asString() const
{
    return _rawPath;
}

bool Path::operator<(const Path& path) const
{
    return _rawPath < path._rawPath;
}

bool Path::operator==(const Path& path) const
{
    return _rawPath == path._rawPath;
}

bool Path::operator!=(const Path& path) const
{
    return _rawPath != path._rawPath;
}

void Path::setRawPath(const char* rawPath)
{
    _rawPath = std::string(rawPath);
    std::string delimiter(" /");

    // Trim trailing delimiter
    size_t end = _rawPath.find_last_not_of(delimiter);
    _rawPath = _rawPath.substr(0, end + 1);
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const Path& path)
{
    encoder << encodeValue(path.asString());
    return encoder;
}

Decoder& operator>>(Decoder& decoder, Path& path)
{
    std::string rawPath;
    decoder >> decodeValue(rawPath);
    path = Path(rawPath);
    return decoder;
}

}
