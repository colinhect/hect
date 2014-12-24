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
#include "Hect/Core/Export.h"
#include "Hect/IO/Path.h"
#include "Hect/Timing/TimeStamp.h"

namespace hect
{

class AssetCache;

///
/// Base asset entry.
class HECT_EXPORT AssetEntryBase
{
public:
    virtual ~AssetEntryBase() { }

    ///
    /// Re-loads the asset if the asset file has been modified since it was
    /// last loaded.
    ///
    /// \param force Whether to force the re-load even if the asset file has
    /// not been modified.
    virtual void refresh(bool force) = 0;
};

///
/// Refers to an Asset existing at a specific path in the FileSystem.
template <typename T>
class AssetEntry :
    public AssetEntryBase
{
public:

    ///
    /// Constructs an asset entry.
    ///
    /// \param assetCache The asset cache.
    /// \param path The path to the asset.
    /// \param constructor A function constructing a new instance of the asset.
    AssetEntry(AssetCache& assetCache, const Path& path, std::function<T*()> constructor);

    void refresh(bool force) override;

    ///
    /// Returns the unique pointer to the asset.
    std::unique_ptr<T>& get();

    ///
    /// Returns the path of the asset.
    const Path& path() const;

private:
    void initiateLoad();
    void load();

    AssetCache& _assetCache;
    Path _path;

    std::function<T*()> _constructor;

    Task::Handle _taskHandle;

    std::unique_ptr<T> _asset;

    bool _errorOccurred { false };
    std::string _errorMessage;

    TimeStamp _lastModified { -1 };
};

}