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
namespace hect
{

template <typename RenderObjectType>
Renderer::Data<RenderObjectType>::Data(Renderer& renderer, RenderObjectType& object) :
    renderer(&renderer),
    object(&object)
{
}

template <typename RenderObjectType>
void Renderer::Data<RenderObjectType>::invalidate()
{
    renderer = nullptr;
    object = nullptr;
}

template <typename RenderObjectType>
Renderer::DataHandle<RenderObjectType>::DataHandle()
{
}

template <typename RenderObjectType>
Renderer::DataHandle<RenderObjectType>::DataHandle(Data<RenderObjectType>* data) :
    data(data)
{
}

template <typename RenderObjectType>
Renderer::DataHandle<RenderObjectType>::DataHandle(const DataHandle<RenderObjectType>& handle)
{
    (void)handle;
}

template <typename RenderObjectType>
Renderer::DataHandle<RenderObjectType>::DataHandle(DataHandle<RenderObjectType>&& handle) :
    data(std::move(handle.data))
{
}

template <typename RenderObjectType>
Renderer::DataHandle<RenderObjectType>& Renderer::DataHandle<RenderObjectType>::operator=(const DataHandle<RenderObjectType>& handle)
{
    (void)handle;
    return *this;
}

template <typename RenderObjectType>
Renderer::DataHandle<RenderObjectType>& Renderer::DataHandle<RenderObjectType>::operator=(DataHandle<RenderObjectType>&& handle)
{
    data = std::move(handle.data);
    return *this;
}

template <typename RenderObjectType>
void Renderer::DataHandle<RenderObjectType>::reset(Data<RenderObjectType>* data)
{
    // Re-use the allocated data if possible
    if (this->data)
    {
        *this->data = *data;

        // Invalidate the old data before deleting it to ensure that the
        // destructor does not destroy the object it represents
        data->invalidate();
        delete data;
    }
    else
    {
        this->data.reset(data);
    }
}

template <typename RenderObjectType>
bool Renderer::Object<RenderObjectType>::is_uploaded() const
{
    return _renderer != nullptr && _handle.data.get() != nullptr;
}

template <typename RenderObjectType>
Renderer& Renderer::Object<RenderObjectType>::renderer()
{
    if (!_renderer)
    {
        throw InvalidOperation("Object is not uploaded to GPU");
    }
    return *_renderer;
}

template <typename RenderObjectType>
template <typename U>
U* Renderer::Object<RenderObjectType>::data_as() const
{
    Data<RenderObjectType>* data = _handle.data.get();
    return reinterpret_cast<U*>(data);
}

template <typename RenderObjectType>
void Renderer::Object<RenderObjectType>::set_as_uploaded(Renderer& renderer, Data<RenderObjectType>* data)
{
    _renderer = &renderer;
    _handle.reset(data);
}

template <typename RenderObjectType>
void Renderer::Object<RenderObjectType>::set_as_destroyed()
{
    _renderer = nullptr;
}

}
