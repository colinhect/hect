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

#include <memory>

#include "Hect/IO/AssetEntry.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"
#include "Hect/IO/Path.h"

namespace hect
{

///
/// A reference-counted handle for an Asset.
///
/// \note An asset handle may reference an asset which was loaded from the
/// file system and is cached in an asset cache.  Alternatively, it may
/// reference a manually created asset.  A handle works much like a smart
/// pointer.
template <typename T>
class AssetHandle
{
public:

    ///
    /// Constructs an empty asset handle.
    AssetHandle();

    ///
    /// Constructs an asset handle.
    ///
    /// \param asset A pointer to the asset.
    /// \param owned Whether the lifetime of the asset should be owned by the
    /// handle.
    AssetHandle(T* asset, bool owned = true);

    ///
    /// Constructs an asset handle.
    ///
    /// \param entry The asset entry.
    AssetHandle(const std::shared_ptr<AssetEntry<T>>& entry);

    ///
    /// Returns the path to the asset.
    ///
    /// \note If the asset handle does not refer to an asset which was
    /// loaded from the file system, then an empty path is returned.
    const Path& path() const;

    ///
    /// Returns whether the handle refers to a asset.
    operator bool() const;

    ///
    /// Returns a reference to the asset.
    ///
    /// \throws Error If the handle does not refer to an asset or if the
    /// referred asset failed to load.
    T& operator*() const;

    ///
    /// Provides pointer-like access to the asset.
    ///
    /// \throws Error If the handle does not refer to an asset or if the
    /// referred asset failed to load.
    T* operator->() const;

    ///
    /// Returns whether the handle refers to the same asset as another handle.
    ///
    /// \param assetHandle The other asset handle.
    bool operator==(const AssetHandle<T>& assetHandle) const;

    ///
    /// Returns whether the handle refers to a different asset as another handle.
    ///
    /// \param assetHandle The other asset handle.
    bool operator!=(const AssetHandle<T>& assetHandle) const;

private:
    std::shared_ptr<T> _owned;
    T* _unowned { nullptr };
    std::shared_ptr<AssetEntry<T>> _entry;
};

template <typename T>
Encoder& operator<<(Encoder& encoder, const AssetHandle<T>& assetHandle);

template <typename T>
Decoder& operator>>(Decoder& decoder, AssetHandle<T>& assetHandle);

}