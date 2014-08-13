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
    /// Selects a directory for the asset cache to first check for assets for
    /// the duration of its lifetime.
    class SelectDirectoryScope :
        public Uncopyable
    {
    public:

        ///
        /// Constructs thescope given the asset cache it affects along with
        /// path to the directory to select.
        ///
        /// \param assetCache The asset cache to select the directory for.
        /// \param directoryPath The path to the directory to select.
        SelectDirectoryScope(AssetCache& assetCache, const Path& directoryPath);

        ~SelectDirectoryScope();

        AssetCache* _assetCache;
    };

    AssetCache();

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

private:
    void selectDirectory(const Path& directoryPath);
    void restoreDirectory();

    std::recursive_mutex _mutex;
    std::map<std::thread::id, std::stack<Path>> _selectedDirectoryStack;
    std::map<Path, std::shared_ptr<AssetEntryBase>> _entries;

    TaskPool _taskPool;
};

}

#include "AssetCache.inl"