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
#pragma once

#include <map>
#include <stack>

#include "Hect/Concurrency/TaskPool.h"
#include "Hect/Core/Export.h"
#include "Hect/Core/Uncopyable.h"
#include "Hect/IO/AssetHandle.h"
#include "Hect/IO/FileSystem.h"
#include "Hect/IO/Path.h"

namespace hect
{

///
/// Provides cached access to Asset%s from the FileSystem.
class HECT_EXPORT AssetCache :
    public Uncopyable
{
public:

    ///
    /// Constructs an asset cache.
    ///
    /// \param file_system The file system.
    /// \param concurrent Whether assets should be loaded in concurrent
    /// threads.
    AssetCache(FileSystem& file_system, bool concurrent);

    ~AssetCache();

    FileSystem& file_system();

    ///
    /// Returns a reference to the asset at the given path.
    ///
    /// \param path The case-sensitive path to the asset.
    /// \param args The arguments to pass to the asset's constructor if the
    /// asset is loaded.
    ///
    /// \throws InvalidOperation If the asset at the given path is of a
    /// different type.
    /// \throws FatalError If the asset failed to load.
    template <typename AssetType, typename... Args>
    AssetType& get(const Path& path, Args&&... args);

    ///
    /// Returns a handle for the asset at the given path.
    ///
    /// \note The asset is not necessarily loaded immediately.  The handle
    /// provides access to the asset itself.
    ///
    /// \param path The case-sensitive path to the asset.
    /// \param args The arguments to pass to the asset's constructor if the
    /// asset is loaded.
    ///
    /// \throws InvalidOperation If the asset at the given path is of a
    /// different type.
    template <typename AssetType, typename... Args>
    AssetHandle<AssetType> get_handle(const Path& path, Args&&... args);

    ///
    /// Re-loads any cached assets of a specific type.
    ///
    /// \param only_modified Whether to only re-load the assets whose asset
    /// files have been modified since it was loaded.
    template <typename AssetType>
    void refresh(bool only_modified);

    ///
    /// Re-loads any cached assets of all types.
    ///
    /// \param only_modified Whether to only re-load the assets whose asset
    /// files have been modified since it was loaded.
    void refresh(bool only_modified);

    ///
    /// Removes an asset from a handle from the cache.
    template <typename AssetType>
    void remove(const AssetHandle<AssetType>& handle);

    ///
    /// Removes an asset at the given path from the cache.
    ///
    /// \param path The case-sensitive path to the asset to remove.
    void remove(const Path& path);

    ///
    /// Clears all cached assets.
    void clear();

    ///
    /// Returns the task pool used for asset loading.
    TaskPool& task_pool();

    ///
    /// Returns the full path to an asset given a partial path based on the
    /// the current preferred directory.
    ///
    /// \param path The path to resolve.
    /// \param prefer_yaml_file Whether to prefer a YAML source file if one
    /// exists.
    Path resolve_path(const Path& path, bool prefer_yaml_file = true);

    ///
    /// Push the given directory as the preferred directory for the current
    /// thread.
    ///
    /// \param directory_path The path to the directory to push.
    void push_directory(const Path& directory_path);

    ///
    /// Pops the previously pushed directory (if any) as the preferred
    /// directory for the current thread.
    void pop_directory();

private:
    FileSystem& _file_system;
    TaskPool _task_pool;

    std::recursive_mutex _mutex;
    std::map<std::thread::id, std::stack<Path>> _directory_stack;
    std::map<Path, std::shared_ptr<AssetEntryBase>> _entries;
};

}

#include "AssetCache.inl"
#include "AssetHandle.inl"
#include "AssetEntry.inl"
