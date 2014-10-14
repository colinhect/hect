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
GraphicsContext::Data<T>::Data(GraphicsContext& graphicsContext, T& object) :
    graphicsContext(&graphicsContext),
    object(&object)
{
}

template <typename T>
GraphicsContext::Data<T>::~Data()
{
}

template <typename T>
void GraphicsContext::Data<T>::invalidate()
{
    graphicsContext = nullptr;
    object = nullptr;
}

template <typename T>
GraphicsContext::DataHandle<T>::DataHandle()
{
}

template <typename T>
GraphicsContext::DataHandle<T>::DataHandle(Data<T>* data) :
    data(data)
{
}

template <typename T>
GraphicsContext::DataHandle<T>::DataHandle(const DataHandle<T>& handle)
{
    handle;
}

template <typename T>
GraphicsContext::DataHandle<T>::DataHandle(DataHandle<T>&& handle) :
    data(std::move(handle.data))
{
}

template <typename T>
GraphicsContext::DataHandle<T>& GraphicsContext::DataHandle<T>::operator=(const DataHandle<T>& handle)
{
    handle;
    return *this;
}

template <typename T>
GraphicsContext::DataHandle<T>& GraphicsContext::DataHandle<T>::operator=(DataHandle<T>&& handle)
{
    data = std::move(handle.data);
    return *this;
}

template <typename T>
void GraphicsContext::DataHandle<T>::reset(Data<T>* data)
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
GraphicsContext::Object<T>::~Object()
{
}

template <typename T>
bool GraphicsContext::Object<T>::isUploaded() const
{
    return _graphicsContext != nullptr && _handle.data.get() != nullptr;
}

template <typename T>
GraphicsContext& GraphicsContext::Object<T>::graphicsContext()
{
    if (!_graphicsContext)
    {
        throw Error("Object is not uploaded to GPU");
    }
    return *_graphicsContext;
}

template <typename T>
template <typename U>
U* GraphicsContext::Object<T>::dataAs() const
{
    Data<T>* data = _handle.data.get();
    return reinterpret_cast<U*>(data);
}

template <typename T>
void GraphicsContext::Object<T>::setAsUploaded(GraphicsContext& graphicsContext, Data<T>* data)
{
    _graphicsContext = &graphicsContext;
    _handle.reset(data);
}

template <typename T>
void GraphicsContext::Object<T>::setAsDestroyed()
{
    _graphicsContext = nullptr;
}

}