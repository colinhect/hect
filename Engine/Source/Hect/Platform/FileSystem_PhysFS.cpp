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
#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"
#include "Hect/Core/Logging.h"
#include "Hect/Core/Uncopyable.h"

#ifdef HECT_WINDOWS_BUILD
#include <Windows.h>
#include <shlwapi.h>
#include <shlobj.h>
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
        _handle = PHYSFS_openRead(path.asString().c_str());
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

    void read(uint8_t* bytes, size_t byteCount)
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
        _handle = PHYSFS_openWrite(path.asString().c_str());
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

    void write(const uint8_t* bytes, size_t byteCount)
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

Path FileSystem::baseDirectory()
{
    return convertPath(PHYSFS_getBaseDir());
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

Path FileSystem::applicationDataDirectory()
{
#ifdef HECT_WINDOWS_BUILD
    TCHAR path[MAX_PATH];
    assert(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, path)));
    return convertPath(path);
#endif
}

void FileSystem::setWriteDirectory(const Path& path)
{
    if (!PHYSFS_setWriteDir(path.asString().c_str()))
    {
        throw Error(format("Failed to set write directory: %s", PHYSFS_getLastError()));
    }
}

void FileSystem::mountArchive(const Path& path, const Path& mountPoint)
{
    const char* pathString = path.asString().c_str();
    const char* mountPointString = mountPoint.asString().c_str();

    if (!PHYSFS_mount(pathString, mountPointString, 0))
    {
        throw Error(format("Failed to mount archive '%s': %s", pathString, PHYSFS_getLastError()));
    }

    if (mountPoint.empty())
    {
        HECT_INFO(format("Mounted archive '%s'", pathString));
    }
    else
    {
        HECT_INFO(format("Mounted archive '%s' to '%s'", pathString, mountPointString));
    }
}

ReadStream FileSystem::openFileForRead(const Path& path)
{
    return ReadStream(new PhysFSReadStream(path));
}

WriteStream FileSystem::openFileForWrite(const Path& path)
{
    return WriteStream(new PhysFSWriteStream(path));
}

void FileSystem::createDirectory(const Path& path)
{
    if (!PHYSFS_mkdir(path.asString().c_str()))
    {
        throw Error(format("Failed to create directory: %s", PHYSFS_getLastError()));
    }
}

std::vector<Path> FileSystem::filesInDirectory(const Path& path)
{
    char** paths = PHYSFS_enumerateFiles(path.asString().c_str());

    std::vector<Path> resultingPaths;
    for (char** i = paths; *i != nullptr; ++i)
    {
        resultingPaths.push_back(path + *i);
    }

    PHYSFS_freeList(paths);

    return resultingPaths;
}

void FileSystem::remove(const Path& path)
{
    if (!PHYSFS_delete(path.asString().c_str()))
    {
        throw Error(format("Failed to remove directory: %s", PHYSFS_getLastError()));
    }
}

bool FileSystem::exists(const Path& path)
{
    return PHYSFS_exists(path.asString().c_str()) != 0;
}

TimeStamp FileSystem::lastModified(const Path& path)
{
    return PHYSFS_getLastModTime(path.asString().c_str());
}
