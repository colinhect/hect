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
GpuObject<T>::~GpuObject()
{
}

template <typename T>
bool GpuObject<T>::isUploaded() const
{
    return _renderer != nullptr && _handle.data.get() != nullptr;
}

template <typename T>
Renderer& GpuObject<T>::renderer()
{
    if (!_renderer)
    {
        throw Error("Object is not uploaded to GPU");
    }
    return *_renderer;
}

template <typename T>
template <typename U>
U* GpuObject<T>::dataAs() const
{
    GpuData<T>* data = _handle.data.get();
    return reinterpret_cast<U*>(data);
}

template <typename T>
void GpuObject<T>::setAsUploaded(Renderer& renderer, GpuData<T>* data)
{
    _renderer = &renderer;
    _handle = GpuDataHandle<T>(data);
}

template <typename T>
void GpuObject<T>::setAsDestroyed()
{
    _renderer = nullptr;
}

}