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

class AssetCache;

///
/// Base asset entry.
class AssetEntryBase
{
public:
    virtual ~AssetEntryBase() { }
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

    ///
    /// Returns a shared pointer to the asset.
    std::shared_ptr<T> get();

    ///
    /// Returns the path of the asset.
    const Path& path() const;

private:
    void _load();

    AssetCache* _assetCache;
    Path _path;

    std::shared_ptr<T> _asset;

    bool _errorOccurred;
    std::string _errorMessage;
};

}

#include "AssetEntry.inl"