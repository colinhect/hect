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
#include "Hect/Core/Logging.h"
#include "Hect/Core/Format.h"
#include "Hect/IO/AssetDecoder.h"
#include "Hect/IO/FileSystem.h"
#include "Hect/Timing/Timer.h"

namespace hect
{

template <typename T>
AssetEntry<T>::AssetEntry(AssetCache& asset_cache, const Path& path, std::function<T*()> constructor) :
    _asset_cache(asset_cache),
    _path(path),
    _constructor(constructor)
{
    initiate_load();
}

template <typename T>
void AssetEntry<T>::refresh(bool force)
{
    if (_asset)
    {
        TimeStamp last_modified = _asset_cache.file_system().last_modified(_path);
        if (force || last_modified > _last_modified)
        {
            T* asset = _asset.get();
            delete asset;
            _asset.release();

            _exception_occurred = false;
            _exception_message.clear();

            initiate_load();
        }
    }
}

template <typename T>
std::unique_ptr<T>& AssetEntry<T>::get()
{
    if (_task_handle)
    {
        _task_handle->wait();
    }

    return _asset;
}

template <typename T>
const Path& AssetEntry<T>::path() const
{
    return _path;
}

template <typename T>
void AssetEntry<T>::initiate_load()
{
    TaskPool& task_pool = _asset_cache.task_pool();
    _task_handle = task_pool.enqueue([this]
    {
        load();
    });
}

template <typename T>
void AssetEntry<T>::load()
{
    _asset.reset(_constructor());
    _exception_occurred = false;

    // Load the asset and keep the error message
    try
    {
        Timer timer;

        _asset->set_name(_path.as_string());

        AssetDecoder decoder(_asset_cache, _path);
        decoder >> decode_value(*_asset);

        HECT_INFO(format("Loaded asset '%s' in %ims", _path.as_string().data(), Milliseconds(timer.elapsed()).value));

        // Remember when the file was last modified
        _last_modified = _asset_cache.file_system().last_modified(_path);
    }
    catch (const std::exception& error)
    {
        // Save the error message
        _exception_occurred = true;
        _exception_message = error.what();

        // Log the error
        HECT_ERROR(format("Failed to load asset '%s': %s", _path.as_string().data(), _exception_message.data()));
    }
}

}
