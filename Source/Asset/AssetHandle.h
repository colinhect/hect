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
/// A handle for an asset.
template <typename T>
class AssetHandle
{
public:

    ///
    /// An array of asset handles.
    typedef std::vector<AssetHandle<T>> Array;

    ///
    /// Constructs an empty asset handle.
    AssetHandle();

    ///
    /// Constructs an asset handle given an asset entry.
    ///
    /// \param entry The asset entry.
    AssetHandle(const std::shared_ptr<AssetEntry<T>>& entry);

    ///
    /// Returns a reference to the asset.
    ///
    /// \throws Error If the asset failed to load.
    T& get() const;

    ///
    /// Returns a shared pointer to the asset.
    ///
    /// \throws Error If the asset failed to load.
    std::shared_ptr<T> getShared() const;

    ///
    /// Returns the path to the asset.
    const Path& path() const;

private:
    std::shared_ptr<AssetEntry<T>> _entry;
};

}

#include "AssetHandle.inl"