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

#include "Hect/Concurrency/TaskPool.h"
#include "Hect/Core/Configuration.h"
#include "Hect/IO/Path.h"
#include "Hect/Timing/TimeStamp.h"

// Issue #75
#ifdef HECT_WINDOWS_BUILD
#pragma warning(disable : 4505)
#endif

namespace hect
{

class AssetCache;

///
/// Base asset entry.
class AssetEntryBase
{
public:
    virtual ~AssetEntryBase() { }

    ///
    /// Re-loads the asset if the asset file has been modified since it was
    /// last loaded.
    virtual void refresh() = 0;
};

///
/// Refers to the asset at a path.
template <typename T>
class AssetEntry :
    public AssetEntryBase
{
public:

    ///
    /// Constructs an asset entry given the asset cache and the path to the
    /// asset.
    ///
    /// \param assetCache The asset cache.
    /// \param path The path to the asset.
    AssetEntry(AssetCache& assetCache, const Path& path);

    void refresh();

    ///
    /// Returns the unique pointer to the asset.
    std::unique_ptr<T>& get();

    ///
    /// Returns the path of the asset.
    const Path& path() const;

private:
    void initiateLoad();
    void load();

    AssetCache* _assetCache;
    Path _path;

    Task::Handle _taskHandle;

    std::unique_ptr<T> _asset;

    bool _errorOccurred;
    std::string _errorMessage;

    TimeStamp _lastModified;
};

}

#include "AssetEntry.inl"