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
#include "AssetCache.h"

using namespace hect;

AssetCache::AssetCache(size_t threadCount) :
    _taskPool(threadCount),
    _fileSystem(nullptr)
{
}

AssetCache::AssetCache(FileSystem& fileSystem, size_t threadCount) :
    _taskPool(threadCount),
    _fileSystem(&fileSystem)
{
}

void AssetCache::pushPreferredDirectory(const Path& directoryPath)
{
    std::lock_guard<std::recursive_mutex> lock(_mutex);

    std::stack<Path>& pathStack = _preferredDirectoryStack[std::this_thread::get_id()];
    pathStack.push(directoryPath);
}

void AssetCache::popPreferredDirectory()
{
    std::lock_guard<std::recursive_mutex> lock(_mutex);

    std::stack<Path>& pathStack = _preferredDirectoryStack[std::this_thread::get_id()];
    pathStack.pop();
}

void AssetCache::refresh()
{
    for (auto& pair : _entries)
    {
        pair.second->refresh();
    }
}

void AssetCache::clear()
{
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    _entries.clear();
}

TaskPool& AssetCache::taskPool()
{
    return _taskPool;
}

FileSystem& AssetCache::fileSystem()
{
    if (!_fileSystem)
    {
        throw Error("Asset cache does not have a file system");
    }
    return *_fileSystem;
}
