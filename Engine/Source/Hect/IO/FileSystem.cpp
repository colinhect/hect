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
#include "FileSystem.h"

#include <cassert>
#include <physfs.h>

#include "Hect/Core/Configuration.h"
#include "Hect/Core/Exception.h"
#include "Hect/Core/Format.h"
#include "Hect/Core/Logging.h"
#include "Hect/Core/Uncopyable.h"

#ifdef HECT_WINDOWS_BUILD
#include <Windows.h>
#include <shlwapi.h>
#include <shlobj.h>
#else
#include <unistd.h>
#endif

using namespace hect;

class HECT_EXPORT FileReadStream :
    public ReadStream,
    public Uncopyable
{
public:
    FileReadStream(const Path& path);
    ~FileReadStream();

    void read(uint8_t* bytes, size_t byteCount) override;
    bool endOfStream() const override;
    size_t length() const override;
    size_t position() const override;
    void seek(size_t position) override;

private:
    Path _path;
    PHYSFS_File* _handle { nullptr };
};

FileReadStream::FileReadStream(const Path& path) :
    _path(path)
{
    const char* pathString = path.asString().data();
    _handle = PHYSFS_openRead(pathString);
    if (!_handle)
    {
        throw IOError(format("Failed to open file '%s' for reading: %s", pathString, PHYSFS_getLastError()));
    }
}

FileReadStream::~FileReadStream()
{
    if (_handle)
    {
        if (!PHYSFS_close(_handle))
        {
            HECT_ERROR(format("Failed to close file for reading: %s", PHYSFS_getLastError()));
        }
    }
}

void FileReadStream::read(uint8_t* bytes, size_t byteCount)
{
    assert(_handle);
    assert(bytes);

    size_t length = this->length();
    size_t position = this->position();

    if (position + byteCount >= length + 1)
    {
        throw IOError("Attempt to read past end of file");
    }

    PHYSFS_sint64 result = PHYSFS_read(_handle, bytes, 1, static_cast<PHYSFS_uint32>(byteCount));
    if (result != static_cast<PHYSFS_sint64>(byteCount))
    {
        throw IOError(format("Failed to read from file: %s", PHYSFS_getLastError()));
    }
}

bool FileReadStream::endOfStream() const
{
    assert(_handle);
    return PHYSFS_eof(_handle) != 0;
}

size_t FileReadStream::length() const
{
    assert(_handle);
    return static_cast<size_t>(PHYSFS_fileLength(_handle));
}

size_t FileReadStream::position() const
{
    assert(_handle);
    return static_cast<size_t>(PHYSFS_tell(_handle));
}

void FileReadStream::seek(size_t position)
{
    assert(_handle);

    size_t length = this->length();

    if (position >= length + 1)
    {
        throw IOError("Attempt to seek past end of file");
    }

    if (!PHYSFS_seek(_handle, position))
    {
        throw IOError(format("Failed to seek in file: %s", PHYSFS_getLastError()));
    }
}

class HECT_EXPORT FileWriteStream :
    public WriteStream,
    public Uncopyable
{
public:
    FileWriteStream(const Path& path);
    ~FileWriteStream();

    void write(const uint8_t* bytes, size_t byteCount) override;
    size_t position() const override;
    void seek(size_t position) override;

private:
    Path _path;
    PHYSFS_File* _handle { nullptr };
};

FileWriteStream::FileWriteStream(const Path& path) :
    _path(path)
{
    const char* pathString = path.asString().data();
    _handle = PHYSFS_openWrite(pathString);
    if (!_handle)
    {
        throw IOError(format("Failed to open file '%s' for writing: %s", pathString, PHYSFS_getLastError()));
    }
}

FileWriteStream::~FileWriteStream()
{
    if (_handle)
    {
        if (!PHYSFS_close(_handle))
        {
            HECT_ERROR(format("Failed to close file for writing: %s", PHYSFS_getLastError()));
        }
    }
}

void FileWriteStream::write(const uint8_t* bytes, size_t byteCount)
{
    assert(_handle);
    assert(bytes);

    PHYSFS_sint64 result = PHYSFS_write(_handle, bytes, 1, static_cast<PHYSFS_uint32>(byteCount));
    if (result != static_cast<PHYSFS_sint64>(byteCount))
    {
        throw IOError(format("Failed to write to file: %s", PHYSFS_getLastError()));
    }
}

size_t FileWriteStream::position() const
{
    assert(_handle);
    return static_cast<size_t>(PHYSFS_tell(_handle));
}

void FileWriteStream::seek(size_t position)
{
    assert(_handle);
    if (!PHYSFS_seek(_handle, position))
    {
        throw IOError(format("Failed to seek in file: %s", PHYSFS_getLastError()));
    }
}

FileSystem::~FileSystem()
{
    if (!PHYSFS_deinit())
    {
        HECT_ERROR(format("Failed to shutdown PhysFS: %s", PHYSFS_getLastError()));
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
#else
    char* rawPath = get_current_dir_name();
    Path path(rawPath);
    free(rawPath);
    return path;
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
#else
    return Path();
#endif
}

void FileSystem::setWriteDirectory(const Path& path)
{
    if (!PHYSFS_setWriteDir(path.asString().data()))
    {
        throw IOError(format("Failed to set write directory to '%s': %s", path.asString().data(), PHYSFS_getLastError()));
    }
}

void FileSystem::mountArchive(const Path& path, const Path& mountPoint)
{
    const char* pathString = path.asString().data();
    const char* mountPointString = mountPoint.asString().data();

    if (!PHYSFS_mount(pathString, mountPointString, 0))
    {
        throw IOError(format("Failed to mount archive '%s': %s", pathString, PHYSFS_getLastError()));
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

std::unique_ptr<ReadStream> FileSystem::openFileForRead(const Path& path)
{
    return std::unique_ptr<ReadStream>(std::make_unique<FileReadStream>(path));
}

std::unique_ptr<WriteStream> FileSystem::openFileForWrite(const Path& path)
{
    return std::unique_ptr<WriteStream>(std::make_unique<FileWriteStream>(path));
}

void FileSystem::createDirectory(const Path& path)
{
    if (!PHYSFS_mkdir(path.asString().data()))
    {
        throw IOError(format("Failed to create directory: %s", PHYSFS_getLastError()));
    }
}

std::vector<Path> FileSystem::filesInDirectory(const Path& path)
{
    char** paths = PHYSFS_enumerateFiles(path.asString().data());

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
    if (!PHYSFS_delete(path.asString().data()))
    {
        throw IOError(format("Failed to remove directory: %s", PHYSFS_getLastError()));
    }
}

bool FileSystem::exists(const Path& path)
{
    return PHYSFS_exists(path.asString().data()) != 0;
}

TimeStamp FileSystem::lastModified(const Path& path)
{
    return PHYSFS_getLastModTime(path.asString().data());
}

FileSystem::FileSystem(int argc, char* const argv[])
{
    (void)argc;

    if (!PHYSFS_init(argv[0]))
    {
        throw FatalError(format("Failed to initialize PhysFS: %s", PHYSFS_getLastError()));
    }
}

Path FileSystem::convertPath(const char* rawPath)
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
