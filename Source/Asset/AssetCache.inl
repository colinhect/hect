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
namespace hect
{

template <typename T>
T& AssetCache::get(const Path& path)
{
    return *getHandle<T>(path).getShared();
}

template <typename T>
AssetHandle<T> AssetCache::getHandle(const Path& path)
{
    std::shared_ptr<AssetEntry<T>> entry;

    auto it = _entries.find(path);
    if (it == _entries.end())
    {
        // First time this asset was requested so create a new entry
        entry.reset(new AssetEntry<T>(*this, path));

        // Add the new entry to the entry map
        _entries[path] = entry;
    }
    else
    {
        // There is already an entry for this asset.
        entry = std::dynamic_pointer_cast<AssetEntry<T>>((*it).second);

        // Throw an error if the asset is not of the same type as the template
        // type
        if (!entry)
        {
            throw Error(format("Asset '%s' is not of the expected type", path.toString().c_str()));
        }
    }

    return AssetHandle<T>(entry);
}

}