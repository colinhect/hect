///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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

AssetCache::AssetCache(FileSystem& fileSystem, bool concurrent) :
    _fileSystem(fileSystem),
    _taskPool(0, concurrent)
{
}

AssetCache::~AssetCache()
{
    _taskPool.wait();
}

FileSystem& AssetCache::fileSystem()
{
    return _fileSystem;
}

void AssetCache::refresh(bool onlyModified)
{
    std::lock_guard<std::recursive_mutex> lock(_mutex);

    bool force = !onlyModified;
    for (auto& pair : _entries)
    {
        pair.second->refresh(force);
    }
}

void AssetCache::remove(const Path& path)
{
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    _entries.erase(path);
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

Path AssetCache::resolvePath(const Path& path)
{
    std::lock_guard<std::recursive_mutex> lock(_mutex);

    Path resolvedPath = path;

    const std::stack<Path>& pathStack = _directoryStack[std::this_thread::get_id()];

    // If there is a selected directory
    if (!pathStack.empty())
    {
        // If there is an asset relative to the selected directory
        if (_fileSystem.exists(pathStack.top() + path))
        {
            // Use that asset
            resolvedPath = pathStack.top() + path;
        }
    }

    return resolvedPath;
}

void AssetCache::pushDirectory(const Path& directoryPath)
{
    std::lock_guard<std::recursive_mutex> lock(_mutex);

    std::stack<Path>& pathStack = _directoryStack[std::this_thread::get_id()];
    pathStack.push(directoryPath);
}

void AssetCache::popDirectory()
{
    std::lock_guard<std::recursive_mutex> lock(_mutex);

    std::stack<Path>& pathStack = _directoryStack[std::this_thread::get_id()];
    pathStack.pop();
}