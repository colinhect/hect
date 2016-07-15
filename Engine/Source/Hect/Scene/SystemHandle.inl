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
SystemHandle<T>::SystemHandle()
{
}

template <typename T>
SystemHandle<T>::SystemHandle(Scene& scene, SystemTypeId typeId) :
    _scene(&scene),
    _typeId(typeId)
{
}

template <typename T>
void SystemHandle<T>::invalidate()
{
    _scene = nullptr;
    _typeId = SystemTypeId(-1);
}

template <typename T>
T& SystemHandle<T>::operator*() const
{
    if (!_scene)
    {
        throw InvalidOperation("Invalid system handle");
    }

    return *reinterpret_cast<T*>(&_scene->systemOfTypeId(_typeId));
}

template <typename T>
T* SystemHandle<T>::operator->() const
{
    if (!_scene)
    {
        throw InvalidOperation("Invalid system handle");
    }

    return reinterpret_cast<T*>(&_scene->systemOfTypeId(_typeId));
}

template <typename T>
bool SystemHandle<T>::operator==(const SystemHandle<T>& other) const
{
    return _scene == other._scene && _typeId == other._typeId;
}

template <typename T>
bool SystemHandle<T>::operator!=(const SystemHandle<T>& other) const
{
    return _scene != other._scene || _typeId != other._typeId;
}

template <typename T>
SystemHandle<T>::operator bool() const
{
    return _scene != nullptr && _scene->hasSystemType<T>();
}

template <typename T>
SystemConstHandle<T>::SystemConstHandle()
{
}

template <typename T>
SystemConstHandle<T>::SystemConstHandle(const Scene& scene, SystemTypeId typeId) :
    _scene(&scene),
    _typeId(typeId)
{
}

template <typename T>
const T& SystemConstHandle<T>::operator*() const
{
    if (!_scene)
    {
        throw InvalidOperation("Invalid system handle");
    }

    return *reinterpret_cast<const T*>(&_scene->systemOfTypeId(_typeId));
}

template <typename T>
const T* SystemConstHandle<T>::operator->() const
{
    if (!_scene)
    {
        throw InvalidOperation("Invalid system handle");
    }

    return reinterpret_cast<const T*>(&_scene->systemOfTypeId(_typeId));
}

template <typename T>
bool SystemConstHandle<T>::operator==(const SystemConstHandle<T>& other) const
{
    return _scene == other._scene && _typeId == other._typeId;
}

template <typename T>
bool SystemConstHandle<T>::operator!=(const SystemConstHandle<T>& other) const
{
    return _scene != other._scene || _typeId != other._typeId;
}

template <typename T>
SystemConstHandle<T>::operator bool() const
{
    return _scene != nullptr && _scene->hasSystemType<T>();
}

}
