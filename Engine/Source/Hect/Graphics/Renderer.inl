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

template <typename T>
Renderer::Data<T>::Data(Renderer& renderer, T& object) :
    renderer(&renderer),
    object(&object)
{
}

template <typename T>
void Renderer::Data<T>::invalidate()
{
    renderer = nullptr;
    object = nullptr;
}

template <typename T>
Renderer::DataHandle<T>::DataHandle()
{
}

template <typename T>
Renderer::DataHandle<T>::DataHandle(Data<T>* data) :
    data(data)
{
}

template <typename T>
Renderer::DataHandle<T>::DataHandle(const DataHandle<T>& handle)
{
    (void)handle;
}

template <typename T>
Renderer::DataHandle<T>::DataHandle(DataHandle<T>&& handle) :
    data(std::move(handle.data))
{
}

template <typename T>
Renderer::DataHandle<T>& Renderer::DataHandle<T>::operator=(const DataHandle<T>& handle)
{
    (void)handle;
    return *this;
}

template <typename T>
Renderer::DataHandle<T>& Renderer::DataHandle<T>::operator=(DataHandle<T>&& handle)
{
    data = std::move(handle.data);
    return *this;
}

template <typename T>
void Renderer::DataHandle<T>::reset(Data<T>* data)
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

template <typename T>
bool Renderer::Object<T>::is_uploaded() const
{
    return _renderer != nullptr && _handle.data.get() != nullptr;
}

template <typename T>
Renderer& Renderer::Object<T>::renderer()
{
    if (!_renderer)
    {
        throw InvalidOperation("Object is not uploaded to GPU");
    }
    return *_renderer;
}

template <typename T>
template <typename U>
U* Renderer::Object<T>::data_as() const
{
    Data<T>* data = _handle.data.get();
    return reinterpret_cast<U*>(data);
}

template <typename T>
void Renderer::Object<T>::set_as_uploaded(Renderer& renderer, Data<T>* data)
{
    _renderer = &renderer;
    _handle.reset(data);
}

template <typename T>
void Renderer::Object<T>::set_as_destroyed()
{
    _renderer = nullptr;
}

}
