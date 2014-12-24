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
#include "PhysFSFileSystem.h"

#include <cassert>
#include <physfs.h>

#include "Hect/Core/Configuration.h"
#include "Hect/Core/Exception.h"
#include "Hect/Core/Format.h"
#include "Hect/Core/Logging.h"
#include "Hect/Core/Uncopyable.h"
#include "Hect/IO/FileSystems/PhysFSReadStream.h"
#include "Hect/IO/FileSystems/PhysFSWriteStream.h"

#ifdef HECT_WINDOWS_BUILD
#include <Windows.h>
#include <shlwapi.h>
#include <shlobj.h>
#else
#include <unistd.h>
#endif

using namespace hect;

PhysFSFileSystem::PhysFSFileSystem(int argc, char* const argv[])
{
    (void)argc;

    assert(!_initialized);
    if (!PHYSFS_init(argv[0]))
    {
        throw FatalError(format("Failed to initialize file system: %s", PHYSFS_getLastError()));
    }
    _initialized = true;
}

PhysFSFileSystem::~PhysFSFileSystem()
{
    if (!PHYSFS_deinit())
    {
        throw FatalError(format("Failed to shutdown file system: %s", PHYSFS_getLastError()));
    }
}

Path PhysFSFileSystem::baseDirectory()
{
    return convertPath(PHYSFS_getBaseDir());
}

Path PhysFSFileSystem::workingDirectory()
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

Path PhysFSFileSystem::userDirectory()
{
    return convertPath(PHYSFS_getUserDir());
}

Path PhysFSFileSystem::applicationDataDirectory()
{
#ifdef HECT_WINDOWS_BUILD
    TCHAR path[MAX_PATH];
    assert(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, path)));
    return convertPath(path);
#else
    return Path();
#endif
}

void PhysFSFileSystem::setWriteDirectory(const Path& path)
{
    if (!PHYSFS_setWriteDir(path.asString().c_str()))
    {
        throw FatalError(format("Failed to set write directory to '%s': %s", path.asString().c_str(), PHYSFS_getLastError()));
    }
}

void PhysFSFileSystem::mountArchive(const Path& path, const Path& mountPoint)
{
    const char* pathString = path.asString().c_str();
    const char* mountPointString = mountPoint.asString().c_str();

    if (!PHYSFS_mount(pathString, mountPointString, 0))
    {
        throw FatalError(format("Failed to mount archive '%s': %s", pathString, PHYSFS_getLastError()));
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

std::unique_ptr<ReadStream> PhysFSFileSystem::openFileForRead(const Path& path)
{
    return std::unique_ptr<ReadStream>(new PhysFSReadStream(path));
}

std::unique_ptr<WriteStream> PhysFSFileSystem::openFileForWrite(const Path& path)
{
    return std::unique_ptr<WriteStream>(new PhysFSWriteStream(path));
}

void PhysFSFileSystem::createDirectory(const Path& path)
{
    if (!PHYSFS_mkdir(path.asString().c_str()))
    {
        throw FatalError(format("Failed to create directory: %s", PHYSFS_getLastError()));
    }
}

std::vector<Path> PhysFSFileSystem::filesInDirectory(const Path& path)
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

void PhysFSFileSystem::remove(const Path& path)
{
    if (!PHYSFS_delete(path.asString().c_str()))
    {
        throw FatalError(format("Failed to remove directory: %s", PHYSFS_getLastError()));
    }
}

bool PhysFSFileSystem::exists(const Path& path)
{
    return PHYSFS_exists(path.asString().c_str()) != 0;
}

TimeStamp PhysFSFileSystem::lastModified(const Path& path)
{
    return PHYSFS_getLastModTime(path.asString().c_str());
}

Path PhysFSFileSystem::convertPath(const char* rawPath)
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

bool PhysFSFileSystem::_initialized = false;
