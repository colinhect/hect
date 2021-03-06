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

#include "Hect/Core/Name.h"
#include "Hect/IO/AssetCache.h"
#include "Hect/IO/Encodable.h"

namespace hect
{

///
/// Denotes an inline path to a asset dependency.
#define HECT_ASSET(path) path

///
/// Inheriting from this allows for caching/loading using an AssetCache.
template <typename AssetType>
class Asset :
    public Encodable
{
public:

    ///
    /// Constructs an asset.
    Asset();

    ///
    /// Constructs an asset.
    ///
    /// \param name The name of the asset.
    Asset(Name name);

    ///
    /// Returns the name.
    Name name() const;

    ///
    /// Sets the name.
    ///
    /// \param name The new name.
    void set_name(Name name);

    ///
    /// Returns an unowned handle to the asset.
    ///
    /// \warning The lifetime of the asset must out-live the handle.
    AssetHandle<AssetType> create_handle();

private:
    Name _name;
};

}

#include "Asset.inl"
