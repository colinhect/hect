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
#include "Hect/Core/Logging.h"
#include "Hect/Core/Format.h"
#include "Hect/IO/AssetDecoder.h"
#include "Hect/IO/FileSystem.h"
#include "Hect/Timing/Timer.h"

namespace hect
{

template <typename T>
AssetEntry<T>::AssetEntry(AssetCache& assetCache, const Path& path, std::function<T*()> constructor) :
    _assetCache(assetCache),
    _path(path),
    _constructor(constructor)
{
    initiateLoad();
}

template <typename T>
void AssetEntry<T>::refresh(bool force)
{
    if (_asset)
    {
        TimeStamp lastModified = _assetCache.fileSystem().lastModified(_path);
        if (force || lastModified > _lastModified)
        {
            T* asset = _asset.get();
            delete asset;
            _asset.release();

            _errorOccurred = false;
            _errorMessage.clear();

            initiateLoad();
        }
    }
}

template <typename T>
std::unique_ptr<T>& AssetEntry<T>::get()
{
    if (_taskHandle)
    {
        _taskHandle->wait();
    }

    // Thow an error if the asset failed to load
    if (_errorOccurred)
    {
        throw Error(format("Failed to load asset '%s': %s", _path.asString().c_str(), _errorMessage.c_str()));
    }

    return _asset;
}

template <typename T>
const Path& AssetEntry<T>::path() const
{
    return _path;
}

template <typename T>
void AssetEntry<T>::initiateLoad()
{
    TaskPool& taskPool = _assetCache.taskPool();
    _taskHandle = taskPool.enqueue([this]
    {
        load();
    });
}

template <typename T>
void AssetEntry<T>::load()
{
    _asset.reset(_constructor());
    _errorOccurred = false;

    // Load the asset and keep the error message
    try
    {
        Timer timer;

        _asset->setName(_path.asString());

        AssetDecoder decoder(_assetCache, _path);
        decoder >> decodeValue(*_asset);

        HECT_INFO(format("Loaded '%s' in %ims", _path.asString().c_str(), timer.elapsed().milliseconds()));

        // Remember when the file was last modified
        _lastModified = _assetCache.fileSystem().lastModified(_path);
    }
    catch (std::exception& error)
    {
        // Save the error message
        _errorOccurred = true;
        _errorMessage = error.what();
    }
}

}
