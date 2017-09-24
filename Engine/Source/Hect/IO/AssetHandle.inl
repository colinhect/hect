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

template <typename T>
AssetHandle<T>::AssetHandle()
{
}

template <typename T>
AssetHandle<T>::AssetHandle(T* asset, bool owned)
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

template <typename T>
AssetHandle<T>::AssetHandle(const std::shared_ptr<AssetEntry<T>>& entry) :
    _entry(entry)
{
}

template<typename T>
AssetHandle<T>::AssetHandle(AssetCache& asset_cache, const Path& path)
{
    *this = asset_cache.get_handle<T>(path);
}

template <typename T>
const Path& AssetHandle<T>::path() const
{
    static Path empty_path;
    if (_entry)
    {
        return _entry->path();
    }

    return empty_path;
}

template <typename T>
void AssetHandle<T>::invalidate()
{
    _owned.reset();
    _unowned = nullptr;
    _entry.reset();
}

template <typename T>
void AssetHandle<T>::reset(T* asset, bool owned)
{
    invalidate();

    *this = AssetHandle<T>(asset, owned);
}

template <typename T>
AssetHandle<T>::operator bool() const
{
    return _entry || _owned || _unowned;
}

template <typename T>
T& AssetHandle<T>::operator*() const
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

template <typename T>
T* AssetHandle<T>::operator->() const
{
    return &**this;
}

template <typename T>
bool AssetHandle<T>::operator==(const AssetHandle<T>& asset_handle) const
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

template <typename T>
bool AssetHandle<T>::operator!=(const AssetHandle<T>& asset_handle) const
{
    return !(*this == asset_handle);
}

template <typename T>
Encoder& operator<<(Encoder& encoder, const AssetHandle<T>& asset_handle)
{
    encoder << asset_handle.path().as_string();
    return encoder;
}

template <typename T>
Decoder& operator>>(Decoder& decoder, AssetHandle<T>& asset_handle)
{
    std::string path;
    decoder >> decode_value(path);
    if (!path.empty())
    {
        asset_handle = decoder.asset_cache().get_handle<T>(path);
    }
    return decoder;
}

}
