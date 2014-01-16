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

FileReadStream::~FileReadStream()
{
    if (_handle)
    {
        if (!PHYSFS_close((PHYSFS_File*)_handle))
        {
            throw Error(format("Failed to close file for reading: %s", PHYSFS_getLastError()));
        }
    }
}

void FileReadStream::readBytes(uint8_t* bytes, size_t byteCount)
{
    assert(bytes);

    size_t length = this->length();
    size_t position = this->position();

    if (position + byteCount >= length + 1)
    {
        throw Error("Attempt to read past end of file");
    }

    auto file = (PHYSFS_File*)_handle;
    PHYSFS_sint64 result = PHYSFS_read(file, bytes, 1, (PHYSFS_uint32)byteCount);
    if (result != (PHYSFS_sint64)byteCount)
    {
        throw Error(format("Failed to read from file: %s", PHYSFS_getLastError()));
    }
}

bool FileReadStream::endOfStream() const
{
    auto file = (PHYSFS_File*)_handle;
    return PHYSFS_eof(file) != 0;
}

size_t FileReadStream::length() const
{
    auto file = (PHYSFS_File*)_handle;
    return (size_t)PHYSFS_fileLength(file);
}

size_t FileReadStream::position() const
{
    auto file = (PHYSFS_File*)_handle;
    return (size_t)PHYSFS_tell(file);
}

void FileReadStream::seek(size_t position)
{
    size_t length = this->length();

    if (position >= length + 1)
    {
        throw Error("Attempt to seek past end of file");
    }

    auto file = (PHYSFS_File*)_handle;
    if (!PHYSFS_seek(file, position))
    {
        throw Error(format("Failed to seek in file: %s", PHYSFS_getLastError()));
    }
}

FileReadStream::FileReadStream(const Path& path) :
    _path(path),
    _handle(nullptr)
{
    std::stringstream ss;
    ss << path;

    _handle = PHYSFS_openRead(ss.str().c_str());
    if (!_handle)
    {
        throw Error(format("Failed to open file for reading: %s", PHYSFS_getLastError()));
    }
}