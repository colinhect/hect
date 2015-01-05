///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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
#include "Hect/Core/Format.h"

namespace hect
{

template <typename T, typename... Args>
T& AssetCache::get(const Path& path, Args&&... args)
{
    return *getHandle<T>(path, args...);
}

template <typename T, typename... Args>
AssetHandle<T> AssetCache::getHandle(const Path& path, Args&&... args)
{
    std::lock_guard<std::recursive_mutex> lock(_mutex);

    std::shared_ptr<AssetEntry<T>> entry;

    Path resolvedPath = resolvePath(path);

    auto it = _entries.find(resolvedPath);
    if (it == _entries.end())
    {
        // First time this asset was requested so create a new entry
        entry.reset(new AssetEntry<T>(*this, resolvedPath, [&]()
        {
            return new T(args...);
        }));

        // Add the new entry to the entry map
        _entries[resolvedPath] = entry;
    }
    else
    {
        // There is already an entry for this asset.
        entry = std::dynamic_pointer_cast<AssetEntry<T>>((*it).second);

        // Throw an error if the asset is not of the same type as the template
        // type
        if (!entry)
        {
            throw InvalidOperation(format("Asset '%s' is not of the expected type", resolvedPath.asString().c_str()));
        }
    }

    return AssetHandle<T>(entry);
}

template <typename T>
void AssetCache::refresh(bool onlyModified)
{
    std::lock_guard<std::recursive_mutex> lock(_mutex);

    bool force = !onlyModified;
    for (auto& pair : _entries)
    {
        AssetEntry<T>* entry = std::dynamic_pointer_cast<AssetEntry<T>>(pair.second);
        if (entry)
        {
            entry->refresh(force);
        }
    }
}

}