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
#include "AssetCache.h"

using namespace hect;

AssetCache::AssetCache(FileSystem& file_system, bool concurrent) :
    _file_system(file_system),
    _task_pool(0, concurrent)
{
}

AssetCache::~AssetCache()
{
    _task_pool.wait();
}

FileSystem& AssetCache::file_system()
{
    return _file_system;
}

void AssetCache::refresh(bool only_modified)
{
    std::lock_guard<std::recursive_mutex> lock(_mutex);

    bool force = !only_modified;
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

TaskPool& AssetCache::task_pool()
{
    return _task_pool;
}

Path AssetCache::resolve_path(const Path& path, bool prefer_yaml_file)
{
    std::lock_guard<std::recursive_mutex> lock(_mutex);

    if (prefer_yaml_file)
    {
        Path resolved_path;

        // Check for a yaml source file
        if (path.extension() != "yaml")
        {
            Path yaml_path = resolve_path(path.as_string() + ".yaml", false);
            if (_file_system.exists(yaml_path))
            {
                resolved_path = yaml_path;
            }
        }

        // If no yaml source file was found
        if (resolved_path.empty())
        {
            // Resolve the initial path
            resolved_path = resolve_path(path, false);
        }

        return resolved_path;
    }
    else
    {
        Path resolved_path = path;

        const std::stack<Path>& path_stack = _directory_stack[std::this_thread::get_id()];

        // If there is a selected directory
        if (!path_stack.empty())
        {
            // If there is an asset relative to the selected directory
            if (_file_system.exists(path_stack.top() + path))
            {
                // Use that asset
                resolved_path = path_stack.top() + path;
            }
        }

        return resolved_path;
    }
}

void AssetCache::push_directory(const Path& directory_path)
{
    std::lock_guard<std::recursive_mutex> lock(_mutex);

    std::stack<Path>& path_stack = _directory_stack[std::this_thread::get_id()];
    path_stack.push(directory_path);
}

void AssetCache::pop_directory()
{
    std::lock_guard<std::recursive_mutex> lock(_mutex);

    std::stack<Path>& path_stack = _directory_stack[std::this_thread::get_id()];
    path_stack.pop();
}
