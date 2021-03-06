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

#include "Hect/Concurrency/TaskPool.h"
#include "Hect/Core/Export.h"
#include "Hect/IO/Path.h"
#include "Hect/Timing/TimeStamp.h"

namespace hect
{

class AssetCache;

///
/// Abstract base for AssetEntry.
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
template <typename AssetType>
class AssetEntry :
    public AssetEntryBase
{
public:

    ///
    /// Constructs an asset entry.
    ///
    /// \param asset_cache The asset cache.
    /// \param path The path to the asset.
    /// \param constructor A function constructing a new instance of the asset.
    AssetEntry(AssetCache& asset_cache, const Path& path, std::function<AssetType*()> constructor);

    void refresh(bool force) override;

    ///
    /// Returns the unique pointer to the asset.
    std::unique_ptr<AssetType>& get();

    ///
    /// Returns the path of the asset.
    const Path& path() const;

private:
    void initiate_load();
    void load();

    AssetCache& _asset_cache;
    Path _path;

    std::function<AssetType*()> _constructor;

    Task::Handle _task_handle;

    std::unique_ptr<AssetType> _asset;

    bool _exception_occurred { false };
    std::string _exception_message;

    TimeStamp _last_modified { -1 };
};

}