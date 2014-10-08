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
#include "Hect/Core/Error.h"

namespace hect
{

template <typename T>
AssetHandle<T>::AssetHandle()
{
}

template <typename T>
AssetHandle<T>::AssetHandle(T* asset) :
    _asset(asset)
{
}

template <typename T>
AssetHandle<T>::AssetHandle(const std::shared_ptr<AssetEntry<T>>& entry) :
    _entry(entry)
{
}

template <typename T>
const Path& AssetHandle<T>::path() const
{
    static Path emptyPath;
    if (_entry)
    {
        return _entry->path();
    }

    return emptyPath;
}

template <typename T>
AssetHandle<T>::operator bool() const
{
    return _entry || _asset;
}

template <typename T>
T& AssetHandle<T>::operator*() const
{
    if (_asset)
    {
        return *_asset;
    }
    else if (_entry)
    {
        return *_entry->get();
    }

    throw Error("Asset handle does not have an entry or asset");
}

template <typename T>
T* AssetHandle<T>::operator->() const
{
    return &**this;
}

template <typename T>
bool AssetHandle<T>::operator==(const AssetHandle<T>& handle) const
{
    if (_entry)
    {
        return _entry == handle._entry;
    }
    else if (_asset)
    {
        return _asset == handle._asset;
    }
    return false;
}

template <typename T>
bool AssetHandle<T>::operator!=(const AssetHandle<T>& handle) const
{
    return !(*this == handle);
}

template <typename T>
Encoder& operator<<(Encoder& encoder, const AssetHandle<T>& assetHandle)
{
    return encoder << assetHandle.path().asString();
}

template <typename T>
Decoder& operator>>(Decoder& decoder, AssetHandle<T>& assetHandle)
{
    std::string path;
    decoder >> decodeValue(path);
    assetHandle = decoder.assetCache().getHandle<T>(path);
    return decoder;
}

}