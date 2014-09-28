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
#include "FileSystem.h"

#include <cassert>
#include <physfs.h>

#include "Hect/Core/Configuration.h"
#include "Hect/Core/Uncopyable.h"
#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"

#ifdef HECT_WINDOWS_BUILD
#include <Windows.h>
#endif

using namespace hect;

namespace
{

Path convertPath(const char* rawPath)
{
    std::string delimiter(PHYSFS_getDirSeparator());
    std::string string(rawPath);

    size_t start = 0;
    while ((start = string.find(delimiter, start)) != std::string::npos)
    {
        string.replace(start, delimiter.length(), "/");
        start += 1;
    }

    return Path(string);
}

class PhysFSReadStream :
    public ReadStream,
    public Uncopyable
{
public:
    PhysFSReadStream(const Path& path) :
        _path(path),
        _handle(nullptr)
    {
        _handle = PHYSFS_openRead(path.toString().c_str());
        if (!_handle)
        {
            throw Error(format("Failed to open file for reading: %s", PHYSFS_getLastError()));
        }
    }

    ~PhysFSReadStream()
    {
        if (_handle)
        {
            if (!PHYSFS_close(_handle))
            {
                throw Error(format("Failed to close file for reading: %s", PHYSFS_getLastError()));
            }
        }
    }

    void readBytes(uint8_t* bytes, size_t byteCount)
    {
        assert(bytes);

        size_t length = this->length();
        size_t position = this->position();

        if (position + byteCount >= length + 1)
        {
            throw Error("Attempt to read past end of file");
        }

        PHYSFS_sint64 result = PHYSFS_read(_handle, bytes, 1, (PHYSFS_uint32)byteCount);
        if (result != (PHYSFS_sint64)byteCount)
        {
            throw Error(format("Failed to read from file: %s", PHYSFS_getLastError()));
        }
    }

    bool endOfStream() const
    {
        return PHYSFS_eof(_handle) != 0;
    }

    size_t length() const
    {
        return (size_t)PHYSFS_fileLength(_handle);
    }

    size_t position() const
    {
        return (size_t)PHYSFS_tell(_handle);
    }

    void seek(size_t position)
    {
        size_t length = this->length();

        if (position >= length + 1)
        {
            throw Error("Attempt to seek past end of file");
        }

        if (!PHYSFS_seek(_handle, position))
        {
            throw Error(format("Failed to seek in file: %s", PHYSFS_getLastError()));
        }
    }

private:
    Path _path;
    PHYSFS_File* _handle;
};

class PhysFSWriteStream :
    public WriteStream,
    public Uncopyable
{
public:
    PhysFSWriteStream(const Path& path) :
        _path(path),
        _handle(nullptr)
    {
        _handle = PHYSFS_openWrite(path.toString().c_str());
        if (!_handle)
        {
            throw Error(format("Failed to open file for writing: %s", PHYSFS_getLastError()));
        }
    }

    PhysFSWriteStream::~PhysFSWriteStream()
    {
        if (_handle)
        {
            if (!PHYSFS_close(_handle))
            {
                throw Error(format("Failed to close file for writing: %s", PHYSFS_getLastError()));
            }
        }
    }

    void writeBytes(const uint8_t* bytes, size_t byteCount)
    {
        assert(bytes);

        PHYSFS_sint64 result = PHYSFS_write(_handle, bytes, 1, (PHYSFS_uint32)byteCount);
        if (result != (PHYSFS_sint64)byteCount)
        {
            throw Error(format("Failed to write to file: %s", PHYSFS_getLastError()));
        }
    }

    size_t position() const
    {
        return (size_t)PHYSFS_tell(_handle);
    }

    void seek(size_t position)
    {
        if (!PHYSFS_seek(_handle, position))
        {
            throw Error(format("Failed to seek in file: %s", PHYSFS_getLastError()));
        }
    }

private:
    Path _path;
    PHYSFS_File* _handle;
};

}

void FileSystem::initialize()
{
    if (!PHYSFS_init(nullptr))
    {
        throw Error(format("Failed to initialize file system: %s", PHYSFS_getLastError()));
    }
}

void FileSystem::deinitialize()
{
    if (!PHYSFS_deinit())
    {
        throw Error(format("Failed to shutdown file system: %s", PHYSFS_getLastError()));
    }
}

Path FileSystem::workingDirectory()
{
#ifdef HECT_WINDOWS_BUILD
    char path[2048];
    GetCurrentDirectory(2048, path);
    return convertPath(path);
#endif
}

Path FileSystem::userDirectory()
{
    return convertPath(PHYSFS_getUserDir());
}

void FileSystem::setWriteDirectory(const Path& path)
{
    if (!PHYSFS_setWriteDir(path.toString().c_str()))
    {
        throw Error(format("Failed to set write directory: %s", PHYSFS_getLastError()));
    }
}

void FileSystem::mount(const Path& path)
{
    if (!PHYSFS_mount(path.toString().c_str(), NULL, 0))
    {
        throw Error(format("Failed to mount path: %s", PHYSFS_getLastError()));
    }
}

ReadStream::Pointer FileSystem::openFileForRead(const Path& path)
{
    return ReadStream::Pointer(new PhysFSReadStream(path));
}

WriteStream::Pointer FileSystem::openFileForWrite(const Path& path)
{
    return WriteStream::Pointer(new PhysFSWriteStream(path));
}

void FileSystem::createDirectory(const Path& path)
{
    if (!PHYSFS_mkdir(path.toString().c_str()))
    {
        throw Error(format("Failed to create directory: %s", PHYSFS_getLastError()));
    }
}

void FileSystem::remove(const Path& path)
{
    if (!PHYSFS_delete(path.toString().c_str()))
    {
        throw Error(format("Failed to remove directory: %s", PHYSFS_getLastError()));
    }
}

bool FileSystem::exists(const Path& path)
{
    return PHYSFS_exists(path.toString().c_str()) != 0;
}

TimeStamp FileSystem::lastModified(const Path& path)
{
    return PHYSFS_getLastModTime(path.toString().c_str());
}
