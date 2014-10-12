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

namespace hect
{

class Renderer;

///
/// Data which provides a handle to the API-specific data.
template <typename T>
class GpuData
{
public:

    ///
    /// Constructs the data given the renderer and the object that the data
    /// represents.
    ///
    /// \param renderer The render that the data is uploaded to.
    /// \param object The object that the data represents.
    GpuData(Renderer& renderer, T& object);

    virtual ~GpuData();

protected:
    Renderer* renderer;
    T* object;
};


///
/// A handle to GPU data.
///
/// \note Copying results in the data not being copied.  Moving results in the
/// data being moved.
template <typename T>
class GpuDataHandle
{
public:

    ///
    /// Constructs a handle with no data.
    GpuDataHandle();

    ///
    /// Constructs a handle given the data.
    ///
    /// \param data The data.
    GpuDataHandle(GpuData<T>* data);

    ///
    /// Constructs a handle copied from another.
    ///
    /// \param handle The handle to copy.
    GpuDataHandle(const GpuDataHandle<T>& handle);

    ///
    /// Constructs a handle moved from another.
    ///
    /// \param handle The handle to move.
    GpuDataHandle(GpuDataHandle<T>&& handle);

    ///
    /// Sets the handle as a copy of another.
    ///
    /// \param handle The handle to copy.
    GpuDataHandle& operator=(const GpuDataHandle<T>& handle);

    ///
    /// Sets the handle as being moved from another.
    ///
    /// \param handle The handle to move.
    GpuDataHandle& operator=(GpuDataHandle<T>&& handle);

    ///
    /// The data that the handle represents.
    std::unique_ptr<GpuData<T>> data;
};

}

#include "GpuData.inl"