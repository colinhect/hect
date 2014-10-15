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
#pragma once

#include <map>
#include <stack>

#include "Hect/Concurrency/TaskPool.h"
#include "Hect/Core/Uncopyable.h"
#include "Hect/IO/AssetHandle.h"
#include "Hect/IO/Path.h"
#include "Hect/Platform/FileSystem.h"

namespace hect
{

///
/// Provides cached access to assets loaded from the file system.
class AssetCache :
    public Uncopyable
{
public:

    ///
    /// Constructs an asset cache.
    ///
    /// \param concurrent Whether assets should be loaded on concurrent
    /// threads.
    AssetCache(bool concurrent);

    ~AssetCache();

    ///
    /// Returns a reference to the asset at the given path.
    ///
    /// \param path The case-sensitive path to the asset.
    ///
    /// \throws Error If the asset at the given path is of a different type or
    /// failed to load.
    template <typename T>
    T& get(const Path& path);

    ///
    /// Returns a handle for the asset at the given path.
    ///
    /// \note The asset is not necessarily loaded immediately.  The handle
    /// provides access to the asset itself.
    ///
    /// \param path The case-sensitive path to the asset.
    ///
    /// \throws Error If the asset at the given path is of a different type.
    template <typename T>
    AssetHandle<T> getHandle(const Path& path);

    ///
    /// Re-loads all assets whose file has been modified since it was last
    /// loaded.
    void refresh();

    ///
    /// Clears all cached resources.
    void clear();

    ///
    /// Returns the task pool.
    TaskPool& taskPool();

    ///
    /// Returns the full path to an asset given a partial path based on the
    /// the selected directory.
    ///
    /// \param path The path to resolve.
    Path resolvePath(const Path& path);

    ///
    /// Selects the given directory as the preferred directory for the current
    /// thread.
    ///
    /// \param directoryPath The path to the directory to select.
    void selectDirectory(const Path& directoryPath);

    ///
    /// Restores the previously selected directory (if any) for the current
    /// thread.
    void restoreDirectory();

private:
    TaskPool _taskPool;

    std::recursive_mutex _mutex;
    std::map<std::thread::id, std::stack<Path>> _selectedDirectoryStack;
    std::map<Path, std::shared_ptr<AssetEntryBase>> _entries;
};

}

#include "AssetCache.inl"