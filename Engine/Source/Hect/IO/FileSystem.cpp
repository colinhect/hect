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

    void read(uint8_t* bytes, size_t byte_count) override;
    bool end_of_stream() const override;
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
    const char* path_string = path.as_string().data();
    _handle = PHYSFS_openRead(path_string);
    if (!_handle)
    {
        throw IOError(format("Failed to open file '%s' for reading: %s", path_string, PHYSFS_getLastError()));
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

void FileReadStream::read(uint8_t* bytes, size_t byte_count)
{
    assert(_handle);
    assert(bytes);

    size_t length = this->length();
    size_t position = this->position();

    if (position + byte_count >= length + 1)
    {
        throw IOError("Attempt to read past end of file");
    }

    PHYSFS_sint64 result = PHYSFS_read(_handle, bytes, 1, static_cast<PHYSFS_uint32>(byte_count));
    if (result != static_cast<PHYSFS_sint64>(byte_count))
    {
        throw IOError(format("Failed to read from file: %s", PHYSFS_getLastError()));
    }
}

bool FileReadStream::end_of_stream() const
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

    void write(const uint8_t* bytes, size_t byte_count) override;
    size_t position() const override;
    void seek(size_t position) override;

private:
    Path _path;
    PHYSFS_File* _handle { nullptr };
};

FileWriteStream::FileWriteStream(const Path& path) :
    _path(path)
{
    const char* path_string = path.as_string().data();
    _handle = PHYSFS_openWrite(path_string);
    if (!_handle)
    {
        throw IOError(format("Failed to open file '%s' for writing: %s", path_string, PHYSFS_getLastError()));
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

void FileWriteStream::write(const uint8_t* bytes, size_t byte_count)
{
    assert(_handle);
    assert(bytes);

    PHYSFS_sint64 result = PHYSFS_write(_handle, bytes, 1, static_cast<PHYSFS_uint32>(byte_count));
    if (result != static_cast<PHYSFS_sint64>(byte_count))
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

Path FileSystem::base_directory()
{
    return convert_path(PHYSFS_getBaseDir());
}

Path FileSystem::working_directory()
{
#ifdef HECT_WINDOWS_BUILD
    char path[2048];
    GetCurrentDirectory(2048, path);
    return convert_path(path);
#else
    char* raw_path = get_current_dir_name();
    Path path(raw_path);
    free(raw_path);
    return path;
#endif
}

Path FileSystem::user_directory()
{
    return convert_path(PHYSFS_getUserDir());
}

Path FileSystem::application_data_directory()
{
#ifdef HECT_WINDOWS_BUILD
    TCHAR path[MAX_PATH];
    assert(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, path)));
    return convert_path(path);
#else
    return Path();
#endif
}

void FileSystem::set_write_directory(const Path& path)
{
    if (!PHYSFS_setWriteDir(path.as_string().data()))
    {
        throw IOError(format("Failed to set write directory to '%s': %s", path.as_string().data(), PHYSFS_getLastError()));
    }
}

void FileSystem::mount_archive(const Path& path, const Path& mount_point)
{
    const char* path_string = path.as_string().data();
    const char* mount_point_string = mount_point.as_string().data();

    if (!PHYSFS_mount(path_string, mount_point_string, 0))
    {
        throw IOError(format("Failed to mount archive '%s': %s", path_string, PHYSFS_getLastError()));
    }

    if (mount_point.empty())
    {
        HECT_INFO(format("Mounted archive '%s'", path_string));
    }
    else
    {
        HECT_INFO(format("Mounted archive '%s' to '%s'", path_string, mount_point_string));
    }
}

std::unique_ptr<ReadStream> FileSystem::open_file_for_read(const Path& path)
{
    return std::unique_ptr<ReadStream>(std::make_unique<FileReadStream>(path));
}

std::unique_ptr<WriteStream> FileSystem::open_file_for_write(const Path& path)
{
    return std::unique_ptr<WriteStream>(std::make_unique<FileWriteStream>(path));
}

void FileSystem::create_directory(const Path& path)
{
    if (!PHYSFS_mkdir(path.as_string().data()))
    {
        throw IOError(format("Failed to create directory: %s", PHYSFS_getLastError()));
    }
}

std::vector<Path> FileSystem::files_in_directory(const Path& path)
{
    char** paths = PHYSFS_enumerateFiles(path.as_string().data());

    std::vector<Path> resulting_paths;
    for (char** i = paths; *i != nullptr; ++i)
    {
        resulting_paths.push_back(path + *i);
    }

    PHYSFS_freeList(paths);

    return resulting_paths;
}

void FileSystem::remove(const Path& path)
{
    if (!PHYSFS_delete(path.as_string().data()))
    {
        throw IOError(format("Failed to remove directory: %s", PHYSFS_getLastError()));
    }
}

bool FileSystem::exists(const Path& path)
{
    return PHYSFS_exists(path.as_string().data()) != 0;
}

TimeStamp FileSystem::last_modified(const Path& path)
{
    return PHYSFS_getLastModTime(path.as_string().data());
}

FileSystem::FileSystem(int argc, char* const argv[])
{
    (void)argc;

    if (!PHYSFS_init(argv[0]))
    {
        throw FatalError(format("Failed to initialize PhysFS: %s", PHYSFS_getLastError()));
    }
}

Path FileSystem::convert_path(const char* raw_path)
{
    std::string delimiter(PHYSFS_getDirSeparator());
    std::string string(raw_path);

    size_t start = 0;
    while ((start = string.find(delimiter, start)) != std::string::npos)
    {
        string.replace(start, delimiter.length(), "/");
        start += 1;
    }

    return Path(string);
}
