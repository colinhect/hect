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

#include <physfs.h>

using namespace hect;

FileWriteStream::~FileWriteStream()
{
    if (_handle)
    {
        if (!PHYSFS_close((PHYSFS_File*)_handle))
        {
            throw Error(format("Failed to close file for writing: %s", PHYSFS_getLastError()));
        }
    }
}

void FileWriteStream::writeBytes(const uint8_t* bytes, size_t byteCount)
{
    assert(bytes);

    auto file = (PHYSFS_File*)_handle;
    PHYSFS_sint64 result = PHYSFS_write(file, bytes, 1, (PHYSFS_uint32)byteCount);
    if (result != (PHYSFS_sint64)byteCount)
    {
        throw Error(format("Failed to write to file: %s", PHYSFS_getLastError()));
    }
}

size_t FileWriteStream::position() const
{
    auto file = (PHYSFS_File*)_handle;
    return (size_t)PHYSFS_tell(file);
}

void FileWriteStream::seek(size_t position)
{
    auto file = (PHYSFS_File*)_handle;
    if (!PHYSFS_seek(file, position))
    {
        throw Error(format("Failed to seek in file: %s", PHYSFS_getLastError()));
    }
}

FileWriteStream::FileWriteStream(const Path& path) :
    _path(path),
    _handle(nullptr)
{
    std::stringstream ss;
    ss << path;

    _handle = PHYSFS_openWrite(ss.str().c_str());
    if (!_handle)
    {
        throw Error(format("Failed to open file for writing: %s", PHYSFS_getLastError()));
    }
}