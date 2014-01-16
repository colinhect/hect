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

namespace hect
{

///
/// Provides cached access to assets loaded from the file system.
class AssetCache :
    public Uncopyable
{
public:

    ///
    /// Constructs an asset cache given a file system.
    ///
    /// \param fileSystem The file system.
    AssetCache(FileSystem& fileSystem);

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
    /// \remarks The asset is not necessarily loaded immediately.  The handle
    /// provides access to the asset itself.
    ///
    /// \param path The case-sensitive path to the asset.
    ///
    /// \throws Error If the asset at the given path is of a different type.
    template <typename T>
    AssetHandle<T> getHandle(const Path& path);

    ///
    /// Clears all cached resources.
    void clear();

    ///
    /// Returns the file system.
    FileSystem& fileSystem();

private:
    FileSystem& _fileSystem;

    std::map<Path, std::shared_ptr<AssetEntryBase>> _entries;
};

}

#include "AssetCache.inl"