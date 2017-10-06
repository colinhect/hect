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
#include "Hect/Core/Exception.h"
#include "AssetHandle.h"

namespace hect
{

template <typename AssetType>
AssetHandle<AssetType>::AssetHandle()
{
}

template <typename AssetType>
AssetHandle<AssetType>::AssetHandle(AssetType* asset, bool owned)
{
    if (owned)
    {
        _owned.reset(asset);
    }
    else
    {
        _unowned = asset;
    }
}

template <typename AssetType>
AssetHandle<AssetType>::AssetHandle(const std::shared_ptr<AssetEntry<AssetType>>& entry) :
    _entry(entry)
{
}

template<typename AssetType>
AssetHandle<AssetType>::AssetHandle(AssetCache& asset_cache, const Path& asset_path)
{
    *this = asset_cache.get_handle<AssetType>(asset_path);
}

template <typename AssetType>
const Path& AssetHandle<AssetType>::path() const
{
    static Path empty_path;
    if (_entry)
    {
        return _entry->path();
    }

    return empty_path;
}

template <typename AssetType>
void AssetHandle<AssetType>::invalidate()
{
    _owned.reset();
    _unowned = nullptr;
    _entry.reset();
}

template <typename AssetType>
void AssetHandle<AssetType>::reset(AssetType* asset, bool owned)
{
    invalidate();

    *this = AssetHandle<AssetType>(asset, owned);
}

template <typename AssetType>
AssetHandle<AssetType>::operator bool() const
{
    return _entry || _owned || _unowned;
}

template <typename AssetType>
AssetType& AssetHandle<AssetType>::operator*() const
{
    if (_entry)
    {
        return *_entry->get();
    }
    else if (_owned)
    {
        return *_owned;
    }
    else if (_unowned)
    {
        return *_unowned;
    }

    throw InvalidOperation("Asset handle does not refer a valid asset");
}

template <typename AssetType>
AssetType* AssetHandle<AssetType>::operator->() const
{
    return &**this;
}

template <typename AssetType>
bool AssetHandle<AssetType>::operator==(const AssetHandle<AssetType>& asset_handle) const
{
    if (_entry)
    {
        return _entry == asset_handle._entry;
    }
    else if (_owned)
    {
        return _owned == asset_handle._owned;
    }
    else if (_unowned)
    {
        return _unowned == asset_handle._unowned;
    }
    return (_entry || _owned || _unowned) == (asset_handle._entry || asset_handle._owned || asset_handle._unowned);
}

template <typename AssetType>
bool AssetHandle<AssetType>::operator!=(const AssetHandle<AssetType>& asset_handle) const
{
    return !(*this == asset_handle);
}

template <typename AssetType>
Encoder& operator<<(Encoder& encoder, const AssetHandle<AssetType>& asset_handle)
{
    encoder << asset_handle.path().as_string();
    return encoder;
}

template <typename AssetType>
Decoder& operator>>(Decoder& decoder, AssetHandle<AssetType>& asset_handle)
{
    std::string path;
    decoder >> decode_value(path);
    if (!path.empty())
    {
        asset_handle = decoder.asset_cache().get_handle<AssetType>(path);
    }
    return decoder;
}

}
